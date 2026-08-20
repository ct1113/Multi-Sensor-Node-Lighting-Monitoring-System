#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Serial.h"
#include "LED.h"
#include "string.h"
#include "Timer.h"
#include "DHT11.h"
#include <stdlib.h>


#include "MyI2C.h"
#include "INA226.h"
#include "GY30.h"


u8 temp,temp1,temp2;
u8 humi,humi1,humi2;   



uint16_t BV, SC, POWER;
float BVF, SCF, POWERF;
	
uint16_t light;
	
	
int main(void)
{

	/*模块初始化*/
	LED_Init();				//LED初始化
	OLED_Init();
	
	DHT11_Init();

	
	Serial_Init();

	
	MyI2C_Init();
	INA226_Init();
    BH1750_Init();



//LED1_ON();
	
	//printf("Start \r\n");
	//Delay_ms(1000);
	

	 
	OLED_ShowString(1, 1, "BV:");
	OLED_ShowString(2, 1, "SC:");
	
	OLED_ShowString(3, 1, "temp:");
	OLED_ShowString(3, 9, "humi:");
	OLED_ShowString(4, 1, "illu:");
	

	
	printf("AT+RST\r\n");					//复位
	Delay_s(10);
	
//	printf("AT+CWJAP=\"HiwonderESP\",\"hiwonder\"\r\n");	//连接wifi
	printf("AT+CWJAP=\"TP-LINK_90D0C8\",\"\"\r\n");	//连接wifi
//	printf("AT+CWJAP=\"be\",\"2005zxcvbnm\"\r\n");	//连接wifi

	Delay_s(10);


//	printf("AT+CIPSTART=\"TCP\",\"192.168.129.66\",20000\r\n");	//建立TCP/UDP连接
	printf("AT+CIPSTART=\"TCP\",\"192.168.1.101\",20000\r\n");	//建立TCP/UDP连接
//	printf("AT+CIPSTART=\"TCP\",\"192.168.118.66\",20000\r\n");

	Delay_s(1);
	printf("AT+CIPSEND=19\r\n");	//发送数据字节数
	Delay_s(1);
	printf("{\"tag\":\"ledroom02\"}\r\n");	//发送数据字节数
	Delay_s(5);

	Timer_Init();
	

	while (1)
	{
		BV = INA226_GetBV();  //读取电压 U1
		SC = INA226_GetSC(); //读取电流  I1
	//	POWER = BV*SC; 
 
		
		BVF = BV*0.00125f; //计算实际总线电压
		
		//BVF = BV*0.00116f; //计算实际总线电压
		SCF = SC*0.0001f;
		//SCF = SC*0.0000592857f;//计算实际电流
	//	POWERF = BVF*SCF;
		if(SCF>1)
		{
			SCF=0;
		}
		
        // OLED_ShowString(1, 1, "F:");
	   // OLED_ShowHexNum(1, 3, INA226_GetCFG(), 4); //获取配置值
	   //  OLED_ShowString(1, 8, "A:");
	  //  OLED_ShowHexNum(1, 10, INA226_GetCAL(), 4); //获取校准值
		
		
		OLED_ShowString(1, 5, ".");
	//	OLED_ShowNum(1, 4, BV, 4); //获取总线电压
		OLED_ShowNum(1, 4, BVF, 1);//显示真实值
		
		
		OLED_ShowNum(1, 6, ((uint8_t)((BVF)*10))%10, 1);
		OLED_ShowNum(1, 7, ((uint8_t)((BVF)*100))%100, 1);
		OLED_ShowNum(1, 8, ((uint8_t)((BVF)*1000))%1000, 1);
		OLED_ShowNum(1, 9, ((uint8_t)((BVF)*10000))%10000, 1);
		OLED_ShowString(1, 10, "V");
	
 
		
		OLED_ShowString(2, 5, ".");
	//	OLED_ShowNum(2, 4, SC, 4); //获取分流电阻电流
		OLED_ShowNum(2, 4, SCF, 1);//显示真实值
		
		
		OLED_ShowNum(2, 6, ((uint8_t)((SCF)*10))%10, 1);
		OLED_ShowNum(2, 7, ((uint8_t)((SCF)*100))%100, 1);
		OLED_ShowNum(2, 8, ((uint8_t)((SCF)*1000))%1000, 1);
		OLED_ShowNum(2, 9, ((uint8_t)((SCF)*10000))%10000, 1);
		OLED_ShowString(2, 10, "A");

		
	//	OLED_ShowString(4, 1, "PW:");
	//	OLED_ShowString(4, 10, ".");
	//	OLED_ShowNum(4, 4, POWER, 4); //获取功率
	//	OLED_ShowNum(4, 9, POWERF, 1);//显示真实值
	//	OLED_ShowNum(4, 11, ((uint8_t)((POWER)*10))%10, 1);
	//	OLED_ShowNum(4, 12, ((uint8_t)(POWER*100))%100, 1);
	//	OLED_ShowNum(4, 13, ((uint8_t)(POWER*1000))%1000, 1);
	//	OLED_ShowNum(4, 14, ((uint8_t)(POWER*10000))%10000, 1);
	//	OLED_ShowString(4, 15, "W");
		
	//	Delay_ms(100); //也可以不delay delay是便于观察屏幕
	
		
	}
}

