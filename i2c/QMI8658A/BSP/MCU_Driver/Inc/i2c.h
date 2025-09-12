#ifndef __IIC_H
#define __IIC_H

#include "gpio.h"
//IO��������
 

//IO��������	 
#define IIC_SCL    PBout(6)         //SCL
#define IIC_SDA    PBout(7)         //SDA	 
#define READ_SDA   PBin(7)          //����SDA 

//IIC���в�������
void IIC_Init(void);                //��ʼ��IIC��IO��				 
void IIC_Start(void);				        //����IIC��ʼ�ź�
void IIC_Stop(void);	  			      //����IICֹͣ�ź�
void IIC_Send_Byte(uint8_t txd);			    //IIC����һ���ֽ�
uint8_t IIC_Read_Byte(void);//IIC��ȡһ���ֽ�
uint8_t IIC_Wait_Ack(void); 				      //IIC�ȴ�ACK�ź�
void IIC_Ack(uint8_t AckBit);					        //IIC����ACK�ź�
void IIC_NAck(void);				        //IIC������ACK�ź�

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
















