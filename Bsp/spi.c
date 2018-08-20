/**
********************************  STM32F10x  *********************************
* @文件名     ： spi.c
* @作者       ： WJ
* @库版本     ： V3.5.0
* @文件版本   ： V1.0.0
* @日期       ： 2018年8月18日
* @摘要       ： SPI初始化及功能
******************************************************************************/


/* 包含的头文件 --------------------------------------------------------------*/
#include "common.h"


/************************************************
函数名称 ： SPI1_GPIO_Config
功    能 ： SPI1的管脚初始化
参    数 ： 无
返 回 值 ： 无
*************************************************/
static void SPI1_GPIO_Config(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  
  //配置SPI1的MISO（PA6）为浮空输入
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA,&GPIO_InitStructure);
  
  //配置SPI1的MOSI（PA7）和SPI1的CLK（PA5）为复用推免输出
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA,&GPIO_InitStructure);
  
  //配置SPI1的CS（PA4）为推免输出
  GPIO_InitStructure.GPIO_Pin = CS_1_Pin;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(CS_1_Port,&GPIO_InitStructure);
  CS_1_HIGH;
  
}


/************************************************
函数名称 ： SPI2_GPIO_Config
功    能 ： SPI2的管脚初始化
参    数 ： 无
返 回 值 ： 无
*************************************************/
static void SPI2_GPIO_Config(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
  
  //配置SPI2的MISO（PB14）为浮空输入
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOB,&GPIO_InitStructure);
  
  //配置SPI2的MOSI（PB15）和SPI2的CLK（PB13）为复用推免输出
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15|GPIO_Pin_13;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOB,&GPIO_InitStructure);
  
  //配置SPI2的CS（PB12）为推免输出
  GPIO_InitStructure.GPIO_Pin = CS_2_Pin;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(CS_2_Port,&GPIO_InitStructure);
  CS_2_HIGH;
}

/************************************************
函数名称 ： SPI1_SPI_Config
功    能 ： SPI1的SPI配置
参    数 ： 无
返 回 值 ： 无
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
函数名称 ： SPI2_SPI_Config
功    能 ： SPI2的SPI配置
参    数 ： 无
返 回 值 ： 无
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
函数名称 ： SPI1_Config
功    能 ： SPI1初始化
参    数 ： 无
返 回 值 ： 无
*************************************************/
static void SPI1_Config(void)
{
  SPI1_GPIO_Config();
  SPI1_SPI_Config();
}

/************************************************
函数名称 ： SPI2_Config
功    能 ： SPI2初始化
参    数 ： 无
返 回 值 ： 无
*************************************************/
static void SPI2_Config(void)
{
  SPI2_GPIO_Config();
  SPI2_SPI_Config();
}


/************************************************
函数名称 ： SPI1_Send_Byte
功    能 ： SPI1发送数据
参    数 ： Byte:unsigned char
返 回 值 ： 无
*************************************************/
void SPI1_Send_Byte(unsigned char Byte)
{
  //如果发送寄存器数据没有发送完，循环等待
  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
  //往发送寄存器写入要发送的数据 
  SPI_I2S_SendData(SPI1,Byte);
}

/************************************************
函数名称 ： SPI2_Send_Byte
功    能 ： SPI2发送数据
参    数 ： Byte:unsigned char
返 回 值 ： 无
*************************************************/
void SPI2_Send_Byte(unsigned char Byte)
{
  //如果发送寄存器数据没有发送完，循环等待
  while(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_TXE) == RESET);
  //往发送寄存器写入要发送的数据 
  SPI_I2S_SendData(SPI2,Byte);
}

/************************************************
函数名称 ： SPI1_Send_Receive_Byte
功    能 ： SPI1发送数据和接受
参    数 ： Byte:unsigned char
返 回 值 ： unsigned char:接收到的数据
作    者 ： WJ
*************************************************/
unsigned char SPI1_Send_Receive_Byte(unsigned char Byte)
{
  while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE) == RESET);
  //往发送寄存器写入数据
  SPI_I2S_SendData(SPI1,Byte);
  
  //如果接受寄存器没有收到数据，循环
  while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_RXNE) == RESET);
  return SPI_I2S_ReceiveData(SPI1);
}

/************************************************
函数名称 ： SPI2_Send_Receive_Byte
功    能 ： SPI2发送数据和接受
参    数 ： Byte:unsigned char
返 回 值 ： unsigned char:接收到的数据
*************************************************/
unsigned char SPI2_Send_Receive_Byte(unsigned char Byte)
{
  while(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_TXE) == RESET);
  //往发送寄存器写入数据
  SPI_I2S_SendData(SPI2,Byte);
  
  //如果接受寄存器没有收到数据，循环
  while(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_RXNE) == RESET);
  return SPI_I2S_ReceiveData(SPI2);
}


/************************************************
函数名称 ： Init_SPIBus
功    能 ： 所有SPI初始化
参    数 ： 
返 回 值 ： 
*************************************************/
void Init_SPIBus(void)
{
  SPI1_Config();
  SPI2_Config();
}