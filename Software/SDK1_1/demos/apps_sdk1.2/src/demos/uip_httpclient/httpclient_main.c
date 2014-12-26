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
#define DHCPC_SERVER_PORT  67
#define DHCPC_CLIENT_PORT  68
#define DNS_PORT        53

#if USE_RTOS
#define APP_TASK_START_PRIO        10
#define TEST_TASK_STACK_SIZE     (2000)
OSA_TASK_DEFINE(httpclient_app, TEST_TASK_STACK_SIZE);

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
 * The function is the real UDP APP CALL to do the UDP application
 */
void MY_UDP_APP(void)
{

    switch (uip_udp_conn->rport)
    {

        case HTONS(DHCPC_SERVER_PORT):
        case HTONS(DHCPC_CLIENT_PORT):
        dhcpc_appcall();
        break;

        case HTONS(DNS_PORT):
        resolv_appcall();
        break;

    }
}

/*---------------------------------------------------------------------------*/
/*
 * The function do the DHCP configuration for host IP address and net mask address 
 * and router IP address with the DHCP addresses.
 */

void
dhcpc_configured(const struct dhcpc_state *s)
{
  uip_ipaddr_t dnsaddr;
  uip_sethostaddr(s->ipaddr);
  uip_setnetmask(s->netmask);
  uip_setdraddr(s->default_router);
  uip_ipaddr_copy(dnsaddr,s->dnsaddr);                 
  resolv_conf(dnsaddr);
#if 1
  printf("Got IP address %d.%d.%d.%d \r\n",
  uip_ipaddr1(s->ipaddr), uip_ipaddr2(s->ipaddr),
  uip_ipaddr3(s->ipaddr), uip_ipaddr4(s->ipaddr));
  printf("Got netmask %d.%d.%d.%d \r\n",
  uip_ipaddr1(s->netmask), uip_ipaddr2(s->netmask),
  uip_ipaddr3(s->netmask), uip_ipaddr4(s->netmask));
  printf("Got DNS server %d.%d.%d.%d \r\n",
  uip_ipaddr1(s->dnsaddr), uip_ipaddr2(s->dnsaddr),
  uip_ipaddr3(s->dnsaddr), uip_ipaddr4(s->dnsaddr));
  printf("Got default router %d.%d.%d.%d \r\n",
  uip_ipaddr1(s->default_router), uip_ipaddr2(s->default_router),
  uip_ipaddr3(s->default_router), uip_ipaddr4(s->default_router));
  printf("Lease expires in %ld seconds \r\n",
  ntohs(s->lease_time[0])*65536ul + ntohs(s->lease_time[1]));
  #endif
}

void
resolv_found(char *name, u16_t *ipaddr)
{  
  if(ipaddr == NULL) {
    printf("Host '%s' not found. \r\n", name);
  } else {
    printf("Found name '%s' = %d.%d.%d.%d \r\n", name,
       htons(ipaddr[0]) >> 8,
       htons(ipaddr[0]) & 0xff,
       htons(ipaddr[1]) >> 8,
       htons(ipaddr[1]) & 0xff);
       webclient_get("www.freescale.com", 80, "/index.html");
  }
}

void webclient_closed(void)
{
  printf("Webclient: connection closed \r\n");
}

void webclient_aborted(void)
{
  printf("Webclient: connection aborted \r\n");
}

void webclient_timedout(void)
{
  printf("Webclient: connection timed out \r\n");
}
void webclient_connected(void)
{
  printf("Webclient: connected, waiting for data... \r\n");
}

void webclient_datahandler(char *data, u16_t len)
{
  printf("Webclient: got %d bytes of data. \r\n", len);
}

/*---------------------------------------------------------------------------*/
/*
 * The is the main loop function
 */
void httpclient_app(task_param_t param)
{
    enet_dev_if_t *enetIfPtr;
    struct timer periodic_timer, arp_timer;
    uint8_t index = 0;
    uint8_t macAddr[kEnetMacAddrLen] = {UIP_ETHADDR0, UIP_ETHADDR1, UIP_ETHADDR2, UIP_ETHADDR3, UIP_ETHADDR4, UIP_ETHADDR5};

    /* Initialize the MACNET device*/
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

    dhcpc_init(macAddr, kEnetMacAddrLen);
    webclient_init();
    resolv_init();
    resolv_query("www.freescale.com");

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

    httpclient_app(NULL);
#else
    /* Create the task*/
    OSA_TaskCreate(httpclient_app, (uint8_t *)"httpclient_app", TEST_TASK_STACK_SIZE, httpclient_app_stack, APP_TASK_START_PRIO, (task_param_t)0, false, &httpclient_app_task_handler);

    OSA_Start();
#endif
}
/*---------------------------------------------------------------------------*/
