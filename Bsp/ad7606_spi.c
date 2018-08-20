/**
********************************  STM32F10x  *********************************
* @文件名     ： ad7606_spi.c
* @作者       ： WJ
* @库版本     ： V3.5.0
* @文件版本   ： V1.0.0
* @日期       ： 2018年8月18日
* @摘要       ： AD7606 SPI驱动
******************************************************************************/



/* 包含的头文件 --------------------------------------------------------------*/
#include "common.h"

int16_t s_adc_now[8];   //原始数值
int16_t s_volt[8];      //转换后的电压值
char AD_InputRange=0;   //设置量程：0：-5V-+5V  1:-10V-+10V


/************************************************
函数名称 ： AD7606_GPIO_Config
功    能 ： ad7606管脚初始化
参    数 ： 无
返 回 值 ： 无
*************************************************/
static void AD7606_GPIO_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  /* 配置几个推完输出IO */
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	/* 推挽输出模式 */
  
  GPIO_InitStructure.GPIO_Pin = PIN_RESET;
  GPIO_Init(PORT_RESET, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = PIN_CONVST;
  GPIO_Init(PORT_CONVST, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = PIN_RANGE;
  GPIO_Init(PORT_RANGE, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = PIN_OS_0;
  GPIO_Init(PORT_OS_0, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = PIN_OS_1;
  GPIO_Init(PORT_OS_1, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = PIN_OS_2;
  GPIO_Init(PORT_OS_2, &GPIO_InitStructure);
  
  /* 配置GPIO为浮动输入模式(实际上CPU复位后就是输入状态) */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;		/* MISO 设置为输入上拉 */
  
  GPIO_InitStructure.GPIO_Pin = PIN_BUSY;
  GPIO_Init(PORT_BUSY, &GPIO_InitStructure);
  
  
}


/************************************************
函数名称 ： AD7606_SetInputRange
功    能 ： 配置AD7606模拟信号输入量程
参    数 ： _ucRange : 0 表示正负5V   1表示正负10V
返 回 值 ： 无
*************************************************/
void AD7606_SetInputRange(uint8_t range)
{
  if (range == 0)
  {
    AD_RANGE_5V();	/* 设置为正负5V */
  }
  else
  {
    AD_RANGE_10V();	/* 设置为正负10V */
  }
}

/************************************************
函数名称 ： AD7606_Reset
功    能 ： 硬件复位
参    数 ： 
返 回 值 ： 
*************************************************/
void AD7606_Reset(void)
{ 
  /* AD7606是高电平复位，要求最小脉宽50ns */
  AD_RESET_LOW();
  
  AD_RESET_HIGH();
  AD_RESET_HIGH();
  AD_RESET_HIGH();
  AD_RESET_HIGH();
  
  AD_RESET_LOW();
}


/************************************************
函数名称 ： AD7606_Init
功    能 ： 初始化AD7606
参    数 ： 
返 回 值 ： 
*************************************************/
void AD7606_Init(void)
{
  AD7606_GPIO_Config();		/* 配置GPIO */
  
  AD7606_SetInputRange(AD_InputRange);	/* 设置采样量程 */
  
  AD_OS0_LOW();
  AD_OS1_LOW();
  AD_OS2_LOW();
  
  AD7606_Reset();				/* 硬件复位复AD7606 */
  
  AD_CONVST_HIGH();			/* CONVST脚设置为高电平 */	
}


/************************************************
函数名称 ： ad7606_StartConv
功    能 ： 启动AD7606的ADC转换
参    数 ： 
返 回 值 ： 
*************************************************/
void AD7606_StartConv(void)
{
  /* 上升沿开始转换，低电平持续时间至少25ns  */
  AD_CONVST_LOW();
  AD_CONVST_LOW();
  AD_CONVST_LOW();	/* 连续执行2次，低电平约50ns */
  
  AD_CONVST_HIGH();
}


/************************************************
函数名称 ： AD7606_scan
功    能 ： 扫描调用本函数，用于读取AD转换器数据
参    数 ： 
返 回 值 ： 
*************************************************/
void AD7606_Scan(void) 		/* 此函数代码按照时序编写 */
{
  uint8_t i;			
  
  /* BUSY = 0 时.ad7606处于空闲状态ad转换结束 */	
  if (BUSY_IS_LOW())	   
  {
    CS_1_LOW; /* SPI片选 = 0 */
    
    for (i = 0; i < CH_NUM; i++)
    {
      s_adc_now[i] = SPI1_Send_Receive_Byte(0x00);
      
      s_adc_now[i] = s_adc_now[i]*256 + SPI1_Send_Receive_Byte(0x00); /* 读数据 */	
    }
    
    CS_1_HIGH; /* SPI片选 = 1 */
    
    AD7606_StartConv();	/* 给开始信号 */	
  }
}

/************************************************
函数名称 ： AD7606_IT_Scan
功    能 ： 扫描调用本函数，用于在外部中断中读取AD转换器数据
参    数 ： 
返 回 值 ： 
*************************************************/
void AD7606_IT_Scan(void) 		/* 此函数代码按照时序编写 */
{
  uint8_t i;			
  
  /* BUSY = 0 时.ad7606处于空闲状态ad转换结束 */	
  CS_1_LOW; /* SPI片选 = 0 */
  
  for (i = 0; i < CH_NUM; i++)
  {
    s_adc_now[i] = SPI1_Send_Receive_Byte(0x00);
    
    s_adc_now[i] = s_adc_now[i]*256 + SPI1_Send_Receive_Byte(0x00); /* 读数据 */	
  }
  CS_1_HIGH; /* SPI片选 = 1 */	
}


/************************************************
函数名称 ： AD7606_Mak
功    能 ： 处理采样后的数据
参    数 ： 
返 回 值 ： 
*************************************************/
void AD7606_Mak(void)
{
  uint8_t i;
  int16_t adc;
  
  for (i = 0;i < CH_NUM; i++)
  {
    /* 
    32767 = 5V , 这是理论值，实际可以根据5V基准的实际值进行公式矫正 
    volt[i] = ((int16_t)dat[i] * 5000) / 32767;	计算实际电压值（近似估算的），如需准确，请进行校准            
    volt[i] = dat[i] * 0.3051850947599719
    */
    
    adc = s_adc_now[i];
    if (AD_InputRange == 0)
    {
      s_volt[i] = (adc * 5000) / 32767;
    }
    else
    {
      s_volt[i] = (adc * 10000) / 32767;
    }
  }
}

/************************************************
函数名称 ： EXTI_GPIO_Config
功    能 ： 外部中断管脚配置
参    数 ： 
返 回 值 ： 
*************************************************/
static void EXTI_GPIO_Config(void)
{
  //配置PB3为外部中断
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);//打开GPIO AFIO的时钟
  GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(PORT_BUSY, &GPIO_InitStructure);
}

/************************************************
函数名称 ： EXTI_NVIC_Config
功    能 ： 外部中断中断配置
参    数 ： 
返 回 值 ： 
*************************************************/
static void EXTI_NVIC_Config(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  EXTI_InitTypeDef EXTI_InitStructure;
  EXTI_ClearITPendingBit(EXTI_Line3);
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource3);//PB3 
  EXTI_InitStructure.EXTI_Line= EXTI_Line3; //PB3 
  EXTI_InitStructure.EXTI_Mode= EXTI_Mode_Interrupt; 
  EXTI_InitStructure.EXTI_Trigger= EXTI_Trigger_Falling;  //中断方式为下降沿
  EXTI_InitStructure.EXTI_LineCmd=ENABLE;
  EXTI_Init(&EXTI_InitStructure);
  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);  //NVIC
  NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority= 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority= 0;        
  NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;  
  NVIC_Init(&NVIC_InitStructure); 
}

/************************************************
函数名称 ： EXTI_Config
功    能 ： 外部中断配置
参    数 ： 
返 回 值 ： 
*************************************************/
void EXTI_Config(void)
{
    EXTI_GPIO_Config();
    EXTI_NVIC_Config();
}