/**
********************************  STM32F10x  *********************************
* @�ļ���     �� ad7606_spi.c
* @����       �� WJ
* @��汾     �� V3.5.0
* @�ļ��汾   �� V1.0.0
* @����       �� 2018��8��18��
* @ժҪ       �� AD7606 SPI����
******************************************************************************/



/* ������ͷ�ļ� --------------------------------------------------------------*/
#include "common.h"

int16_t s_adc_now[8];   //ԭʼ��ֵ
int16_t s_volt[8];      //ת����ĵ�ѹֵ
char AD_InputRange=0;   //�������̣�0��-5V-+5V  1:-10V-+10V


/************************************************
�������� �� AD7606_GPIO_Config
��    �� �� ad7606�ܽų�ʼ��
��    �� �� ��
�� �� ֵ �� ��
*************************************************/
static void AD7606_GPIO_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  /* ���ü����������IO */
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	/* �������ģʽ */
  
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
  
  /* ����GPIOΪ��������ģʽ(ʵ����CPU��λ���������״̬) */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;		/* MISO ����Ϊ�������� */
  
  GPIO_InitStructure.GPIO_Pin = PIN_BUSY;
  GPIO_Init(PORT_BUSY, &GPIO_InitStructure);
  
  
}


/************************************************
�������� �� AD7606_SetInputRange
��    �� �� ����AD7606ģ���ź���������
��    �� �� _ucRange : 0 ��ʾ����5V   1��ʾ����10V
�� �� ֵ �� ��
*************************************************/
void AD7606_SetInputRange(uint8_t range)
{
  if (range == 0)
  {
    AD_RANGE_5V();	/* ����Ϊ����5V */
  }
  else
  {
    AD_RANGE_10V();	/* ����Ϊ����10V */
  }
}

/************************************************
�������� �� AD7606_Reset
��    �� �� Ӳ����λ
��    �� �� 
�� �� ֵ �� 
*************************************************/
void AD7606_Reset(void)
{ 
  /* AD7606�Ǹߵ�ƽ��λ��Ҫ����С����50ns */
  AD_RESET_LOW();
  
  AD_RESET_HIGH();
  AD_RESET_HIGH();
  AD_RESET_HIGH();
  AD_RESET_HIGH();
  
  AD_RESET_LOW();
}


/************************************************
�������� �� AD7606_Init
��    �� �� ��ʼ��AD7606
��    �� �� 
�� �� ֵ �� 
*************************************************/
void AD7606_Init(void)
{
  AD7606_GPIO_Config();		/* ����GPIO */
  
  AD7606_SetInputRange(AD_InputRange);	/* ���ò������� */
  
  AD_OS0_LOW();
  AD_OS1_LOW();
  AD_OS2_LOW();
  
  AD7606_Reset();				/* Ӳ����λ��AD7606 */
  
  AD_CONVST_HIGH();			/* CONVST������Ϊ�ߵ�ƽ */	
}


/************************************************
�������� �� ad7606_StartConv
��    �� �� ����AD7606��ADCת��
��    �� �� 
�� �� ֵ �� 
*************************************************/
void AD7606_StartConv(void)
{
  /* �����ؿ�ʼת�����͵�ƽ����ʱ������25ns  */
  AD_CONVST_LOW();
  AD_CONVST_LOW();
  AD_CONVST_LOW();	/* ����ִ��2�Σ��͵�ƽԼ50ns */
  
  AD_CONVST_HIGH();
}


/************************************************
�������� �� AD7606_scan
��    �� �� ɨ����ñ����������ڶ�ȡADת��������
��    �� �� 
�� �� ֵ �� 
*************************************************/
void AD7606_Scan(void) 		/* �˺������밴��ʱ���д */
{
  uint8_t i;			
  
  /* BUSY = 0 ʱ.ad7606���ڿ���״̬adת������ */	
  if (BUSY_IS_LOW())	   
  {
    CS_1_LOW; /* SPIƬѡ = 0 */
    
    for (i = 0; i < CH_NUM; i++)
    {
      s_adc_now[i] = SPI1_Send_Receive_Byte(0x00);
      
      s_adc_now[i] = s_adc_now[i]*256 + SPI1_Send_Receive_Byte(0x00); /* ������ */	
    }
    
    CS_1_HIGH; /* SPIƬѡ = 1 */
    
    AD7606_StartConv();	/* ����ʼ�ź� */	
  }
}

/************************************************
�������� �� AD7606_IT_Scan
��    �� �� ɨ����ñ��������������ⲿ�ж��ж�ȡADת��������
��    �� �� 
�� �� ֵ �� 
*************************************************/
void AD7606_IT_Scan(void) 		/* �˺������밴��ʱ���д */
{
  uint8_t i;			
  
  /* BUSY = 0 ʱ.ad7606���ڿ���״̬adת������ */	
  CS_1_LOW; /* SPIƬѡ = 0 */
  
  for (i = 0; i < CH_NUM; i++)
  {
    s_adc_now[i] = SPI1_Send_Receive_Byte(0x00);
    
    s_adc_now[i] = s_adc_now[i]*256 + SPI1_Send_Receive_Byte(0x00); /* ������ */	
  }
  CS_1_HIGH; /* SPIƬѡ = 1 */	
}


/************************************************
�������� �� AD7606_Mak
��    �� �� ��������������
��    �� �� 
�� �� ֵ �� 
*************************************************/
void AD7606_Mak(void)
{
  uint8_t i;
  int16_t adc;
  
  for (i = 0;i < CH_NUM; i++)
  {
    /* 
    32767 = 5V , ��������ֵ��ʵ�ʿ��Ը���5V��׼��ʵ��ֵ���й�ʽ���� 
    volt[i] = ((int16_t)dat[i] * 5000) / 32767;	����ʵ�ʵ�ѹֵ�����ƹ���ģ�������׼ȷ�������У׼            
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
�������� �� EXTI_GPIO_Config
��    �� �� �ⲿ�жϹܽ�����
��    �� �� 
�� �� ֵ �� 
*************************************************/
static void EXTI_GPIO_Config(void)
{
  //����PB3Ϊ�ⲿ�ж�
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);//��GPIO AFIO��ʱ��
  GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(PORT_BUSY, &GPIO_InitStructure);
}

/************************************************
�������� �� EXTI_NVIC_Config
��    �� �� �ⲿ�ж��ж�����
��    �� �� 
�� �� ֵ �� 
*************************************************/
static void EXTI_NVIC_Config(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  EXTI_InitTypeDef EXTI_InitStructure;
  EXTI_ClearITPendingBit(EXTI_Line3);
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource3);//PB3 
  EXTI_InitStructure.EXTI_Line= EXTI_Line3; //PB3 
  EXTI_InitStructure.EXTI_Mode= EXTI_Mode_Interrupt; 
  EXTI_InitStructure.EXTI_Trigger= EXTI_Trigger_Falling;  //�жϷ�ʽΪ�½���
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
�������� �� EXTI_Config
��    �� �� �ⲿ�ж�����
��    �� �� 
�� �� ֵ �� 
*************************************************/
void EXTI_Config(void)
{
    EXTI_GPIO_Config();
    EXTI_NVIC_Config();
}