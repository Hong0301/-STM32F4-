#include "dht11.h"
#include "delay.h"
#include "sys.h"




/*
����˵����PG9 -- DQ
*/

void Dht11_Init(void)
{
	GPIO_InitTypeDef  	GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;				//��9������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;			//���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;			//�����������ǿ�������������ŵ������������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;		//���ŵ��ٶ����Ϊ100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;		//û��ʹ���ڲ���������
	GPIO_Init(GPIOG, &GPIO_InitStructure);	
	
	//��ʪ��ģ�黹û�й�������ô���Ĵ��������Ǹߵ�ƽ
	PGout(9)=1;
}

//����ģʽ���
void Dht11_Pin_Mode(GPIOMode_TypeDef mode)
{
	GPIO_InitTypeDef  	GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_9;				//��9������
	GPIO_InitStructure.GPIO_Mode  = mode;					//����/���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;			//�����������ǿ�������������ŵ������������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;		//���ŵ��ٶ����Ϊ100MHz
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;		//û��ʹ���ڲ���������
	GPIO_Init(GPIOG, &GPIO_InitStructure);	
}

//����DHT11��������0 �쳣����-1
int32_t Dht11_Start(void)
{
	u32 t = 0;

	//�����ź�
	Dht11_Pin_Mode(GPIO_Mode_OUT);
	PGout(9) = 1;
	delay_us(50);
	PGout(9) = 0;
	delay_ms(20);
	PGout(9) = 1;
	delay_us(30);
	
	Dht11_Pin_Mode(GPIO_Mode_IN);
	
	t = 0;
	//�ȴ��͵�ƽ����
	while(PGin(9) == 1)
	{
		t++;
		delay_us(2);
	
		if(t >= 1000) //�ȴ�2msδ�ܵȴ����͵�ƽ
		{
			return -1;
		}	
		
	}
	
	delay_us(40);
	t = 0;
	
	//�ȴ��ߵ�ƽ���� ���˵͵�ƽ
	while(PGin(9) == 0)
	{
		t++;
		delay_us(2);
	
		if(t >= 80) //�ȴ�160usδ�ܵȴ����ߵ�ƽ
		{
			return -1;
		}	
	
	}		
	delay_us(40);
	t = 0;	
	//�ȴ��͵�ƽ���� ���˸ߵ�ƽ
	while(PGin(9) == 1)
	{
		t++;
		delay_us(2);
	
		if(t >= 80) //�ȴ�160usδ�ܵȴ����͵�ƽ
		{
			return -1;
		}		
	
	}
	
	return 0;
}

//һ���Զ�ȡ��λ����
uint8_t Dht11_Read_Byte(void)
{
	u8 i, data = 0;  //0000 0000
	u32 t = 0;
	
	for(i=0; i<8; i++)
	{
		t = 0;	
		//�ȴ��ߵ�ƽ���� ���˵͵�ƽ
		while(PGin(9) == 0)
		{
			t++;
			delay_us(2);
		
			if(t >= 50) //�ȴ�100usδ�ܵȴ����ߵ�ƽ
			{
				return 0;
			}	
		
		}		
	
		delay_us(40);
		
		if(PGin(9) == 1)
		{
			data |= (1<<(7-i));
			t = 0;
			//�ȴ��͵�ƽ���� ���˸ߵ�ƽ
			while(PGin(9) == 1)
			{
				t++;
				delay_us(2);
			
				if(t >= 50) //�ȴ�100usδ�ܵȴ����͵�ƽ
				{
					return -1;
				}		
			
			}		
		
		}
	
	}
	
	return data;
	
}

//�ɹ�����0��ʧ�ܷ���-1
int32_t Dht11_Read(u8 *data)
{
	u8 i=0;
	
	for(i=0; i<5; i++)
	{
		data[i] = Dht11_Read_Byte();
	}
	
	if(data[4] == data[0]+data[1]+data[2]+data[3])
	{
		return 0;
	}
	else
	{
		return -1;	
	}

}

