#ifndef __BSTIM_INTERFACE_H__
#define __BSTIM_INTERFACE_H__

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "fm33LG0XX_include_all.h"
#include "fm33lg0xx_fl_bstim32.h"


/*prescaller values*/
#define APB_PRES_MASK		(0x70000)
#define APB_PRES_DIV_BY_2	(0x4)//100
#define APB_PRES_DIV_BY_4	(0x5)//101
#define APB_PRES_DIV_BY_8	(0x6)//100
#define APB_PRES_DIV_BY_16	(0x7)//100
#define APB_PRES_DIV_BY_1	(0x3)//0xx



void BSTIM32_gInit_ms(uint32_t ticks_ms);
void BSTIM32_gStart(void);
void BSTIM32_gStop(void);
void BSTIM32_Setup(void);
void BSTIM32_gSetCallBack(void (*callBack)(void));
void PINA1_Toggle(void);



#endif