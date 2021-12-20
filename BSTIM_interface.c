#include "BSTIM_interface.h"
#include "TIMER_Interface.h" // timer prototypes of renesas
#include "GPIO_Interface.h"

static void (*callBackAPI)(void);

static uint32_t ARR_value = 0, BSTIM_pres = 0;


void BSTIM32_Setup(void) {
    
    FL_BSTIM32_InitTypeDef timInit;

    /*---------------- Timer time base configuration  ----------------*/
    FL_BSTIM32_StructInit(&timInit); // for default values of the timInit struct
    
    timInit.clockSource          = FL_CMU_LPTIM32_CLK_SOURCE_APBCLK;
    timInit.prescaler            = BSTIM_pres;
    timInit.autoReload           = ARR_value;
    timInit.autoReloadState   	 = FL_ENABLE;
    
    FL_BSTIM32_Init(BSTIM32, &timInit);

    /*---------------------------------------------*/

    /*---------------- Interrupt configuration  ----------------*/
    /* Clear flag  */
    FL_BSTIM32_ClearFlag_Update(BSTIM32);

    /* Interrupt enable  */
    FL_BSTIM32_EnableIT_Update(BSTIM32);

    /* Enable and configure NVIC  */
    
    NVIC_SetPriority(BSTIM_IRQn, 2); //Set interrupt priority 
    NVIC_EnableIRQ(BSTIM_IRQn);

    
}


void BSTIM32_gInit_ms(uint32_t ticks_ms) {
	uint32_t temp, sysclk_hz;
	
        sysclk_hz = FL_CMU_GetRCHFClockFreq();
	
	/* APB prescaler, assume AHB is sysclk without prescaler */
	temp = CMU->SYSCLKCR & APB_PRES_MASK;
	if(temp == APB_PRES_DIV_BY_2) {
		sysclk_hz /= 2;
	}
	else if(temp == APB_PRES_DIV_BY_4){
		sysclk_hz /= 4;
	}
	else if(temp == APB_PRES_DIV_BY_8){
		sysclk_hz /= 8;
	}
	else if(temp == APB_PRES_DIV_BY_16){
		sysclk_hz /= 16;
	}
	else {
		/*sysclk_hz doesn't change*/
	}
	/* timer reload value at pres = 0 */
	ARR_value = (uint32_t) ((float)(ticks_ms / 1000.0) * ( (sysclk_hz) / (BSTIM_pres+1) )) - (uint32_t)1;

	BSTIM32_Setup();
}
void BSTIM32_gStart(void){
    FL_BSTIM32_Enable(BSTIM32);
}
void BSTIM32_gStop(void){
	FL_BSTIM32_Disable(BSTIM32);
        NVIC_DisableIRQ(BSTIM_IRQn);
        FL_BSTIM32_ClearFlag_Update(BSTIM32);
}

void BSTIM_IRQHandler(void)
{   
  if(callBackAPI != NULL) {
    callBackAPI();
  }
  FL_BSTIM32_ClearFlag_Update(BSTIM32);
}

void BSTIM32_gSetCallBack(void (*callBack)(void)) {
	callBackAPI = callBack;
}
 
void BSTIM_Test(void) {
  GPIO_Inf_gInit(PORT_A, GPIO_INF_Pin_01, OUTPUT_WITHOUT_PULLUP_PUSHPULL);
  BSTIM32_gInit_ms(10);
  BSTIM32_gSetCallBack(PINA1_Toggle);
  BSTIM32_gStart();
}

void TIMER1_gInit(void) {
  BSTIM32_gInit_ms(10);
}

//TU08 TIMER1_CHECK_ENABLE(void) {
//  
//  
//}

void TIMER1_gStart(void) {
  BSTIM32_gStart();
}

void TIMER1_gStop(void) {
  BSTIM32_gStop();
}