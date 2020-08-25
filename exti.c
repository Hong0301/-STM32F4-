#include "exti.h"
#include "mlx90614.h"
u8 flag=1;
u8 on_in=1;
/*
���ţ�PA0����Ϊ�ⲿ�ж�����

*/

//��ʼ��PA0
void Gpio_A_Pin0_Init(void)
{
	GPIO_InitTypeDef GPIOA_InitStruct;
	
	//����GPIOAʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	
	GPIOA_InitStruct.GPIO_Pin  = GPIO_Pin_0;	//�˿�λPA0
	GPIOA_InitStruct.GPIO_Mode = GPIO_Mode_IN;	//����ģʽ
	GPIOA_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;	//����
	
	//��ʼ��
	GPIO_Init(GPIOA, &GPIOA_InitStruct);
	
}

u8 PWR_Check_Standby(void)
{
      u8 t = 0;
      u8 tx = 0;
      while(1)
      {
					if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == 0) //��⵽��������
          {
									t++;
                  tx = 0;
					}
          else
          {
									tx++;
                  if(tx > 3)
                  {
                        
                        return 0;      //���󰴼����߰���ʱ�䲻��
                  }                        
           }
           delay_ms(30);
           if(t > 100)       //100*30ms = 3s
           {
                   return 1;  // 3s
						}
    }
}

//��ʼ���ⲿ�ж�Exti0
void Exti0_Init(void)
{
	EXTI_InitTypeDef EXTI0_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	
	//����ϵͳʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	
	//��ʼ��PA0�˿�λ����
	Gpio_A_Pin0_Init();
	
	//ӳ��PA0ΪEXTI0
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);
	
	EXTI0_InitStruct.EXTI_Line 	  = EXTI_Line0;				//�ⲿ�ж���EXTI0
	EXTI0_InitStruct.EXTI_Mode 	  = EXTI_Mode_Interrupt;	//�ж�����
	EXTI0_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising;	//�����ش���
	EXTI0_InitStruct.EXTI_LineCmd = ENABLE;					//ʹ��
	
	//��ʼ��EXTI0
	EXTI_Init(&EXTI0_InitStruct);
	
	NVIC_InitStruct.NVIC_IRQChannel					  = EXTI0_IRQn;	//ѡ���ⲿ�ж���
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x00;		//������ռ���ȼ�
	NVIC_InitStruct.NVIC_IRQChannelSubPriority 		  = 0x01;		//��Ӧ���ȼ�
	NVIC_InitStruct.NVIC_IRQChannelCmd 				  = ENABLE;		//ʹ��
	
	//��ʼ��NVIC_InitStruct
	NVIC_Init(&NVIC_InitStruct);
	
}


//�ⲿ�ж�Exti0��Ӧ����
void EXTI0_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line0) == SET)	//��ȡEXTI0��״̬
	{

		if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == Bit_RESET)
		{
				delay_ms(15); //��ʱ����
			
				//�ж��Ƿ���
				if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == 0)
				{
					on_in=0;
					//��������,����������
					GPIO_ResetBits(GPIOF, GPIO_Pin_8);		
					// K1 �������������ģʽ
					if(PWR_Check_Standby())  //PWR_Check_Standby()�����������жϰ���ʱ���ģ��������ʱ�䳤����3S�ͷ���1�����򷵻�0
					{
								//ʹ��WKUP���ŵĻ��ѹ���
								PWR_WakeUpPinCmd (ENABLE);

								//�������ģʽ
								PWR_EnterSTANDBYMode();
					}
			
				}	
			//��ձ�־λ
			EXTI_ClearITPendingBit(EXTI_Line0);		
		}		
			
	}
}


/*
���ţ�PE2����Ϊ�ⲿ�ж�����
			Exti2

*/

//��ʼ��PE2
void Gpio_E_Pin2_Init(void)
{
	GPIO_InitTypeDef GPIOE_InitStruct;
	
	//����GPIOEʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	
	GPIOE_InitStruct.GPIO_Pin  = GPIO_Pin_2;	//�˿�λPE2
	GPIOE_InitStruct.GPIO_Mode = GPIO_Mode_IN;	//����ģʽ
	GPIOE_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;	//����
	
	//��ʼ��PE2
	GPIO_Init(GPIOE, &GPIOE_InitStruct);
	
}

