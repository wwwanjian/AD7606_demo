/**
  ********************************  STM32F10x  *********************************
  * @�ļ���     �� bsp.c
  * @����       �� strongerHuang
  * @��汾     �� V3.5.0
  * @�ļ��汾   �� V1.0.0
  * @����       �� 2018��8��18��
  * @ժҪ       �� BSP�弶֧�ְ�ͷ�ļ�
  ******************************************************************************/

/* �����ֹ�ݹ���� ----------------------------------------------------------*/
#ifndef _BSP_H
#define _BSP_H

/* ������ͷ�ļ� --------------------------------------------------------------*/
#include "stm32f10x.h"


/* �궨�� --------------------------------------------------------------------*/
#define PORT_LED                  GPIOC
#define PIN_LED                   GPIO_Pin_13

/* LED���� */
#define LED_ON                    GPIO_ResetBits(PORT_LED, PIN_LED)
#define LED_OFF                   GPIO_SetBits(PORT_LED, PIN_LED)
#define LED_TOGGLE                (PORT_LED->ODR ^= PIN_LED)


/* �������� ------------------------------------------------------------------*/
void BSP_Initializes(void);


#endif /* _BSP_H */

/**** Copyright (C)2016 strongerHuang. All Rights Reserved **** END OF FILE ****/
