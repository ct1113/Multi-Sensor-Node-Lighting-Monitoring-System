#ifndef __INA226_H__
#define __INA226_H__
#include "stdint.h"
 
 
#define 	CFG_REG	 		0x00		//
#define 	SV_REG 			0x01		//分流电压， 此处分流电阻为 0.1欧
#define 	BV_REG 			0x02		//总线电压
#define 	PWR_REG 		0x03		//电源功率
#define 	CUR_REG 		0x04		//电流
#define 	CAL_REG 		0x05		//校准，设定满量程范围以及电流和功率测数的 
#define 	ONFF_REG 		0x06		//屏蔽 使能 警报配置和转换准备就绪
#define 	AL_REG 			0x07		//包含与所选警报功能相比较的限定值
#define 	INA226_GET_ADDR 0XFF		//包含唯一的芯片标识号
#define   	INA226_ADDR1	0x80 
 
//初始化
void INA226_Init(void);
//发送数据到reg寄存器
void INA226_SendData(uint8_t reg,uint16_t data);
//读取寄存器reg的值
uint16_t INA226_ReadData(uint8_t reg);
 
 
//获取校准值
uint16_t  INA226_GetCAL(void);
//获取配置值
uint16_t  INA226_GetCFG(void);
//获取芯片ID
uint16_t  INA226_GetID(void);
//获取总线电压寄存器的值（只是寄存器值，不是真实电压值）
uint16_t INA226_GetBV(void);
//获取分流电压寄存器的值（只是寄存器值，不是真实电压值）
uint16_t INA226_GetSV(void);
//获取电流寄存器的值（只是寄存器值，不是真实电压值）
uint16_t INA226_GetSC(void);
 
 
#endif