//��ʼ���ⲿ�ж�Exti2
void Exti2_Init(void)
{
	EXTI_InitTypeDef EXTI2_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	
	//����ϵͳʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	
	//��ʼ��PE2	
	Gpio_E_Pin2_Init();
	
	//ӳ���ж�Exti2
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource2);
	
	EXTI2_InitStruct.EXTI_Line 	  = EXTI_Line2;				//�ⲿ�ж���EXTI
	EXTI2_InitStruct.EXTI_Mode 	  = EXTI_Mode_Interrupt;	//�ж�����
	EXTI2_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;	//�½��Ӵ���
	EXTI2_InitStruct.EXTI_LineCmd = ENABLE;					//ʹ��
	
	//��ʼ��EXTI2
	EXTI_Init(&EXTI2_InitStruct);
	
	NVIC_InitStruct.NVIC_IRQChannel 					= EXTI2_IRQn;   //ѡ���ⲿ�ж���
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority 	= 0x01;			//������ռ���ȼ�
	NVIC_InitStruct.NVIC_IRQChannelSubPriority 			= 0x01;			//��Ӧ���ȼ�
	NVIC_InitStruct.NVIC_IRQChannelCmd 					= ENABLE;		//ʹ��
	
	//��ʼ��NVIC_InitStruct
	NVIC_Init(&NVIC_InitStruct);
	
}

//�ⲿ�ж�Exti2��Ӧ����
void EXTI2_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line2) == SET)	//��ȡEXTI2��״̬
	{
		
		if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_2) == Bit_RESET)
		{
				delay_ms(15); //��ʱ����
			
				//�ж��Ƿ���
				if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_2) == 0)
				{
								if(flag == 1)
											flag=0;
								else
											flag=1;
								OLED_CLS();//����	
								//�������
								SMBus_Init();									//���⴫����ģ���ʼ��
								//temp=SMBus_ReadTemp();
								char dstr[20];
								sprintf(dstr,"Tmp:%.2f C",SMBus_ReadTemp()); //������ת�ַ���	
								OLED_ShowStr(0,3,(u8 *)dstr,2);
								if(SMBus_ReadTemp() > 37.3)							//����
								{
										on_in=1;
										if(on_in ==1)
										{
												GPIO_SetBits(GPIOF, GPIO_Pin_8); //��������	
										}	
											 	
								}	
								delay_s(3);
								
								OLED_CLS();//����	
														
				}	
			//��ձ�־λ
			EXTI_ClearITPendingBit(EXTI_Line2);		
		}
		
	}
}



/*
���ţ�PE3����Ϊ�ⲿ�ж�����
			Exti3

*/

//��ʼ��PE3
void Gpio_E_Pin3_Init(void)
{
	GPIO_InitTypeDef GPIOE_InitStruct;
	
	//����GPIOEʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	
	GPIOE_InitStruct.GPIO_Pin  = GPIO_Pin_3;		//ѡ��PE3
	GPIOE_InitStruct.GPIO_Mode = GPIO_Mode_IN;		//����Ϊ����
	GPIOE_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;		//����
	
	//��ʼ��GPIOE
	GPIO_Init(GPIOE, &GPIOE_InitStruct);
	
}

//��ʼ���ⲿ�ж�Exti2
void Exti3_Init(void)
{
	EXTI_InitTypeDef EXTI3_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	
	//����ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	
	//��ʼ���˿�PE3
	Gpio_E_Pin3_Init();
	
	//ӳ��˿�EXTI3
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource3);
	
	EXTI3_InitStruct.EXTI_Line    = EXTI_Line3;				//�����ⲿ�ж���
	EXTI3_InitStruct.EXTI_Mode    = EXTI_Mode_Interrupt;	//�ⲿ�ж�
	EXTI3_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;	//�½��ش���
	EXTI3_InitStruct.EXTI_LineCmd = ENABLE;					//ʹ��
	
	//��ʼ��EXTI3
	EXTI_Init(&EXTI3_InitStruct);
	
	NVIC_InitStruct.NVIC_IRQChannel 					= EXTI3_IRQn;	//ѡ���ⲿ�ж���
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority 	= 0x01;			//������ռ���ȼ�
	NVIC_InitStruct.NVIC_IRQChannelSubPriority 			= 0x01;			//��Ӧ���ȼ�
	NVIC_InitStruct.NVIC_IRQChannelCmd 					= ENABLE;		//ʹ��
	
	//��ʼ����ʼ��NVIC_InitStruct
	NVIC_Init(&NVIC_InitStruct);
	
}