void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
	{
		
		light = BH1750_ReadLight();
		OLED_ShowNum(4,6,light,4);//单位是 lux（勒克斯）
		
		BV = INA226_GetBV();  //读取电压 U1
		SC = INA226_GetSC(); //读取电流  I1
	//	POWER = BV*SC; 
 
		//BVF = BV*0.00116f; //计算实际总线电压
		//SCF = SC*0.0000592857f;//计算实际电流
		BVF = BV*0.00125f; //计算实际总线电压
		
		//BVF = BV*0.00116f; //计算实际总线电压
		SCF = SC*0.0001f;
		if(SCF>1)
		{
			SCF=0;
		}
		
		
	//	OLED_ShowString(1, 5, ".");
	//	OLED_ShowNum(1, 4, BVF, 1);//显示真实值
		
		
	//	OLED_ShowNum(1, 6, ((uint8_t)((BVF)*10))%10, 1);
	//	OLED_ShowNum(1, 7, ((uint8_t)((BVF)*100))%100, 1);
	//	OLED_ShowNum(1, 8, ((uint8_t)((BVF)*1000))%1000, 1);
	//	OLED_ShowNum(1, 9, ((uint8_t)((BVF)*10000))%10000, 1);
	//	OLED_ShowString(1, 10, "V");
 
		
	//	OLED_ShowString(2, 5, ".");
	//	OLED_ShowNum(2, 4, SCF, 1);//显示真实值
		
		
	//	OLED_ShowNum(2, 6, ((uint8_t)((SCF)*10))%10, 1);
	//	OLED_ShowNum(2, 7, ((uint8_t)((SCF)*100))%100, 1);
	//	OLED_ShowNum(2, 8, ((uint8_t)((SCF)*1000))%1000, 1);
	//	OLED_ShowNum(2, 9, ((uint8_t)((SCF)*10000))%10000, 1);
	//	OLED_ShowString(2, 10, "A");
		
		DHT11_Read_Data(&temp, &humi);
		OLED_ShowNum(3,6,temp,2);
		OLED_ShowNum(3,14,humi,2);
		
		printf("AT+CIPSEND=42\r\n");	//发送数据字节数
		Delay_s(2);
		printf("{\"sensor_tag\":\"temperature2\",\"value\":\"%d\"}\r\n", temp);	//发送数据字节数
		Delay_s(1);
		
		printf("AT+CIPSEND=39\r\n");	//发送数据字节数
		Delay_s(2);
		printf("{\"sensor_tag\":\"humidity2\",\"value\":\"%d\"}\r\n", humi);	//发送数据字节数
		Delay_s(1);
		
		printf("AT+CIPSEND=41\r\n");	//发送数据字节数
		Delay_s(2);
		printf("{\"sensor_tag\":\"voltage2\",\"value\":\"%.3f\"}\r\n",BVF);	//发送数据字节数
		Delay_s(1);
		
		printf("AT+CIPSEND=41\r\n");	//发送数据字节数
		Delay_s(2);
		printf("{\"sensor_tag\":\"current2\",\"value\":\"%.3f\"}\r\n", SCF);	//发送数据字节数
		Delay_s(1);
		
		printf("AT+CIPSEND=44\r\n");	//发送数据字节数
		Delay_s(2);
		printf("{\"sensor_tag\":\"illuminance2\",\"value\":\"%04d\"}\r\n",light );	//发送数据字节数
		Delay_s(1);



		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}



