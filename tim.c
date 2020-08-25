#include "tim.h"

/*
��ʱ��˵��

TIM3 -- APB1
��ʱ��TIM3��84MHZ

16λ��ʱ����ֵ��Χ��0~65535

*/


void Tim3_Init(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseInitStruct;
	
	
	//1���ܶ�ʱ��ʱ�ӡ�
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	
	
	TIM_TimeBaseInitStruct.TIM_Prescaler    = 84-1;    //84��Ƶ 84000 000/84 = 1MHZ  1us
	TIM_TimeBaseInitStruct.TIM_Period		= 50000-1;  //��50000���� ��10000HZ�£���ʱ1s
	TIM_TimeBaseInitStruct.TIM_CounterMode  = TIM_CounterMode_Up; //���ϼ���
	TIM_TimeBaseInitStruct.TIM_ClockDivision= TIM_CKD_DIV1; //��Ƶ����
	//2����ʼ����ʱ��������ARR,PSC��
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStruct);		
	
	//5��ʹ�ܶ�ʱ����
	TIM_Cmd(TIM3, DISABLE);
	
}


//��д�жϷ������������������Ҫ����Ա�����������ã���������CPU���е��õĺ���
void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)==SET) //����ж�
	{
		/*����ĳ����*/
		GPIO_ToggleBits(GPIOE, GPIO_Pin_14);
	}
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update); //����жϱ�־λ
}







