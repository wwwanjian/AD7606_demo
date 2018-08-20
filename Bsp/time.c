/**
********************************  STM32F10x  *********************************
* @�ļ���     �� time.c
* @����       �� WJ
* @��汾     �� V3.5.0
* @�ļ��汾   �� V1.0.0
* @����       �� 2018��8��18��
* @ժҪ       �� ��ʱ��
******************************************************************************/

/* ������ͷ�ļ� --------------------------------------------------------------*/
#include "common.h"



static u8  fac_us=0;//us��ʱ������
static u16 fac_ms=0;//ms��ʱ������

/************************************************
�������� �� delay_init
��    �� �� SYSTICK��ʱ��ʼ��
��    �� �� ��
�� �� ֵ �� ��
*************************************************/
void delay_init()	 
{
  SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);	
  fac_us=SystemCoreClock/8000000;
  fac_ms=(u16)fac_us*1000;
}								    

/************************************************
�������� �� delay_us
��    �� �� ��ʱ
��    �� �� nus:u32 ��ʱus
�� �� ֵ �� ��
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
�������� �� delay_ms
��    �� �� ��ʱ
��    �� �� nms:u32 ��ʱms
�� �� ֵ �� ��
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
�������� �� TIM3_TIM_config
��    �� �� TIM3��ʱ����
��    �� �� arr:u32 ����
�� �� ֵ �� ��
*************************************************/
static void TIM3_TIM_config(u32 arr)
{
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
  //��ʱ�� TIM3 ��ʼ��
  TIM_TimeBaseStructure.TIM_Period = arr; //�����Զ���װ�ؼĴ������ڵ�ֵ
  TIM_TimeBaseStructure.TIM_Prescaler =7199; //����ʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM ���ϼ���
  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //��ʼ�� TIM3
  TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //��������ж�
  TIM_Cmd(TIM3, ENABLE); //ʹ�� TIM3
}

/************************************************
�������� �� TIM3_NVIC_Config
��    �� �� TIM3�ж�����
��    �� �� 
�� �� ֵ �� 
*************************************************/
static void TIM3_NVIC_Config(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  //�ж����ȼ� NVIC ����
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
  NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn; //TIM3 �ж�
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //��ռ���ȼ� 0 ��
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3; //�����ȼ� 3 ��
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ ͨ����ʹ��
  NVIC_Init(&NVIC_InitStructure); //�ܳ�ʼ�� NVIC �Ĵ���
}

/************************************************
�������� �� TIM3_Config
��    �� �� TIM3��ʼ��
��    �� �� 
�� �� ֵ �� 
*************************************************/
void TIM3_Config(u16 ms)
{
  TIM3_TIM_config(ms*10-1);
  TIM3_NVIC_Config();
}

/************************************************
�������� �� TIM2_GPIO_Config
��    �� �� TIM2 PWM����ܽ�����
��    �� �� 
�� �� ֵ �� 
*************************************************/
static void TIM2_GPIO_Config(void)
{
  //PA2 TIM2CH��
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  
  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
}

/************************************************
�������� �� TIM2_NVIC_Config
��    �� �� TIM2 PWM�������
��    �� �� fre:u32 Ƶ�� duty:u16 0~999,ռ�ձ�
�� �� ֵ �� 
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
�������� �� TIM2_Config
��    �� �� TIM2 PWM�����ʼ��
��    �� �� fre:u32 Ƶ�� duty:u16 0~99,ռ�ձ�
�� �� ֵ �� 
*************************************************/
void TIM2_Config(u32 fre, u16 duty)
{
  TIM2_GPIO_Config();
  TIM2_NVIC_Config(fre, duty*10);
}