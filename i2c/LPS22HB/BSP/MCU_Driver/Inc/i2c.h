#ifndef __IIC_H
#define __IIC_H

#include "gpio.h"
//IO方向设置
 

//IO操作函数	 
#define IIC_SCL    PBout(6)         //SCL
#define IIC_SDA    PBout(7)         //SDA	 
#define READ_SDA   PBin(7)          //输入SDA 

//IIC所有操作函数
void IIC_Init(void);                //初始化IIC的IO口				 
void IIC_Start(void);				        //发送IIC开始信号
void IIC_Stop(void);	  			      //发送IIC停止信号
void IIC_Send_Byte(uint8_t txd);			    //IIC发送一个字节
uint8_t IIC_Read_Byte(void);//IIC读取一个字节
uint8_t IIC_Wait_Ack(void); 				      //IIC等待ACK信号
void IIC_Ack(uint8_t AckBit);					        //IIC发送ACK信号
void IIC_NAck(void);				        //IIC不发送ACK信号

void MX_I2C2_Init(void);
void I2C2_ScanDevices(void);
/************iic write lsm6dsl*******************/
HAL_StatusTypeDef LSM_WriteReg(uint8_t reg, uint8_t value);
HAL_StatusTypeDef LSM_ReadReg(uint8_t reg, uint8_t *data);
HAL_StatusTypeDef LSM_ReadMultiReg(uint8_t reg, uint8_t *data, uint16_t len) ;

HAL_StatusTypeDef LPS_WriteReg(uint8_t reg, uint8_t value);
HAL_StatusTypeDef LPS_ReadReg(uint8_t reg, uint8_t *data);
HAL_StatusTypeDef LPS_ReadMultiReg(uint8_t reg, uint8_t *data, uint16_t len) ;
//uint8_t LSM_ReadReg(uint8_t RegAddress);
//void LSM_WriteReg(uint8_t RegAddress, uint8_t Data);
void get_pressure(void);
#endif
















