#include "DHT11.h"   // Device header
#include "Delay.h"   // Device header


u8 DHT11_Init(void)
{
	RCC_APB2PeriphClockCmd(DHT11_GPIO_CLK, ENABLE);				 //使能端口时钟
 	GPIO_InitTypeDef  GPIO_InitStructure;	
 	GPIO_InitStructure.GPIO_Pin = DHT11_GPIO_PIN;				 //端口配置
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 	 //推挽输出
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(DHT11_GPIO_PORT, &GPIO_InitStructure);			 //初始化IO口
 	GPIO_SetBits(DHT11_GPIO_PORT,DHT11_GPIO_PIN);				 //输出高
			    
	DHT11_Rst();  												 //复位DHT11
	return DHT11_Check();										 //等待DHT11的回应
}

void DHT11_Mode(u8 mode)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	if(mode)													//mode为1时设置为推挽输出模式
	{
		GPIO_InitStructure.GPIO_Pin = DHT11_GPIO_PIN;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	}
	else														//mode为0时设置为浮空输入模式
	{
		GPIO_InitStructure.GPIO_Pin =  DHT11_GPIO_PIN;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	}
	GPIO_Init(DHT11_GPIO_PORT, &GPIO_InitStructure);
}

void DHT11_Rst(void)	   
{                 
	DHT11_Mode(1); 						//SET OUTPUT（输出模式）
	DHT11_Low; 	     			     	//拉低DQ
	Delay_ms(20);    					//主机至少拉低18ms
	DHT11_High; 						//DQ=1 
	Delay_us(30);     					//主机拉高20~40us
}

u8 DHT11_Check(void) 	   																	//使用u8是为了返回值1为失败0为响应成功
{   
	u8 Retry=0;																				//设置一个用于计时的参数
	DHT11_Mode(0);																		    //SET INPUT（输入模式）	 
    while (GPIO_ReadInputDataBit(DHT11_GPIO_PORT,DHT11_GPIO_PIN)&&Retry<100)				//DHT11会拉低40~80us
	{
		Retry++;
		Delay_us(1);																		//每次加1us
	};	 
	if(Retry>=100)return 1;																	//如果函数等于或大于100则返回1
	else Retry=0;																			//清零函数为下段循环使用
    while (!GPIO_ReadInputDataBit(DHT11_GPIO_PORT,DHT11_GPIO_PIN)&&Retry<100)				//DHT11拉低后会再次拉高40~80us
	{
		Retry++;
		Delay_us(1);
	};
	if(Retry>=100)return 1;	    
	return 0;																				//响应成功
}	

u8 DHT11_Read_Bit(void) 			 														//读取一个位
{
 	u8 Retry=0;																				
	while(GPIO_ReadInputDataBit(DHT11_GPIO_PORT,DHT11_GPIO_PIN)&&Retry<100)					//等待变为低电平
	{
		Retry++;
		Delay_us(1);
	}
	Retry=0;
	while(!GPIO_ReadInputDataBit(DHT11_GPIO_PORT,DHT11_GPIO_PIN)&&Retry<100)				//等待变高电平
	{
		Retry++;
		Delay_us(1);
	}
	Delay_us(40);																			//选择一个中值来区分信号，等待40us是因为判断数据位为1或0，
																							//0信号持续时间为26-28us,1信号持续时间为70us
	if(GPIO_ReadInputDataBit(DHT11_GPIO_PORT,DHT11_GPIO_PIN))return 1;
	else return 0;		   
}

u8 DHT11_Read_Byte(void)    					//读取一个字节													
{        
	u8 i,Dat;									//Dat用来存放读取的数据位
	Dat=0;
	for (i=0;i<8;i++) 
	{
		Dat<<=1; 								//向左移一位共8位
		Dat|=DHT11_Read_Bit();					//按位或
	}						    
	return Dat;
}

u8 DHT11_Read_Data(u8 *temp,u8 *humi)    		//指针参数temp温度，humi湿度
{        
 	u8 Buf[5];									//5个数组
	u8 i;
	DHT11_Rst();								//重置DHT11
	if(DHT11_Check()==0)						//自检是否符合时序
	{
		for(i=0;i<5;i++)						//读取40位数据
		{
			Buf[i]=DHT11_Read_Byte();			//字节赋于Buf数组中
		}
		if((Buf[0]+Buf[1]+Buf[2]+Buf[3])==Buf[4])	//校验和
		{
			*humi=Buf[0];							//第一个数组为湿度的整数，数组[1]为湿度的小数
			*temp=Buf[2];							//第三个数组为温度的整数，数组[3]为温度的小数
		}
	}
	else return 1;									//自检失败
	return 0;	    								//数据接收成功
}
