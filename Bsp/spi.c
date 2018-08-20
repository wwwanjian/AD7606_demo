/**
********************************  STM32F10x  *********************************
* @�ļ���     �� spi.c
* @����       �� WJ
* @��汾     �� V3.5.0
* @�ļ��汾   �� V1.0.0
* @����       �� 2018��8��18��
* @ժҪ       �� SPI��ʼ��������
******************************************************************************/


/* ������ͷ�ļ� --------------------------------------------------------------*/
#include "common.h"


/************************************************
�������� �� SPI1_GPIO_Config
��    �� �� SPI1�Ĺܽų�ʼ��
��    �� �� ��
�� �� ֵ �� ��
*************************************************/
static void SPI1_GPIO_Config(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  
  //����SPI1��MISO��PA6��Ϊ��������
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA,&GPIO_InitStructure);
  
  //����SPI1��MOSI��PA7����SPI1��CLK��PA5��Ϊ�����������
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA,&GPIO_InitStructure);
  
  //����SPI1��CS��PA4��Ϊ�������
  GPIO_InitStructure.GPIO_Pin = CS_1_Pin;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(CS_1_Port,&GPIO_InitStructure);
  CS_1_HIGH;
  
}


/************************************************
�������� �� SPI2_GPIO_Config
��    �� �� SPI2�Ĺܽų�ʼ��
��    �� �� ��
�� �� ֵ �� ��
*************************************************/
static void SPI2_GPIO_Config(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
  
  //����SPI2��MISO��PB14��Ϊ��������
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOB,&GPIO_InitStructure);
  
  //����SPI2��MOSI��PB15����SPI2��CLK��PB13��Ϊ�����������
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15|GPIO_Pin_13;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOB,&GPIO_InitStructure);
  
  //����SPI2��CS��PB12��Ϊ�������
  GPIO_InitStructure.GPIO_Pin = CS_2_Pin;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(CS_2_Port,&GPIO_InitStructure);
  CS_2_HIGH;
}

/************************************************
�������� �� SPI1_SPI_Config
��    �� �� SPI1��SPI����
��    �� �� ��
�� �� ֵ �� ��
*************************************************/
static void SPI1_SPI_Config(void)
{
  SPI_InitTypeDef SPI_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_Init(SPI1, &SPI_InitStructure);
  SPI_Cmd(SPI1, ENABLE);
}

/************************************************
�������� �� SPI2_SPI_Config
��    �� �� SPI2��SPI����
��    �� �� ��
�� �� ֵ �� ��
*************************************************/
static void SPI2_SPI_Config(void)
{
  SPI_InitTypeDef SPI_InitStructure;
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_Init(SPI2, &SPI_InitStructure);
  SPI_Cmd(SPI2, ENABLE);
}


/************************************************
�������� �� SPI1_Config
��    �� �� SPI1��ʼ��
��    �� �� ��
�� �� ֵ �� ��
*************************************************/
static void SPI1_Config(void)
{
  SPI1_GPIO_Config();
  SPI1_SPI_Config();
}

/************************************************
�������� �� SPI2_Config
��    �� �� SPI2��ʼ��
��    �� �� ��
�� �� ֵ �� ��
*************************************************/
static void SPI2_Config(void)
{
  SPI2_GPIO_Config();
  SPI2_SPI_Config();
}


/************************************************
�������� �� SPI1_Send_Byte
��    �� �� SPI1��������
��    �� �� Byte:unsigned char
�� �� ֵ �� ��
*************************************************/
void SPI1_Send_Byte(unsigned char Byte)
{
  //������ͼĴ�������û�з����꣬ѭ���ȴ�
  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
  //�����ͼĴ���д��Ҫ���͵����� 
  SPI_I2S_SendData(SPI1,Byte);
}

/************************************************
�������� �� SPI2_Send_Byte
��    �� �� SPI2��������
��    �� �� Byte:unsigned char
�� �� ֵ �� ��
*************************************************/
void SPI2_Send_Byte(unsigned char Byte)
{
  //������ͼĴ�������û�з����꣬ѭ���ȴ�
  while(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_TXE) == RESET);
  //�����ͼĴ���д��Ҫ���͵����� 
  SPI_I2S_SendData(SPI2,Byte);
}

/************************************************
�������� �� SPI1_Send_Receive_Byte
��    �� �� SPI1�������ݺͽ���
��    �� �� Byte:unsigned char
�� �� ֵ �� unsigned char:���յ�������
��    �� �� WJ
*************************************************/
unsigned char SPI1_Send_Receive_Byte(unsigned char Byte)
{
  while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE) == RESET);
  //�����ͼĴ���д������
  SPI_I2S_SendData(SPI1,Byte);
  
  //������ܼĴ���û���յ����ݣ�ѭ��
  while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_RXNE) == RESET);
  return SPI_I2S_ReceiveData(SPI1);
}

/************************************************
�������� �� SPI2_Send_Receive_Byte
��    �� �� SPI2�������ݺͽ���
��    �� �� Byte:unsigned char
�� �� ֵ �� unsigned char:���յ�������
*************************************************/
unsigned char SPI2_Send_Receive_Byte(unsigned char Byte)
{
  while(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_TXE) == RESET);
  //�����ͼĴ���д������
  SPI_I2S_SendData(SPI2,Byte);
  
  //������ܼĴ���û���յ����ݣ�ѭ��
  while(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_RXNE) == RESET);
  return SPI_I2S_ReceiveData(SPI2);
}


/************************************************
�������� �� Init_SPIBus
��    �� �� ����SPI��ʼ��
��    �� �� 
�� �� ֵ �� 
*************************************************/
void Init_SPIBus(void)
{
  SPI1_Config();
  SPI2_Config();
}