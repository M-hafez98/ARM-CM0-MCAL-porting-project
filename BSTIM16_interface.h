#ifndef __BSTIM16_INTERFACE_H__
#define __BSTIM16_INTERFACE_H__

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "fm33LG0XX_include_all.h"

/*prescaller values*/
#define APB_PRES_MASK		(0x70000)
#define APB_PRES_DIV_BY_2	(0x4)//100
#define APB_PRES_DIV_BY_4	(0x5)//101
#define APB_PRES_DIV_BY_8	(0x6)//100
#define APB_PRES_DIV_BY_16	(0x7)//100
#define APB_PRES_DIV_BY_1	(0x3)//0xx


void BSTIM16_gInit_us(uint32_t delay_us);
void BSTIM16_gWait_us(void);
void BSTIM16_gStop(void);
_Noreturn void BSTIM16_test(void);




#endif