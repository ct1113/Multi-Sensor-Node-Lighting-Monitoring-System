#include "INA226.h"
#include "Delay.h"
#include "MyI2C.h"
 
 
//配置INA226
void INA226_Init(void)
{	
	MyI2C_Init();
	Delay_ms(10);
	//0100 010 100 100 111设置转换时间1.1ms,求平均值次数16，分流和总线连续模式
	INA226_SendData(CFG_REG,0x4527);
	INA226_SendData(CAL_REG,0x2800);//设置分流电压转电流转换参数 以最大测1A来设置分辨率
}
 
//发送16位数据到INA226:reg是寄存器地址，data是要发送的16位数据
void INA226_SendData(uint8_t reg,uint16_t data)
{
	uint8_t temp=0;
	MyI2C_Start();//IIC开始
	MyI2C_SendByte(INA226_ADDR1); //这里我就直接把从机的写地址写入函数了（也就是0x80），没作为传参参数
	MyI2C_ReceiveAck(); //接收应答
	
	MyI2C_SendByte(reg);//发送寄存器地址，表明我们对INA226的这个寄存器进行写入
	MyI2C_ReceiveAck();//接收应答
	
	temp = (uint8_t)(data>>8); 
	MyI2C_SendByte(temp);//写入高8位
	MyI2C_ReceiveAck();//接收应答
	
	temp = (uint8_t)(data&0x00FF);
	MyI2C_SendByte(temp); //写入低8位
	MyI2C_ReceiveAck();//接收应答
	
	MyI2C_Stop();//IIC结束
}
 
//接收来自INA226的16位数据，reg表示寄存器地址
uint16_t INA226_ReadData(uint8_t reg)
{
	
	
	uint16_t temp=0;
	
	MyI2C_Start();//IIC开始
	MyI2C_SendByte(INA226_ADDR1); //发送从机写地址
	MyI2C_ReceiveAck();//接收从机应答
	
	MyI2C_SendByte(reg);//发送寄存器地址
	MyI2C_ReceiveAck();//接收从机应答
	
	MyI2C_Stop();//IIC停止
	
	MyI2C_Start();//IIC开始
	
	MyI2C_SendByte(INA226_ADDR1+1);//从机写地址+1 = 从机读地址
	MyI2C_ReceiveAck();//接收从机应答
	
	
	temp = MyI2C_ReceiveByte();//接收高8位数据到temp
	MyI2C_SendAck(0);//0表示发送应答给从机，让从机不要再发数据了，我先把这16位接收完
	temp = temp <<8;//左移8位，让出低位
	temp = temp | MyI2C_ReceiveByte();//接收低8位数据到temp
	MyI2C_SendAck(1);//1表示发送应答
 
	MyI2C_Stop();//IIC停止
	

	
	return temp; 
}
 
//获取 id
uint16_t  INA226_GetID(void)
{
	uint16_t temp=0;
	temp = INA226_ReadData(INA226_GET_ADDR);
	return temp;
}
 
//获取校准值
uint16_t  INA226_GetCAL(void)
{
	uint16_t temp=0;
	temp = INA226_ReadData(CAL_REG);
	return temp;	
}
//获取配置值
uint16_t  INA226_GetCFG(void)
{
	uint16_t temp=0;
	temp = INA226_ReadData(CFG_REG);
	return temp;	
}
 
 
//获取总线电压BV
uint16_t INA226_GetBV(void)
{

	uint16_t temp=0;
	temp = INA226_ReadData(BV_REG);
	

	return temp;
	
}
 
//获取分流电压寄存器的值（只是寄存器值，不是真实电压值）
uint16_t INA226_GetSV(void)
{

	uint16_t temp=0;
	temp = INA226_ReadData(SV_REG);
	
	return temp;
	
}
 
//获取电流寄存器的值（只是寄存器值，不是真实电压值）
uint16_t INA226_GetSC(void)
{
	uint16_t temp=0;
	temp = INA226_ReadData(CUR_REG);
	return temp;
}
 