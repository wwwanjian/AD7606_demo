/**
  ********************************  STM32F10x  *********************************
  * @文件名     ： bsp.c
  * @作者       ： WJ
  * @库版本     ： V3.5.0
  * @文件版本   ： V1.0.0
  * @日期       ： 2018年8月18日
  * @摘要       ： BSP板级支持包源文件
  ******************************************************************************/
/*----------------------------------------------------------------------------
  更新日志:
  2018-8-18 V1.0.0:初始版本
  ----------------------------------------------------------------------------*/
/* 包含的头文件 --------------------------------------------------------------*/
#include "bsp.h"


/************************************************
函数名称 ： RCC_Configuration
功    能 ： 时钟配置
参    数 ： 无
返 回 值 ： 无
*************************************************/
void RCC_Configuration(void)
{
  
  RCC_DeInit();  
  RCC_HSEConfig(RCC_HSE_ON);   //打开外部时钟HSE    8MHz
  if(RCC_WaitForHSEStartUp() == SUCCESS)    //等待HSE启动成功
  {  
    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);  //选择PLL的时钟源并9倍 72Mhz
    RCC_PLLCmd(ENABLE);   //使能PLL时钟                                  
    while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);     //等待PLL时钟就绪
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK); //系统时钟选择PLL作为时钟源    
    while(RCC_GetSYSCLKSource()!=0x08);   //判断返回的时钟源是否为PLL时钟源
    RCC_HCLKConfig(RCC_SYSCLK_Div1); //PLL不分频给系统时钟   72MHz
  }  
  /* 使能APB2时钟 */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB  | 
                         RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD  | 
                         RCC_APB2Periph_GPIOE | RCC_APB2Periph_AFIO , ENABLE);
}

/************************************************
函数名称 ： GPIO_Basic_Configuration
功    能 ： 基本输入输出引脚配置（LED）
参    数 ： 无
返 回 值 ： 无
*************************************************/
void GPIO_Basic_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  GPIO_InitStructure.GPIO_Pin =  PIN_LED;                            //引脚
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                  //频率(50M)
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;                   //输出类型(推挽式输出)
  GPIO_Init(PORT_LED, &GPIO_InitStructure);
}

/************************************************
函数名称 ： BSP_Initializes
功    能 ： 底层初始化
参    数 ： 无
返 回 值 ： 无
*************************************************/
void BSP_Initializes(void)
{
  RCC_Configuration();                                               //时钟配置
  GPIO_Basic_Configuration();                                        //IO配置
}


/**** Copyright (C)2018 WJ. All Rights Reserved **** END OF FILE ****/
