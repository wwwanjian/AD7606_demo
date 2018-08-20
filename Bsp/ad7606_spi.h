/**
  ********************************  STM32F10x  *********************************
  * @文件名     ： ad7606_spi.h
  * @作者       ： WJ
  * @库版本     ： V3.5.0
  * @文件版本   ： V1.0.0
  * @日期       ： 2018年8月18日
  * @摘要       ： AD7606 SPI驱动头文件
  ******************************************************************************/

/* 定义防止递归包含 ----------------------------------------------------------*/
#ifndef _AD7606_SPI_H
#define _AD7606_SPI_H

/* SPI总线的CS、SCK、MOSI、MISO 在 spi.c中配置  */

//读取通道数
#define CH_NUM  3

/* RESET 复位引脚*/
#define PORT_RESET	GPIOA
#define PIN_RESET	GPIO_Pin_1
#define AD_RESET_LOW()					GPIO_ResetBits(PORT_RESET, PIN_RESET)
#define AD_RESET_HIGH()					GPIO_SetBits(PORT_RESET, PIN_RESET)


/* RANGE 设置量程*/		
#define PORT_RANGE 	GPIOA
#define PIN_RANGE 	GPIO_Pin_0
#define AD_RANGE_5V()					GPIO_ResetBits(PORT_RANGE, PIN_RANGE)
#define AD_RANGE_10V()					GPIO_SetBits(PORT_RANGE, PIN_RANGE)	//置1


/* CONVST 起始信号*/		
#define PORT_CONVST	GPIOA
#define PIN_CONVST 	GPIO_Pin_2
#define	AD_CONVST_LOW()					GPIO_ResetBits(PORT_CONVST, PIN_CONVST)
#define	AD_CONVST_HIGH()				GPIO_SetBits(PORT_CONVST, PIN_CONVST)

/* BUSY */
#define PORT_BUSY 	GPIOB
#define PIN_BUSY 	GPIO_Pin_3
#define BUSY_IS_LOW()				(GPIO_ReadInputDataBit(PORT_BUSY, PIN_BUSY) == Bit_RESET)

/* OS 过采样设置*/
#define PORT_OS_0 	GPIOC
#define PIN_OS_0	GPIO_Pin_7
#define	AD_OS0_HIGH()				GPIO_SetBits(PORT_OS_0, PIN_OS_0)
#define	AD_OS0_LOW()				GPIO_ResetBits(PORT_OS_0, PIN_OS_0)

#define PORT_OS_1 	GPIOC
#define PIN_OS_1	GPIO_Pin_6
#define	AD_OS1_HIGH()				GPIO_SetBits(PORT_OS_1, PIN_OS_1)
#define	AD_OS1_LOW()				GPIO_ResetBits(PORT_OS_1, PIN_OS_1)

#define PORT_OS_2	GPIOC
#define PIN_OS_2	GPIO_Pin_9
#define	AD_OS2_HIGH()				GPIO_SetBits(PORT_OS_2, PIN_OS_2)
#define	AD_OS2_LOW()				GPIO_ResetBits(PORT_OS_2, PIN_OS_2)

extern int16_t s_adc_now[8];
extern int16_t s_volt[8];
extern char AD_InputRange;

void AD7606_SetInputRange(uint8_t range);
void AD7606_Reset(void);
void AD7606_Init(void);
void AD7606_StartConv(void);
void AD7606_Scan(void);
void AD7606_IT_Scan(void);
void AD7606_Mak(void);
void EXTI_Config(void);

#endif