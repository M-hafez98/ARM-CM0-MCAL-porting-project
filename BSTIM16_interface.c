#include "BSTIM16_interface.h"
#include "TIMER_Interface.h" // timer prototypes of renesas
#include "GPIO_Interface.h" // for test function

 

void BSTIM16_gInit_us(uint32_t delay_us) {

	FL_BSTIM16_InitTypeDef   InitStructer;
	uint32_t temp, sysclk_hz, pres = 0;
          uint32_t ARR_value = 0;
	if((float)(delay_us / 1000000.0) > 2.0) {
		/*invalid value*/
		return;
	}

	else {
		sysclk_hz = FL_CMU_GetRCHFClockFreq();
		/*APB Prescaler, assume AHB is sysclk without prescaler*/
		temp = CMU->SYSCLKCR & APB_PRES_MASK;
		if(temp == APB_PRES_DIV_BY_2) {
			sysclk_hz /= 2;
		}
		else if(temp == APB_PRES_DIV_BY_4) {
			sysclk_hz /= 4;
		}
		else if(temp == APB_PRES_DIV_BY_8) {
			sysclk_hz /= 8;
		}
		else if(temp == APB_PRES_DIV_BY_16) {
			sysclk_hz /= 16;
		}
		else {
			/*sysclk_hz doesn't change*/
		}
		
		/* timer reload value at pres = 0 */
		ARR_value = (uint32_t) ((float)(delay_us / 1000000.0) * ( (sysclk_hz) / (pres+1) )) - (uint32_t)1;
		/* timer reload value at certain pres if ARR value exceeds the reg limit val */
		while(ARR_value > 65535) { 
			pres++;
			ARR_value = (uint32_t) ((float)(delay_us / 1000000.0) * ( (sysclk_hz) / (pres+1) )) - (uint32_t)1;
		}

	InitStructer.prescaler         = pres;
    	InitStructer.autoReload        = ARR_value;
    	InitStructer.autoReloadState   = FL_DISABLE;
    	InitStructer.clockSource       = FL_CMU_BSTIM16_CLK_SOURCE_APBCLK;
        
        FL_BSTIM16_Init(BSTIM16, &InitStructer);
        
        /* Interrupt disable  */
    	FL_BSTIM16_DisableIT_Update(BSTIM16);
        /* Clear flag  */
    	FL_BSTIM16_ClearFlag_Update(BSTIM16);
	}
}

void BSTIM16_gWait_us(void) {
        FL_BSTIM16_ClearFlag_Update(BSTIM16);
	FL_BSTIM16_Enable(BSTIM16);
	while(!FL_BSTIM16_IsActiveFlag_Update(BSTIM16)); // polling until the current counter is greater than or equal to the value of the compare register
	FL_BSTIM16_Disable(BSTIM16);
}

void BSTIM16_gStop(void) {
	FL_BSTIM16_Disable(BSTIM16);
	FL_BSTIM16_ClearFlag_Update(BSTIM16);
	//FL_BSTIM16_WriteCounter(BSTIM16, 0);
}

_Noreturn void BSTIM16_test(void){
  BSTIM16_gInit_us(100);
  GPIO_Inf_gInit(PORT_A, GPIO_INF_Pin_01, OUTPUT_WITHOUT_PULLUP_PUSHPULL);
  while(1)
  {
    GPIO_Inf_gWriteBit(PORT_A, GPIO_INF_Pin_01, Bit_SET);
    BSTIM16_gWait_us();
    GPIO_Inf_gWriteBit(PORT_A, GPIO_INF_Pin_01, Bit_RESET);
    BSTIM16_gWait_us();
  }
}

void TIMER3_gInit(void) {
  BSTIM16_gInit_us(100);
  
}

void TIMER3_ENABLE(void) {
  
  BSTIM16_gWait_us();
  
}

void TIMER3_DISABLE(void) {
  
  BSTIM16_gStop();
}