/**
  ********************************  STM32F10x  *********************************
  * @文件名     ： spi.h
  * @作者       ： WJ
  * @库版本     ： V3.5.0
  * @文件版本   ： V1.0.0
  * @日期       ： 2018年8月18日
  * @摘要       ： SPI头文件
  ******************************************************************************/

/* 定义防止递归包含 ----------------------------------------------------------*/
#ifndef _SPI_H
#define _SPI_H

//SPI 1 片选
#define CS_1_Pin        GPIO_Pin_4
#define CS_1_Port       GPIOA

#define CS_1_HIGH       GPIO_SetBits(CS_1_Port, CS_1_Pin)
#define CS_1_LOW        GPIO_ResetBits(CS_1_Port, CS_1_Pin)

//SPI 2 片选
#define CS_2_Pin        GPIO_Pin_12
#define CS_2_Port       GPIOB

#define CS_2_HIGH       GPIO_SetBits(CS_2_Port, CS_2_Pin)
#define CS_2_LOW        GPIO_ResetBits(CS_2_Port, CS_2_Pin)


void Init_SPIBus(void);
void SPI1_Send_Byte(unsigned char Byte);
void SPI2_Send_Byte(unsigned char Byte);
unsigned char SPI1_Send_Receive_Byte(unsigned char Byte);
unsigned char SPI2_Send_Receive_Byte(unsigned char Byte);

#endif