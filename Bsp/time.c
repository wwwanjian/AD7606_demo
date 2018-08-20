/**
********************************  STM32F10x  *********************************
* @文件名     ： time.c
* @作者       ： WJ
* @库版本     ： V3.5.0
* @文件版本   ： V1.0.0
* @日期       ： 2018年8月18日
* @摘要       ： 定时器
******************************************************************************/

/* 包含的头文件 --------------------------------------------------------------*/
#include "common.h"



static u8  fac_us=0;//us延时倍乘数
static u16 fac_ms=0;//ms延时倍乘数

/************************************************
函数名称 ： delay_init
功    能 ： SYSTICK延时初始化
参    数 ： 无
返 回 值 ： 无
*************************************************/
void delay_init()	 
{
  SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);	
  fac_us=SystemCoreClock/8000000;
  fac_ms=(u16)fac_us*1000;
}								    

/************************************************
函数名称 ： delay_us
功    能 ： 延时
参    数 ： nus:u32 延时us
返 回 值 ： 无
*************************************************/
void delay_us(uint32_t nus)
{		
  u32 temp;	    	 
  SysTick->LOAD=nus*fac_us; 
  SysTick->VAL=0x00;        
  SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk;
  do
  {
    temp=SysTick->CTRL;
  }
  while(temp&0x01&&!(temp&(1<<16)));
  SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;
  SysTick->VAL =0X00;
}


/************************************************
函数名称 ： delay_ms
功    能 ： 延时
参    数 ： nms:u32 延时ms
返 回 值 ： 无
*************************************************/
void delay_ms(u16 nms)
{	 		  	  
  u32 temp;		   
  SysTick->LOAD=(u32)nms*fac_ms;
  SysTick->VAL =0x00;       
  SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk;
  do
  {
    temp=SysTick->CTRL;
  }
  while(temp&0x01&&!(temp&(1<<16)));
  SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;
  SysTick->VAL =0X00;
} 


/************************************************
函数名称 ： TIM3_TIM_config
功    能 ： TIM3计时设置
参    数 ： arr:u32 周期
返 回 值 ： 无
*************************************************/
static void TIM3_TIM_config(u32 arr)
{
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
  //定时器 TIM3 初始化
  TIM_TimeBaseStructure.TIM_Period = arr; //设置自动重装载寄存器周期的值
  TIM_TimeBaseStructure.TIM_Prescaler =7199; //设置时钟频率除数的预分频值
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM 向上计数
  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //初始化 TIM3
  TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //允许更新中断
  TIM_Cmd(TIM3, ENABLE); //使能 TIM3
}

/************************************************
函数名称 ： TIM3_NVIC_Config
功    能 ： TIM3中断设置
参    数 ： 
返 回 值 ： 
*************************************************/
static void TIM3_NVIC_Config(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  //中断优先级 NVIC 设置
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
  NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn; //TIM3 中断
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //先占优先级 0 级
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3; //从优先级 3 级
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ 通道被使能
  NVIC_Init(&NVIC_InitStructure); //④初始化 NVIC 寄存器
}

/************************************************
函数名称 ： TIM3_Config
功    能 ： TIM3初始化
参    数 ： 
返 回 值 ： 
*************************************************/
void TIM3_Config(u16 ms)
{
  TIM3_TIM_config(ms*10-1);
  TIM3_NVIC_Config();
}

/************************************************
函数名称 ： TIM2_GPIO_Config
功    能 ： TIM2 PWM输出管脚配置
参    数 ： 
返 回 值 ： 
*************************************************/
static void TIM2_GPIO_Config(void)
{
  //PA2 TIM2CH３
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  
  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
}

/************************************************
函数名称 ： TIM2_NVIC_Config
功    能 ： TIM2 PWM输出配置
参    数 ： fre:u32 频率 duty:u16 0~999,占空比
返 回 值 ： 
*************************************************/
static void TIM2_NVIC_Config(u32 fre, u16 duty)
{
  uint16_t TimerPeriod = 0;
  uint16_t ChannelPulse = 0;
  
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  TIM_OCInitTypeDef  TIM_OCInitStructure;
  
  /* Compute the value to be set in ARR regiter to generate signal frequency at 17.57 Khz */
  TimerPeriod = (SystemCoreClock / fre ) - 1;
  /* Compute CCR1 value to generate a duty cycle at 50% for channel 1 and 1N */
  ChannelPulse = (uint16_t) (((uint32_t) duty * (TimerPeriod - 1)) / 1000);
/* Time Base configuration */
  TIM_TimeBaseStructure.TIM_Prescaler = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseStructure.TIM_Period = TimerPeriod;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;

  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

  /* Channel Configuration in PWM mode */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
  TIM_OCInitStructure.TIM_Pulse = ChannelPulse;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
  TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
  TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
  TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;
  TIM_OC3Init(TIM2, &TIM_OCInitStructure);

  /* TIM2 counter enable */
  TIM_Cmd(TIM2, ENABLE);

  /* TIM2 Main Output Enable */
  TIM_CtrlPWMOutputs(TIM2, ENABLE);
}

/************************************************
函数名称 ： TIM2_Config
功    能 ： TIM2 PWM输出初始化
参    数 ： fre:u32 频率 duty:u16 0~99,占空比
返 回 值 ： 
*************************************************/
void TIM2_Config(u32 fre, u16 duty)
{
  TIM2_GPIO_Config();
  TIM2_NVIC_Config(fre, duty*10);
}