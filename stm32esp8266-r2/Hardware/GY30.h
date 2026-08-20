#ifndef __GY30_H
#define __GY30_H

#include "stm32f10x.h"

#define BH1750_ADDR 0x23 << 1  // ±ê×¼¿â×óÒÆ1Î»

void BH1750_Init(void);
uint16_t BH1750_ReadLight(void);

#endif