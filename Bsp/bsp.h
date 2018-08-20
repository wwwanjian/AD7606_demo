/**
  ********************************  STM32F10x  *********************************
  * @文件名     ： bsp.c
  * @作者       ： strongerHuang
  * @库版本     ： V3.5.0
  * @文件版本   ： V1.0.0
  * @日期       ： 2018年8月18日
  * @摘要       ： BSP板级支持包头文件
  ******************************************************************************/

/* 定义防止递归包含 ----------------------------------------------------------*/
#ifndef _BSP_H
#define _BSP_H

/* 包含的头文件 --------------------------------------------------------------*/
#include "stm32f10x.h"


/* 宏定义 --------------------------------------------------------------------*/
#define PORT_LED                  GPIOC
#define PIN_LED                   GPIO_Pin_13

/* LED开关 */
#define LED_ON                    GPIO_ResetBits(PORT_LED, PIN_LED)
#define LED_OFF                   GPIO_SetBits(PORT_LED, PIN_LED)
#define LED_TOGGLE                (PORT_LED->ODR ^= PIN_LED)


/* 函数申明 ------------------------------------------------------------------*/
void BSP_Initializes(void);


#endif /* _BSP_H */

/**** Copyright (C)2016 strongerHuang. All Rights Reserved **** END OF FILE ****/
