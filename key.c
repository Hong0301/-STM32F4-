#include "key.h"

/*
����˵����

KEY0��S1�� -- PA0

*/


void Key_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;
	
	//ʹ��GPIO A��ʱ�ӣ�
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	
	
	GPIO_InitStruct.GPIO_Pin	= GPIO_Pin_0;		//����0
	GPIO_InitStruct.GPIO_Mode	= GPIO_Mode_IN;		//����ģʽ
	GPIO_InitStruct.GPIO_PuPd	= GPIO_PuPd_UP;     //����
	GPIO_Init(GPIOA, &GPIO_InitStruct);

}

