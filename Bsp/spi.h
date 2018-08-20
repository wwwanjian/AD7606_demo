/**
  ********************************  STM32F10x  *********************************
  * @�ļ���     �� spi.h
  * @����       �� WJ
  * @��汾     �� V3.5.0
  * @�ļ��汾   �� V1.0.0
  * @����       �� 2018��8��18��
  * @ժҪ       �� SPIͷ�ļ�
  ******************************************************************************/

/* �����ֹ�ݹ���� ----------------------------------------------------------*/
#ifndef _SPI_H
#define _SPI_H

//SPI 1 Ƭѡ
#define CS_1_Pin        GPIO_Pin_4
#define CS_1_Port       GPIOA

#define CS_1_HIGH       GPIO_SetBits(CS_1_Port, CS_1_Pin)
#define CS_1_LOW        GPIO_ResetBits(CS_1_Port, CS_1_Pin)

//SPI 2 Ƭѡ
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