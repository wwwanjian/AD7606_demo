/**
  ********************************  STM32F10x  *********************************
  * @�ļ���     �� bsp.c
  * @����       �� WJ
  * @��汾     �� V3.5.0
  * @�ļ��汾   �� V1.0.0
  * @����       �� 2018��8��18��
  * @ժҪ       �� BSP�弶֧�ְ�Դ�ļ�
  ******************************************************************************/
/*----------------------------------------------------------------------------
  ������־:
  2018-8-18 V1.0.0:��ʼ�汾
  ----------------------------------------------------------------------------*/
/* ������ͷ�ļ� --------------------------------------------------------------*/
#include "bsp.h"


/************************************************
�������� �� RCC_Configuration
��    �� �� ʱ������
��    �� �� ��
�� �� ֵ �� ��
*************************************************/
void RCC_Configuration(void)
{
  
  RCC_DeInit();  
  RCC_HSEConfig(RCC_HSE_ON);   //���ⲿʱ��HSE    8MHz
  if(RCC_WaitForHSEStartUp() == SUCCESS)    //�ȴ�HSE�����ɹ�
  {  
    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);  //ѡ��PLL��ʱ��Դ��9�� 72Mhz
    RCC_PLLCmd(ENABLE);   //ʹ��PLLʱ��                                  
    while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);     //�ȴ�PLLʱ�Ӿ���
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK); //ϵͳʱ��ѡ��PLL��Ϊʱ��Դ    
    while(RCC_GetSYSCLKSource()!=0x08);   //�жϷ��ص�ʱ��Դ�Ƿ�ΪPLLʱ��Դ
    RCC_HCLKConfig(RCC_SYSCLK_Div1); //PLL����Ƶ��ϵͳʱ��   72MHz
  }  
  /* ʹ��APB2ʱ�� */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB  | 
                         RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD  | 
                         RCC_APB2Periph_GPIOE | RCC_APB2Periph_AFIO , ENABLE);
}

/************************************************
�������� �� GPIO_Basic_Configuration
��    �� �� ������������������ã�LED��
��    �� �� ��
�� �� ֵ �� ��
*************************************************/
void GPIO_Basic_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  GPIO_InitStructure.GPIO_Pin =  PIN_LED;                            //����
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                  //Ƶ��(50M)
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;                   //�������(����ʽ���)
  GPIO_Init(PORT_LED, &GPIO_InitStructure);
}

/************************************************
�������� �� BSP_Initializes
��    �� �� �ײ��ʼ��
��    �� �� ��
�� �� ֵ �� ��
*************************************************/
void BSP_Initializes(void)
{
  RCC_Configuration();                                               //ʱ������
  GPIO_Basic_Configuration();                                        //IO����
}


/**** Copyright (C)2018 WJ. All Rights Reserved **** END OF FILE ****/
