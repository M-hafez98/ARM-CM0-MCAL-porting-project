#include "ADC_interface.h"
#include "GPIO_Interface.h"


//static float sensitivity;

/*
	pres values are:
	VRTC_ADCCR_CKS_RCMF, VRTC_ADCCR_CKS_RCMF_2, VRTC_ADCCR_CKS_RCMF_4, VRTC_ADCCR_CKS_RCMF_8, VRTC_ADCCR_CKS_RCMF_16, VRTC_ADCCR_CKS_RCMF_32
*/

void ADC_Init(ADC_CH_t ch) {

	FL_ADC_CommonInitTypeDef    ADC_CommonInitStruct;
    FL_ADC_InitTypeDef          ADC_InitStruct ;

	//sensitivity = (float) VREF / resolution;

	/* Configure the pin as an analog function in port A */
	if (ch == ADC_IN18) {
		GPIO_Inf_gInit(PORT_D, GPIO_INF_Pin_10, ANALOG_WITHOUT_SWITH);
	}
	else if (ch == ADC_IN19) {
		GPIO_Inf_gInit(PORT_E, GPIO_INF_Pin_09, ANALOG_WITHOUT_SWITH);
	}
	else if (ch == ADC_IN14){
		GPIO_Inf_gInit(PORT_C, GPIO_INF_Pin_09, ANALOG_WITHOUT_SWITH);
	}
	else if (ch == ADC_IN15){
		GPIO_Inf_gInit(PORT_C, GPIO_INF_Pin_10, ANALOG_WITHOUT_SWITH);
	}
	else if (ch == ADC_IN16){
		GPIO_Inf_gInit(PORT_C, GPIO_INF_Pin_11, ANALOG_WITHOUT_SWITH);
	}
	else if (ch == ADC_IN17){
		GPIO_Inf_gInit(PORT_C, GPIO_INF_Pin_12, ANALOG_WITHOUT_SWITH);
	}

	//ADC Clock setting 
    ADC_CommonInitStruct.operationSource            = FL_ADC_CLK_SOURCE_ADCCLK;
    ADC_CommonInitStruct.clockSource                = FL_CMU_ADC_CLK_SOURCE_RCHF;
    ADC_CommonInitStruct.clockPrescaler             = FL_CMU_ADC_PSC_DIV8;
    ADC_CommonInitStruct.APBClockPrescaler          = FL_ADC_APBCLK_PSC_DIV1;
     /*based on reference source config, update the #undef macro in header file*/
    ADC_CommonInitStruct.referenceSource            = FL_ADC_REF_SOURCE_VDDA;//FL_ADC_REF_SOURCE_VREFP;
    ADC_CommonInitStruct.bitWidth                   = FL_ADC_BIT_WIDTH_12B;
    FL_ADC_CommonInit(&ADC_CommonInitStruct);

    //ADC Register settings 
    ADC_InitStruct.conversionMode                   = FL_ADC_CONV_MODE_SINGLE;
    ADC_InitStruct.autoMode                         = FL_ADC_SINGLE_CONV_MODE_AUTO;//automatic
    ADC_InitStruct.waitMode                         = FL_ENABLE; // wait last conv. data to be read and don't start the next conv.
    ADC_InitStruct.overrunMode                      = FL_ENABLE;//Overwrite the old data
    ADC_InitStruct.scanDirection                    = FL_ADC_SEQ_SCAN_DIR_FORWARD;//Channel positive sequence scan 
    ADC_InitStruct.externalTrigConv                 = FL_ADC_TRIGGER_EDGE_NONE;//Disable hardware trigger signal 
    ADC_InitStruct.triggerSource                    = FL_ADC_TRGI_LUT0; // has no effect since hardware trigger signal disabled
    ADC_InitStruct.fastChannelTime                  = FL_ADC_FAST_CH_SAMPLING_TIME_2_ADCCLK;//Fast channel sampling time 
    ADC_InitStruct.lowChannelTime                   = FL_ADC_SLOW_CH_SAMPLING_TIME_192_ADCCLK;//Slow channel sampling time 
    ADC_InitStruct.oversamplingMode                 = FL_ENABLE;
    ADC_InitStruct.overSampingMultiplier            = FL_ADC_OVERSAMPLING_MUL_8X;//8 times oversampling 
    ADC_InitStruct.oversamplingShift                = FL_ADC_OVERSAMPLING_SHIFT_3B;//Data shift right , /8

    FL_ADC_Init(ADC, &ADC_InitStruct);


}


void ADC_gGetVREF1P2Sample(uint16_t* sample) {
    FL_CMU_SetADCPrescaler(FL_CMU_ADC_PSC_DIV8);
    FL_VREF_EnableVREFBuffer(VREF);
    FL_ADC_EnableSequencerChannel(ADC, FL_ADC_INTERNAL_VREF1P2);//Channel selection VREF 

    FL_ADC_ClearFlag_EndOfConversion(ADC);
    FL_ADC_Enable(ADC);
    FL_ADC_EnableSWConversion(ADC);

    while(FL_ADC_IsActiveFlag_EndOfConversion(ADC) == FL_RESET);

    FL_ADC_ClearFlag_EndOfConversion(ADC);
    *sample = FL_ADC_ReadConversionData(ADC);

    FL_ADC_Disable(ADC);
    FL_ADC_DisableSequencerChannel(ADC, FL_ADC_INTERNAL_VREF1P2);
    FL_VREF_DisableVREFBuffer(VREF);
    
}


ADC_CONVERSION_t ADC_Interface_Read(ADC_CH_t chNum, uint16_t * ADCData) {
	
	ADC_CONVERSION_t status = ADC_CONVERSION_INCOMPLETE;
	uint16_t timeOut = 0;

	//ADC_Init(chNum, VRTC_ADCCR_CKS_RCMF_16);

    FL_CMU_SetADCPrescaler(FL_CMU_ADC_PSC_DIV1);
    FL_ADC_EnableSequencerChannel(ADC, (uint32_t)chNum); //Channel selection ADC_14  

    FL_ADC_ClearFlag_EndOfConversion(ADC);
    FL_ADC_Enable(ADC);
    FL_ADC_EnableSWConversion(ADC);

    while((FL_ADC_IsActiveFlag_EndOfConversion(ADC) == FL_RESET) && (++timeOut < ADC_TIME_OUT));

    FL_ADC_ClearFlag_EndOfConversion(ADC);

    if(timeOut < ADC_TIME_OUT) {
		*ADCData = FL_ADC_ReadConversionData(ADC);
		status = ADC_CONVERSION_COMPLETE;
	}
    
    FL_ADC_Disable(ADC);
    FL_ADC_DisableSequencerChannel(ADC, (uint32_t)chNum);//channel closed    	
	
	return status;
}

void ADC_test(uint16_t * ADCData) {
  ADC_Init(ADC_IN14);
  ADC_Interface_Read(ADC_IN14, ADCData);
}