//�ⲿ�ж�Exti0��Ӧ����
void EXTI3_IRQHandler(void)
{
	char RTC_time[128];
	if(EXTI_GetITStatus(EXTI_Line3) == SET)	//��ȡEXTI3��״̬
	{
		
		if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_3) == Bit_RESET)
		{
				delay_ms(15); //��ʱ����
			
				//�ж��Ƿ���
				if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_3) == 0)
				{
						if(flag==1)
						{	
								//�������� Сʱ++
								RTC_TimeTypeDef RTC_TimeStruct;
								RTC_GetTime(RTC_Format_BIN, &RTC_TimeStruct);
								RTC_TimeStruct.RTC_Hours++;
								if(RTC_TimeStruct.RTC_Hours == 24)
												RTC_TimeStruct.RTC_Hours=0;
								RTC_SetTime(RTC_Format_BIN, &RTC_TimeStruct);
						}
						if(flag==0)
						{	
								//�������� Сʱ--
								RTC_TimeTypeDef RTC_TimeStruct;
								RTC_GetTime(RTC_Format_BIN, &RTC_TimeStruct);
								RTC_TimeStruct.RTC_Hours--;
								if(RTC_TimeStruct.RTC_Hours < 0)
												RTC_TimeStruct.RTC_Hours= 23;
								RTC_SetTime(RTC_Format_BIN, &RTC_TimeStruct);
						}
							
				}	
			//��ձ�־λ
			EXTI_ClearITPendingBit(EXTI_Line3);		
		}		
		
	}
}



/*
���ţ�PE4����Ϊ�ⲿ�ж�����
			Exti4

*/

//��ʼ��PE4
void Gpio_E_Pin4_Init(void)
{
	GPIO_InitTypeDef GPIOE_InitStruct;
	
	//����GPIOEʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	
	GPIOE_InitStruct.GPIO_Pin  = GPIO_Pin_4;	//ѡ��PE4�˿�
	GPIOE_InitStruct.GPIO_Mode = GPIO_Mode_IN;	//����ģʽ
	GPIOE_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;	//����
	
	//��ʼ��GPIOE
	GPIO_Init(GPIOE, &GPIOE_InitStruct);
	
}

//��ʼ���ⲿ�ж�Exti2
void Exti4_Init(void)
{
	EXTI_InitTypeDef EXTI4_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	
	//����ϵͳʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	
	//��ʼ��PE4
	Gpio_E_Pin4_Init();
	
	//ӳ��˿�EXTI4
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource4);
	
	EXTI4_InitStruct.EXTI_Line 	  = EXTI_Line4;				//�����ⲿ�ж���
	EXTI4_InitStruct.EXTI_Mode 	  = EXTI_Mode_Interrupt;	//�ⲿ�ж�
	EXTI4_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;	//�½��ش���
	EXTI4_InitStruct.EXTI_LineCmd = ENABLE;					//ʹ��
	
	//��ʼ��EXTI4
	EXTI_Init(&EXTI4_InitStruct);
	
	NVIC_InitStruct.NVIC_IRQChannel 				  	= EXTI4_IRQn;	//ѡ���ⲿ�ж���
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority 	= 0x01;			//������ռ���ȼ�
	NVIC_InitStruct.NVIC_IRQChannelSubPriority 			= 0x01;			//��Ӧ���ȼ�
	NVIC_InitStruct.NVIC_IRQChannelCmd					= ENABLE;		//ʹ��
	
	//��ʼ����ʼ��NVIC_InitStruct
	NVIC_Init(&NVIC_InitStruct);
	
}

//�ⲿ�ж�Exti4��Ӧ����
void EXTI4_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line4) == SET)	//��ȡEXTI4��״̬
	{
		
		if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_4) == Bit_RESET)
		{
				delay_ms(15); //��ʱ����
			
				//�ж��Ƿ���
				if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_4) == 0)
				{
						if(flag==1)
						{
									//��������  ��++
						RTC_TimeTypeDef RTC_TimeStruct;
						RTC_GetTime(RTC_Format_BIN, &RTC_TimeStruct);
						RTC_TimeStruct.RTC_Minutes++;
						if(RTC_TimeStruct.RTC_Minutes ==60)
									RTC_TimeStruct.RTC_Minutes=0;
						RTC_SetTime(RTC_Format_BIN, &RTC_TimeStruct);
						}
						if(flag==0)
						{
									//��������  ��--
						RTC_TimeTypeDef RTC_TimeStruct;
						RTC_GetTime(RTC_Format_BIN, &RTC_TimeStruct);
						RTC_TimeStruct.RTC_Minutes--;
						if(RTC_TimeStruct.RTC_Minutes < 0)
									RTC_TimeStruct.RTC_Minutes=59;
						RTC_SetTime(RTC_Format_BIN, &RTC_TimeStruct);
						}
					
				}	
			//��ձ�־λ
			EXTI_ClearITPendingBit(EXTI_Line4);		
		}		
		
	}
}