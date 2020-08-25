#include "iic.h"
#include "delay.h"

/*
����˵����

PB8 -- SCL
PB9 -- SDA


*/

void Iic_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;
	

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

	
	GPIO_InitStruct.GPIO_Pin	= GPIO_Pin_8|GPIO_Pin_9;		//����8 9
	GPIO_InitStruct.GPIO_Mode	= GPIO_Mode_OUT;	//���ģʽ
	GPIO_InitStruct.GPIO_OType	= GPIO_OType_PP;	//����
	GPIO_InitStruct.GPIO_Speed	= GPIO_Speed_50MHz; //����
	GPIO_InitStruct.GPIO_PuPd	= GPIO_PuPd_UP;     //����
	GPIO_Init(GPIOB, &GPIO_InitStruct);

	//���߿���
	SCL = 1;
	SDA_OUT = 1;
	
}

//SDAģʽ
void Iic_Sda_Mode(GPIOMode_TypeDef Mode)
{
	GPIO_InitTypeDef  GPIO_InitStruct;
	
	GPIO_InitStruct.GPIO_Pin	= GPIO_Pin_9;		//����9
	GPIO_InitStruct.GPIO_Mode	= Mode;	
	GPIO_InitStruct.GPIO_OType	= GPIO_OType_PP;	//����
	GPIO_InitStruct.GPIO_Speed	= GPIO_Speed_50MHz; //����
	GPIO_InitStruct.GPIO_PuPd	= GPIO_PuPd_UP;     //����
	GPIO_Init(GPIOB, &GPIO_InitStruct);

}


//��ʼ�ź�
void Iic_Start(void)
{
	Iic_Sda_Mode(GPIO_Mode_OUT);

	//���߿���
	SCL = 1;
	SDA_OUT = 1;
	delay_us(5);
	
	
	SDA_OUT = 0;
	delay_us(5);
	SCL = 0;  //ǯס����
}

//ֹͣ�ź�
void Iic_Stop(void)
{
	Iic_Sda_Mode(GPIO_Mode_OUT);
	
	SCL = 0;
	SDA_OUT = 0;
	delay_us(5);	
	
	SCL = 1;
	delay_us(5);
	SDA_OUT = 1;
}

//����һλ���� 
void Iic_Send_Ack(u8 ack)
{
	Iic_Sda_Mode(GPIO_Mode_OUT);
	
	SCL = 0;
	//׼������
	//Ҫ������1
	if(ack == 1)
	{
		SDA_OUT = 1;	 //������1
	}
	//Ҫ������0
	if(ack == 0)
	{
		SDA_OUT = 0;	 //������0
	}	
	
	delay_us(5);
	SCL = 1;
	delay_us(5);
	SCL = 0;

}

//��һ���ֽ�
void Iic_Send_Byte(u8 data)
{
	u8 i;
	
	Iic_Sda_Mode(GPIO_Mode_OUT);
	
	
	
	
	SCL = 0;
	
	for(i=0; i<8; i++)
	{
		//׼������ ������ 0x87 1 0 0 0 0 1 1 1 
		if(data & (1<<(7-i)))  
		{
			SDA_OUT = 1;	 //������1
		}
		//Ҫ������0
		else
		{
			SDA_OUT = 0;	 //������0
		}	
		delay_us(5);
		SCL = 1;
		delay_us(5);
		SCL = 0;
	}

}

//����һλ����
u8 Iic_Recv_Ack(void)
{
	u8 ack = 0;
	
	Iic_Sda_Mode(GPIO_Mode_IN);
	
	SCL = 0;
	delay_us(5);
	SCL = 1;
	delay_us(5);
	if(SDA_IN == 1) //�ж����ŵ�ƽ�Ƿ�Ϊ�ߵ�ƽ
	{
		ack = 1;
	}
	if(SDA_IN == 0) //�ж����ŵ�ƽ�Ƿ�Ϊ�͵�ƽ
	{
		ack = 0;
	}	
	
	SCL = 0;

	return ack;
}

//����һ���ֽ�
u8 Iic_Recv_Byte(void)
{
	u8 i, data = 0;  //0 0 0 0 0 0 0 0
	
	Iic_Sda_Mode(GPIO_Mode_IN);
	
	SCL = 0;
	//ѭ��8�Σ�����һ���ֽ�
	for(i=0; i<8; i++)
	{	
		delay_us(5);
		SCL = 1;
		delay_us(5);
		if(SDA_IN == 1) //�ж����ŵ�ƽ�Ƿ�Ϊ�ߵ�ƽ
		{
			data |= (1<<(7-i));
		}

		SCL = 0;
	}
	
	return data;
}


void AT24C02_Write(u8 addr, u8 *write_buf, u8 len)
{
	u8 ack;
	//��ʼ�ź�
	Iic_Start();
	
	//�������õ�ַ����ִ��д����
	Iic_Send_Byte(0xA0);
	ack = Iic_Recv_Ack();
	if(ack == 1)
	{
		printf("ack failure\n");
		Iic_Stop();
		return;
	}
	
	//����д���ݵ���ʼ��ַ
	Iic_Send_Byte(addr);
	ack = Iic_Recv_Ack();
	if(ack == 1)
	{
		printf("ack failure\n");
		Iic_Stop();
		return;
	}	
	
	while(len--)
	{
		//��������
		Iic_Send_Byte(*write_buf);
		ack = Iic_Recv_Ack();
		if(ack == 1)
		{
			printf("ack failure\n");
			Iic_Stop();
			return;
		}
		//��ַ��1
		write_buf++;	
	}
	
	
	Iic_Stop();
	
	printf("write finish\n");

}


void AT24C02_Read(u8 addr, u8 *read_buf, u8 len)
{
	u8 ack;
	//��ʼ�ź�
	Iic_Start();
	//�������õ�ַ����ִ��д����
	Iic_Send_Byte(0xA0);
	ack = Iic_Recv_Ack();
	if(ack == 1)
	{
		printf("ack failure\n");
		Iic_Stop();
		return;
	}
	
	//���Ͷ����ݵ���ʼ��ַ
	Iic_Send_Byte(addr);
	ack = Iic_Recv_Ack();
	if(ack == 1)
	{
		printf("ack failure\n");
		Iic_Stop();
		return;
	}		
	//��ʼ�ź�
	Iic_Start();
	
	//�������õ�ַ����ִ�ж�����
	Iic_Send_Byte(0xA1);
	ack = Iic_Recv_Ack();
	if(ack == 1)
	{
		printf("ack failure\n");
		Iic_Stop();
		return;
	}	
	
	while(len--)  //len = 5
	{
		
		// len 4 3 2 1 0
		//��������
		*read_buf = Iic_Recv_Byte();
		//��ַ��1
		read_buf++;
		
		if(len > 0)
			Iic_Send_Ack(0);	

	}
	
	//���ͷ�Ӧ��
	Iic_Send_Ack(1);	
	Iic_Stop();
	
	printf("read finish\n");	
	

}

