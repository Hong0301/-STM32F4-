#include "led.h"


/*
����˵����
LED0������PF9����
PF9���VCC(1),����
PF9���GND(0),����
*/
#define LED_Time 400
void Led_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;
	
	//ʹ��GPIO F��ʱ�ӣ�
	//RCC_AHB1ENR |= (0x01<<5);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	
	GPIO_InitStruct.GPIO_Pin	=  GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10;		//���� 8 ,9 10,����8Ϊ������
	GPIO_InitStruct.GPIO_Mode	= GPIO_Mode_OUT;	//���ģʽ
	GPIO_InitStruct.GPIO_OType	= GPIO_OType_PP;	//����
	GPIO_InitStruct.GPIO_Speed	= GPIO_Speed_50MHz; //����
	GPIO_InitStruct.GPIO_PuPd	= GPIO_PuPd_UP;     //����
	GPIO_Init(GPIOF, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin	= GPIO_Pin_13|GPIO_Pin_14;		//����13 14
	GPIO_InitStruct.GPIO_Mode	= GPIO_Mode_OUT;	//���ģʽ
	GPIO_InitStruct.GPIO_OType	= GPIO_OType_PP;	//����
	GPIO_InitStruct.GPIO_Speed	= GPIO_Speed_50MHz; //����
	GPIO_InitStruct.GPIO_PuPd	= GPIO_PuPd_UP;     //����
	GPIO_Init(GPIOE, &GPIO_InitStruct);
	
	GPIO_SetBits(GPIOF, GPIO_Pin_9);
	GPIO_SetBits(GPIOF, GPIO_Pin_10);
	GPIO_SetBits(GPIOE, GPIO_Pin_13);
	GPIO_SetBits(GPIOE, GPIO_Pin_14);
}

void PA2_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	
	GPIO_InitStruct.GPIO_Pin	= GPIO_Pin_2;		//����2
	GPIO_InitStruct.GPIO_Mode	= GPIO_Mode_OUT;	//���ģʽ
	GPIO_InitStruct.GPIO_OType	= GPIO_OType_PP;	//����
	GPIO_InitStruct.GPIO_Speed	= GPIO_Speed_50MHz; //����
	GPIO_InitStruct.GPIO_PuPd	= GPIO_PuPd_UP;     //����
	GPIO_Init(GPIOA, &GPIO_InitStruct);
}

void PA3_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	
	GPIO_InitStruct.GPIO_Pin	= GPIO_Pin_3;		//����0
	GPIO_InitStruct.GPIO_Mode	= GPIO_Mode_IN;		//����ģʽ
	GPIO_InitStruct.GPIO_PuPd	= GPIO_PuPd_UP;     //����
	GPIO_Init(GPIOA, &GPIO_InitStruct);
}

void Water_lamp(void)
{
	
	GPIO_ResetBits(GPIOF, GPIO_Pin_9);
	delay_ms(LED_Time);
	GPIO_SetBits(GPIOF, GPIO_Pin_9);
	
	GPIO_ResetBits(GPIOF, GPIO_Pin_10);
	delay_ms(LED_Time);
	GPIO_SetBits(GPIOF, GPIO_Pin_10);
	
	GPIO_ResetBits(GPIOE, GPIO_Pin_13);
	delay_ms(LED_Time);
	GPIO_SetBits(GPIOE, GPIO_Pin_13);
	
	GPIO_ResetBits(GPIOE, GPIO_Pin_14);
	delay_ms(LED_Time);
	GPIO_SetBits(GPIOE, GPIO_Pin_14);

}
