#include "pwm.h"

/*
����˵����

LED0������PF9,TIM14ͨ��1

TIM14 -- APB1,��ʱ��Ƶ�ʣ�84MHZ


*/

void Pwm_Led0_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStruct;
	
	TIM_OCInitTypeDef  TIM_OCInitStruct; 	//��ʱһ���ýṹ��
	
	
	//���� TIM14 �� �� GPIOʱ�ӣ����� PF9ѡ���ù��� AF9 ��TIM14�������
	//�⺯��ʹ�� TIM14 ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14,ENABLE);
	//����Ҫʹ�õ�GPIOF9������ҲҪʹ�ܸ����� 
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE); 
	
	
	//����ʹ�õ�PF9�ĸ��ù��ܣ����ÿ⺯��ʹ��������TIM14��ʱ��
	GPIO_PinAFConfig(GPIOF,GPIO_PinSource9,GPIO_AF_TIM14); 
	
	
	GPIO_InitStruct.GPIO_Pin 	= GPIO_Pin_9; 		//GPIOF9
	GPIO_InitStruct.GPIO_Mode 	= GPIO_Mode_AF; 	//��������Ϊ���ù���
	GPIO_InitStruct.GPIO_Speed 	= GPIO_Speed_50MHz; //�ٶ� 50MHz
	GPIO_InitStruct.GPIO_OType 	= GPIO_OType_PP; 	//���츴�����
	GPIO_InitStruct.GPIO_PuPd 	= GPIO_PuPd_UP; 	//����
	GPIO_Init(GPIOF,&GPIO_InitStruct); 	//��ʼ�� PF9���ÿ⺯���Ĵ���


	TIM_TimeBaseStruct.TIM_Prescaler	= 84-1 ; 	//���ö�ʱ���ķ�Ƶֵ 1MHZ
	TIM_TimeBaseStruct.TIM_Period		= 500-1; 	//���ö�ʱ���ĵ��Զ���װ�ص�ֵ ����Ϊ500us
	TIM_TimeBaseStruct.TIM_CounterMode	= TIM_CounterMode_Up;//���ö�ʱ��Ϊ���ϼ���ģʽ
	TIM_TimeBaseStruct.TIM_ClockDivision= TIM_CKD_DIV1;//���ö�ʱ����ʱ�ӷ�Ƶ����
	TIM_TimeBaseInit(TIM14,&TIM_TimeBaseStruct);//��ʼ����ʱ�� 14
	

	
	TIM_OCInitStruct.TIM_OCMode			= TIM_OCMode_PWM1; 		//ѡ��PWM��ģʽ��ѡ��PWMģʽ1
	TIM_OCInitStruct.TIM_OCPolarity 	= TIM_OCPolarity_Low; 	//����ļ��ԣ�����Ǹߵ�ƽ���ǵ͵�ƽ������ѡ��͵�ƽ
	TIM_OCInitStruct.TIM_OutputState 	= TIM_OutputState_Enable; //�������������ʹ�ܣ�ʹ��PWM������˿�
	//�����趨��Ϣ����TIM14 OC1 -- ͨ��1
	TIM_OC1Init(TIM14, &TIM_OCInitStruct); 
	
	//ʹ��Ԥװ�ؼĴ�����
	TIM_OC1PreloadConfig(TIM14, TIM_OCPreload_Enable); 
	//ʹ���Զ���װ�ص�Ԥװ�ؼĴ�������λ	
	TIM_ARRPreloadConfig(TIM14,ENABLE);

	//ʹ�ܶ�ʱ����
	TIM_Cmd(TIM14, ENABLE);

}

/*
����˵����

��������ź�����PB10,TIM2ͨ��3
PB11 TIM2ͨ��4
TIM2 -- APB1,��ʱ��Ƶ�ʣ�84MHZ
TIM_SetCompare3(TIM2, 0);
TIM_SetCompare4(TIM2, 0);

*/
void Pwm_Motor_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;         	  //GPIO��һ���ṹ��
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStruct;   //��ʱ���ṹ��
		
	TIM_OCInitTypeDef  TIM_OCInitStruct; 					//��ʱһ���ýṹ��
	
	
	//���� TIM4 �� �� GPIOʱ�ӣ����� PF9ѡ���ù��� AF9 ��TIM14�������
	//�⺯��ʹ�� TIM4 ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	//����Ҫʹ�õ�GPIOD13������ҲҪʹ�ܸ����� 
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); 
	
	
	//����ʹ�õ�PF9�ĸ��ù��ܣ����ÿ⺯��ʹ��������TIM4��ʱ��
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_TIM2); 
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource11,GPIO_AF_TIM2);
	
	GPIO_InitStruct.GPIO_Pin 	= GPIO_Pin_10 | GPIO_Pin_11; 		  //����ΪGPIOD13����
	GPIO_InitStruct.GPIO_Mode 	= GPIO_Mode_AF; 	//��������Ϊ���ù���
	GPIO_InitStruct.GPIO_Speed 	= GPIO_Speed_50MHz; //�ٶ� 50MHz
	GPIO_InitStruct.GPIO_OType 	= GPIO_OType_PP; 	//���츴�����
	GPIO_InitStruct.GPIO_PuPd 	= GPIO_PuPd_UP; 	//����
	GPIO_Init(GPIOB,&GPIO_InitStruct); 	          //��ʼ�� PF9���ÿ⺯���Ĵ���


	TIM_TimeBaseStruct.TIM_Prescaler	= 84-1 ; 		//���ö�ʱ���ķ�Ƶֵ 1MHZ
	TIM_TimeBaseStruct.TIM_Period		= 500-1; 	  	//���ö�ʱ���ĵ��Զ���װ�ص�ֵ ����Ϊ500us
	TIM_TimeBaseStruct.TIM_CounterMode	= TIM_CounterMode_Up;//���ö�ʱ��Ϊ���ϼ���ģʽ
	TIM_TimeBaseStruct.TIM_ClockDivision= TIM_CKD_DIV1;//���ö�ʱ����ʱ�ӷ�Ƶ����
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStruct);	//��ʼ����ʱ�� 4
	

	
	TIM_OCInitStruct.TIM_OCMode			= TIM_OCMode_PWM1; 		//ѡ��PWM��ģʽ��ѡ��PWMģʽ1
	TIM_OCInitStruct.TIM_OCPolarity 	= TIM_OCPolarity_Low; 	//����ļ��ԣ�����Ǹߵ�ƽ���ǵ͵�ƽ������ѡ��͵�ƽ
	TIM_OCInitStruct.TIM_OutputState 	= TIM_OutputState_Enable; //�������������ʹ�ܣ�ʹ��PWM������˿�
	//�����趨��Ϣ����TIM2 OC3 -- ͨ��1
	TIM_OC3Init(TIM2, &TIM_OCInitStruct); 
	TIM_OC4Init(TIM2, &TIM_OCInitStruct); 
	
	//ʹ��Ԥװ�ؼĴ�����
	TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable); 
	TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable); 
	//ʹ���Զ���װ�ص�Ԥװ�ؼĴ�������λ	
	TIM_ARRPreloadConfig(TIM2,ENABLE);

	//ʹ�ܶ�ʱ����
	TIM_Cmd(TIM2, ENABLE);

}

