/*
 * My_ADC.c
 *
 *  Created on: 2014-8-10
 *      Author: FGZ
 */
#include "Common.h"
#include "My_ADC.h"
#include "OLED.h"

void init_ADC(void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_3);
    ADCSequenceConfigure(ADC0_BASE, 3, ADC_TRIGGER_PROCESSOR, 0);
    ADCSequenceStepConfigure(ADC0_BASE, 3, 0, ADC_CTL_CH0 | ADC_CTL_IE | ADC_CTL_END);
    ADCSequenceEnable(ADC0_BASE, 3);
    ADCIntClear(ADC0_BASE, 3);
}

void get_ADC_Val(uint32_t *val)
{
    ADCProcessorTrigger(ADC0_BASE, 3);
    while(!ADCIntStatus(ADC0_BASE, 3, false))
    {}
    ADCIntClear(ADC0_BASE, 3);
    ADCSequenceDataGet(ADC0_BASE, 3, val);
}

void battary_V_dis(uint8_t x, uint8_t y,uint32_t batV)
{
    OLED_P6x8Str(x, y, "V:");
    OLED_P6x8Num(x + 12, y, batV / 282);
    OLED_P6x8Str(x + 24, y, ".");
    OLED_P6x8Num(x + 30, y, batV * 100 / 282 % 100 / 10);
    OLED_P6x8Num(x + 36, y, batV * 100 / 282 % 10);
}
