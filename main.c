#include "stm32f4xx.h"
#include "led.h"
#include "key.h"
#include "exti.h"
#include "delay.h"
#include "tim.h"
#include "pwm.h"
#include "usart.h"
#include "string.h"
#include "sys.h"
#include "stdio.h"
#include "iwdg.h"
#include "rtc.h"
#include "iic.h"
#include "OLED_I2C.h"
#include "mlx90614.h"
#include "dht11.h"
#include <stdlib.h>

#define LED0_ON    GPIO_ResetBits(GPIOF, GPIO_Pin_9)
#define LED0_OFF   GPIO_SetBits(GPIOF, GPIO_Pin_9)



u8 Usart_Data;   //ֵ��Χ��0~255
u8 rx_flag = 0;  //����������� rx_flag = 1
u8 buffer[64] = {0};
u8 rx_buffer[64] = {0};
u8 rx_i=0, count = 0;



void USART1_IRQHandler(void)
{
	
	 
   //���Ƿǿգ��򷵻�ֵΪ1����RESET��0���жϣ���������ж�Ϊ��
   if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
   {	
		//�ж�Ϊ���Ϊ�´��ж���׼��������Ҫ���жϵı�־����
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);
		/* DR��ȡ���ܵ�������*/
		buffer[count++] = USART_ReceiveData(USART1);	
	   
	   if(buffer[count-1] == ':')
	   {
		   //ȥ��:
			for(rx_i=0; rx_i< (count-1); rx_i++)
			{
				rx_buffer[rx_i] = buffer[rx_i];
			}
	   
			memset(buffer, 0, sizeof(buffer));
			count = 0;  //��Ϊ0,��һ֡���ݴ�buffer[0]��ʼ�洢
			rx_flag = 1; //����������� rx_flag = 1
	   }
	   
	
   }

}


int Get_SR04_Value(void)
{

	
	u32 echo_time, t = 0;

	//��trig��������10us�ߵ�ƽ
	PAout(2) = 0;
	delay_us(5);
	PAout(2) = 1;
	delay_us(15);
	PAout(2) = 0;
	
	//���ö�ʱ��CNTֵΪ0
	TIM_SetCounter(TIM3, 0);
	//TIM3->CNT;
	
	//�ȴ�echo�ߵ�ƽ  ����ʱ�ȴ�
	while(PAin(3) == 0)
	{
		t++;
		delay_us(2);
		if( t >= 1000) //2ms
			return -1;
	
	}
	//����ʱ��
	TIM_Cmd(TIM3, ENABLE);
	//����ֱ���ߵ�ƽ����
	
	//����ʱ�ȴ�
	t = 0; 
	while(PAin(3) == 1)
	{
		t++;
		delay_us(2);
		if( t >= 11500) //23.2ms
		{
			TIM_Cmd(TIM3, DISABLE);
			return -2;	
		}				
	}
	//�ض�ʱ��
	TIM_Cmd(TIM3, DISABLE);

	//��ȡCNTֵ
	echo_time = TIM_GetCounter(TIM3);
	//��ʱ �������
	delay_s(1);
	return echo_time/58.0;
}


void init_func(void)
{
			//NVIC���飨һ�����̵���ֻ������һ�η��飩��ռ���ȼ�2λ,ֵ��Χ��0~3����Ӧ���ȼ�2λ,ֵ��Χ��0~3��
			NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
			Delay_Init();                      //��ʱ
			Led_Init();                       //LED�Ƴ�ʼ������������ʼ��	
			Usart1_Init();										//����
			Pwm_Motor_Init();									//���
			
			I2C_Configuration();
			OLED_Init();                       //OLED��ʼ��
			SMBus_Init();                      //���⴫����ģ���ʼ��
			Rtc_Init();                        //ʵʱʱ�ӳ�ʼ��
			Dht11_Init();                      //��ʪ��ģ���ʼ��
			
			//�ж���
			Exti0_Init();
			Exti2_Init();
			Exti3_Init();
			Exti4_Init();
			
			//�������Ͷ�ʱ��
			PA2_Init();		//trig
			PA3_Init();		//echo
			Tim3_Init();

}	


void OLED_show()
{
		RTC_TimeTypeDef RTC_TimeStruct;
		RTC_DateTypeDef RTC_DateStruct;
		char RTC_time[64];
		char RTC_date[64];
		char temp_humi_l[64];
		char distance[64];
		int to=1;
		float temp;
		int value;
		extern const unsigned char BMP1[]; //ͼƬ
		u8 i;
		
		init_func();

		
		OLED_Fill(0x00);//ȫ����
		
		
		while(1)
		{

			//LED_Fill(0xFF);//ȫ������
			//delay_s(2);
			//OLED_Fill(0x00);//ȫ����
		//	delay_s(2);
		/*	
			for(i=5;i<11;i++)
			{
				OLED_ShowCN((i-5)*16,0,i);//������ʾ����
			}
		*/
			memset(RTC_time,0,sizeof(RTC_time));
			memset(RTC_date,0,sizeof(RTC_date));
			//ʱ������
			RTC_GetTime(RTC_Format_BIN, &RTC_TimeStruct);
			RTC_GetDate(RTC_Format_BIN, &RTC_DateStruct);
			sprintf(RTC_time, "-Time:%d:%d:%d-", RTC_TimeStruct.RTC_Hours, RTC_TimeStruct.RTC_Minutes, 
																		RTC_TimeStruct.RTC_Seconds);
			sprintf(RTC_date, "-Date:20%d.%d.%d-",RTC_DateStruct.RTC_Year, 
									RTC_DateStruct.RTC_Month, RTC_DateStruct.RTC_Date);
			
			//OLED��ʾʱ������
			OLED_ShowStr(0,1,(u8 *)RTC_time,1);//����6*8�ַ�
			OLED_ShowStr(0,3,(u8 *)RTC_date,1);				//����8*16�ַ�
			
			
			
			//TIM_SetCompare3(TIM2,100);
			
			//TIM_SetCompare4(TIM2,10);	
			
			//������
			value = Get_SR04_Value();
			
			//�������
			//printf("��������Ϊ%dcm\n", value);
			//��ʱ �������
			
			if(value < 10)
						Water_lamp();
			printf("����%d\r\n",value);
			
			sprintf(distance,"**distance :%d cm**",value);
			OLED_ShowStr(0,7,(u8 *)distance,1);
			//�ɼ���ʪ��
			u8 data[5] = {0};
			int ret;
			ret = Dht11_Start();
			if(ret == 0)
			{
					Dht11_Read(data);
					
			}
			memset(temp_humi_l,0,sizeof(temp_humi_l));
			sprintf(temp_humi_l,"-T:%d.%d H:%d.%d-",data[2],data[3],data[0],data[1]);
			OLED_ShowStr(0,5,(u8 *)temp_humi_l,1);
			

			//delay_s(2);
			//OLED_CLS();//����
			//OLED_OFF();//����OLED����
			//delay_s(2);
			//OLED_ON();//����OLED���ߺ���
			//OLED_DrawBMP(0,0,128,8,(unsigned char *)BMP1);//����BMPλͼ��ʾ
			//delay_s(2);
		
			}


}	


int main(void)
{
	OLED_show();
	return 0;
}



