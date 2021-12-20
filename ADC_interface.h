#ifndef __ADC_INTERFACE_H__
#define __ADC_INTERFACE_H__

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "fm33LG0XX_include_all.h"


#define ADC_TIME_OUT		(300U)
#define ADC_V_REF               (float)(1.2) // page 139 in ref.
#define ADC_V_DD                (float)(3.2)
#define ADC_RESOLUTION          (4095)

#undef  ADC_VREFP // based on reference source config in ADC_CommonInitStruct

typedef enum
{
	ADC_CONVERSION_INCOMPLETE = 0,
	ADC_CONVERSION_COMPLETE = 1,
}ADC_CONVERSION_t;


typedef enum
{
	ADC_IN14 = (0x1U << 14U),
	ADC_IN15 = (0x1U << 15U),
	ADC_IN16 = (0x1U << 16U),
	ADC_IN17 = (0x1U << 17U),
	ADC_IN18 = (0x1U << 18U),
	ADC_IN19 = (0x1U << 19U)
}ADC_CH_t;

void ADC_Init(ADC_CH_t ch);

ADC_CONVERSION_t ADC_Interface_Read(ADC_CH_t ch, uint16_t * ADCData);

void ADC_gGetVREF1P2Sample(uint16_t* sample);

void ADC_test(uint16_t * ADCData);


#endif