/*
 * Copyright (c) 2013 - 2014, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include <stdio.h>
#include <string.h>
#include "uip.h"
#include "uip_arp.h"
#include "uip/timer.h"
#include "macnetdev.h"
#ifndef FSL_RTOS_MQX
#include "board.h"
#endif
#include "fsl_enet_driver.h"
#include "fsl_clock_manager.h"
#include "fsl_os_abstraction.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define BUF ((struct uip_eth_hdr *)&uip_buf[0])

#if USE_RTOS
#define APP_TASK_START_PRIO        10
#define TEST_TASK_STACK_SIZE     (2000)
OSA_TASK_DEFINE(httpsrv_app, TEST_TASK_STACK_SIZE);

#if defined(FSL_RTOS_MQX)
void Main_Task(uint32_t param);
TASK_TEMPLATE_STRUCT  MQX_template_list[] =
{
   { 1L, Main_Task, TEST_TASK_STACK_SIZE, MQX_MAIN_TASK_PRIORITY, "Main", MQX_AUTO_START_TASK},
   { 0L, 0L,        0L,   0L,                     0L,     0L }
};
#endif
#endif
#define BOARD_DEBUG_ENET_INSTANCE (0)

/*---------------------------------------------------------------------------*/
/*
 * This is the UIP log function.
 */
void
uip_log(char *m)
{
  printf("uIP log message: %s \r\n", m);
}


/*---------------------------------------------------------------------------*/
/*
 * The function initialize the hardware.
 */
void app_init_hardware(void)
{
#ifndef FSL_RTOS_MQX
    /* Open UART module for debug */
    hardware_init();
    configure_enet_pins(BOARD_ENET_INSTANCE);
    dbg_uart_init();
#endif

#if FSL_FEATURE_ENET_SUPPORT_PTP 
    /* Select the OSCERCLK as the PTP timer*/
    CLOCK_SYS_SetEnetTimeStampSrc(0, kClockTimeSrcOsc0erClk);
#endif
    /* Disable the MPU*/
    BW_MPU_CESR_VLD(MPU_BASE, 0);
}


/*
 * The is the main loop function
 */
void httpsrv_app(task_param_t param)
{
    enet_dev_if_t *enetIfPtr;
    uip_ipaddr_t ipaddr;
    struct timer periodic_timer, arp_timer;
    uint8_t index = 0;

    /* Initialize the MAC-NET device*/
    enetIfPtr = enet_init(BOARD_DEBUG_ENET_INSTANCE);
    if(!enetIfPtr)
    {
        printf(" enet device init failed! \r\n");
        return;
    }
    else
    {
        printf(" enet device init success! \r\n");
    }    

    /* Initialize the uIp TCP/IP stack*/
    uip_init();
    uip_arp_init();

    /* Initialize the host IP address*/
    uip_ipaddr(ipaddr, 192,168,0,10);
    uip_sethostaddr(ipaddr);
    /* Initialize the router IP address*/
    uip_ipaddr(ipaddr, 192,168,0,1);
    uip_setdraddr(ipaddr);
    /* Initialize the net mask address*/
    uip_ipaddr(ipaddr, 255,255,255,0);
    uip_setnetmask(ipaddr); 

    /* Initialize the HTTP SERVER application api*/
    httpd_init();

    /* Initialize the periodic timer and ARP time out*/
    timer_set(&periodic_timer, CLOCK_SECOND / 2);
    timer_set(&arp_timer, CLOCK_SECOND * 10);

    while(1) {
        enet_read(enetIfPtr);
        if(uip_len > 0) {
            if(BUF->type == htons(UIP_ETHTYPE_IP)) {
                uip_arp_ipin();
                uip_input();
                /* If the above function invocation resulted in data that
                    should be sent out on the network, the global variable
                    uip_len is set to a value > 0. */
                if(uip_len > 0) {
                    uip_arp_out();
                    enet_send(enetIfPtr);
                }
            } else if(BUF->type == htons(UIP_ETHTYPE_ARP)) {
                    uip_arp_arpin();
                    /* If the above function invocation resulted in data that
                     should be sent out on the network, the global variable
                     uip_len is set to a value > 0. */
                if(uip_len > 0) {
                    enet_send(enetIfPtr);
                }
            }

        } else if(timer_expired(&periodic_timer)) {
            timer_reset(&periodic_timer);
            for(index = 0; index < UIP_CONNS; index ++) {
                uip_periodic(index);
                /* If the above function invocation resulted in data that
                   should be sent out on the network, the global variable
                   uip_len is set to a value > 0. */
                if(uip_len > 0) {
                    uip_arp_out();
                    enet_send(enetIfPtr);
                }
            }

#if UIP_UDP
            for(index = 0; index < UIP_UDP_CONNS; index++) {
                uip_udp_periodic(index);
                /* If the above function invocation resulted in data that
                   should be sent out on the network, the global variable
                   uip_len is set to a value > 0. */
                if(uip_len > 0) {
                  uip_arp_out();
                  enet_send(enetIfPtr);
                }
           }
#endif /* UIP_UDP */
      
            /* Call the ARP timer function every 10 seconds. */
            if(timer_expired(&arp_timer)) {
                timer_reset(&arp_timer);
                uip_arp_timer();
            }
        }
    }
}  

#if defined(FSL_RTOS_MQX)
void Main_Task(uint32_t param)
#else
int main(void)
#endif
{
    OSA_Init();

    /* Hardware initialize*/
    app_init_hardware();

#if !USE_RTOS
    /* Initialize hardware timer for bare metal*/
    time_init();

    httpsrv_app(NULL);
#else
    /* Create the task*/
    OSA_TaskCreate(httpsrv_app, (uint8_t *)"httpsrv_app", TEST_TASK_STACK_SIZE, httpsrv_app_stack, APP_TASK_START_PRIO, (task_param_t)0, false, &httpsrv_app_task_handler);

    OSA_Start();
#endif
}

/*---------------------------------------------------------------------------*/
