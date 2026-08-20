#ifndef __DHT11_H
#define __DHT11_H

#include "stm32f10x.h"                  // Device header
 

//DHT11引脚宏定义
#define DHT11_GPIO_PORT  GPIOA
#define DHT11_GPIO_PIN   GPIO_Pin_8
#define DHT11_GPIO_CLK   RCC_APB2Periph_GPIOA

//DHT11引脚输出高低电平
#define DHT11_Low  GPIO_ResetBits(DHT11_GPIO_PORT,DHT11_GPIO_PIN)
#define DHT11_High GPIO_SetBits(DHT11_GPIO_PORT,DHT11_GPIO_PIN)

//输出状态定义
#define OUT 1
#define IN  0


u8 DHT11_Init(void);
void DHT11_Mode(u8 mode);
void DHT11_Rst(void);	
u8 DHT11_Check(void);
u8 DHT11_Read_Bit(void);
u8 DHT11_Read_Byte(void);
u8 DHT11_Read_Data(u8 *temp,u8 *humi);

#endif
