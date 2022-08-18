#include "main.h"
#include "soft_i2c.h"
#include "gpio.h"
//IO��������
//SCL:PB6 pc7 \ SDA:PB7 pc6
void SOFT_I2C_SDA_IN(void)
	{
//		GPIO_InitTypeDef GPIO_InitStruct = {0};
//		GPIO_InitStruct.Pin = GPIO_PIN_7;
//		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
//		GPIO_InitStruct.Pull = GPIO_NOPULL;
//		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
//		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
		GPIO_Set(GPIOB,GPIO_PIN_7,GPIO_MODE_INPUT,0,GPIO_SPEED_FREQ_HIGH,GPIO_NOPULL);
	}

	void SOFT_I2C_SDA_OUT(void)
	{
//		GPIO_InitTypeDef GPIO_InitStruct = {0};
//		GPIO_InitStruct.Pin = GPIO_PIN_7;
//		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
//		GPIO_InitStruct.Pull = GPIO_NOPULL;
//		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
//		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
		GPIO_Set(GPIOB,GPIO_PIN_7,GPIO_MODE_OUTPUT_PP,0,GPIO_SPEED_FREQ_HIGH,GPIO_NOPULL);
	}
	

//IO��������	 
#define SOFT_I2C_SCL     PBout(6) //SCL
#define SOFT_I2C_SDA     PBout(7) //SDA	
#define MPU_READ_SDA   	PBin(7)
//PBin(7) //SDA	



void SOFT_I2C_Delay(void)
{
	//HAL_Delay(1);
	delay_quarter_us(1);
}
//����IIC
void SOFT_I2C_Start(void)
{
	//SOFT_I2C_SDA_OUT(); // sda�����
	
//	SOFT_I2C_SCL=0;
//	SOFT_I2C_SDA=0;
//	SOFT_I2C_Delay();
	
	SOFT_I2C_SDA=1;
	SOFT_I2C_SCL=1;
	SOFT_I2C_Delay();
	
	SOFT_I2C_SDA=0; // START:when CLK is high,DATA change form high to low
	SOFT_I2C_Delay();
	SOFT_I2C_SCL=0; //ǯסI2C���ߣ�׼�����ͻ��������
}
//����IICֹͣ�ź�
void SOFT_I2C_Stop(void)
{
//	SOFT_I2C_SCL=0;
	//SOFT_I2C_SDA_OUT(); // sda�����
//	SOFT_I2C_Delay();
	
	SOFT_I2C_SDA=0;
//	SOFT_I2C_SCL=0; // STOP:when CLK is high DATA change form low to high
//	SOFT_I2C_Delay();
	
	SOFT_I2C_SCL=1;
	SOFT_I2C_Delay();
	SOFT_I2C_SDA=1; //����I2C���߽����ź�
	SOFT_I2C_Delay();
//	SOFT_I2C_SCL=0;
	
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
u8 SOFT_I2C_Wait_Ack(void)
{
	u8 ucErrTime = 0;
	
	SOFT_I2C_SDA=1;
	SOFT_I2C_SDA_IN(); // SDA����Ϊ����
	//SOFT_I2C_Delay();
	SOFT_I2C_SCL=1;
	//SOFT_I2C_Delay();
	while (MPU_READ_SDA)
	{
		ucErrTime++;
		if (ucErrTime > 250)
		{
			SOFT_I2C_Stop();
			SOFT_I2C_SDA_OUT();
			return 1;
		}
	}
	SOFT_I2C_SCL=0; //ʱ�����0
	SOFT_I2C_SDA_OUT();
	return 0;
}
//����ACKӦ��
void SOFT_I2C_Ack(void)
{
	SOFT_I2C_SCL=0;
//	SOFT_I2C_SDA_OUT();
	
	SOFT_I2C_SDA=0;
	SOFT_I2C_Delay();
	
	SOFT_I2C_SCL=1;
	SOFT_I2C_Delay();
	
	SOFT_I2C_SCL=0;
	SOFT_I2C_Delay();
}
//������ACKӦ��
void SOFT_I2C_NAck(void)
{
	SOFT_I2C_SCL=0;
//	SOFT_I2C_SDA_OUT();
	
	SOFT_I2C_SDA=1;
	SOFT_I2C_Delay();
	
	SOFT_I2C_SCL=1;
	SOFT_I2C_Delay();
	
	SOFT_I2C_SCL=0;
	SOFT_I2C_Delay();
}


// IIC����һ���ֽ�
//���شӻ�����Ӧ��
// 1����Ӧ��
// 0����Ӧ��
void SOFT_I2C_Send_Byte(u8 txd)
{
	u8 t;
//	SOFT_I2C_SDA_OUT();
	SOFT_I2C_SCL=0; //����ʱ�ӿ�ʼ���ݴ���
	SOFT_I2C_Delay();
	
	for (t = 0; t < 8; t++)
	{
		SOFT_I2C_SDA = (txd & 0x80) >> 7;
		txd <<= 1;
		SOFT_I2C_SCL=1;
		SOFT_I2C_Delay();
		
		SOFT_I2C_SCL=0;
		SOFT_I2C_Delay();
		
	}
	//SOFT_I2C_SCL=0;
}
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK
u8 SOFT_I2C_Read_Byte(unsigned char ack)
{
	unsigned char i, receive = 0;
			SOFT_I2C_SCL=0;
		SOFT_I2C_Delay();
	SOFT_I2C_SDA_IN(); // SDA����Ϊ����
	for (i = 0; i < 8; i++)
	{
		SOFT_I2C_SCL=0;
		SOFT_I2C_Delay();
		SOFT_I2C_SCL=1;
		
		receive <<= 1;
		if (MPU_READ_SDA)
			receive++;
		SOFT_I2C_Delay();
	}
	if (!ack)
		SOFT_I2C_NAck(); //����nACK
	else
		SOFT_I2C_Ack(); //����ACK
	SOFT_I2C_SDA_OUT();
	return receive;
}

u8 i2cdevReadByte(u8 dev, uint8_t devAddress, uint8_t memAddress, uint8_t *data)
{
	u8 res;
	SOFT_I2C_Start();
	SOFT_I2C_Send_Byte((devAddress << 1) | 0); //����������ַ+д����
	SOFT_I2C_Wait_Ack();						//�ȴ�Ӧ��
	SOFT_I2C_Send_Byte(memAddress);					//д�Ĵ�����ַ
	SOFT_I2C_Wait_Ack();						//�ȴ�Ӧ��
	SOFT_I2C_Start();
	SOFT_I2C_Send_Byte((devAddress << 1) | 1); //����������ַ+������
	SOFT_I2C_Wait_Ack();						//�ȴ�Ӧ��
	*data = SOFT_I2C_Read_Byte(0);				//��ȡ����,����nACK
	SOFT_I2C_Stop();							//����һ��ֹͣ����
	return 1;
}

u8 i2cdevReadBit(u8 dev, uint8_t devAddress, uint8_t memAddress, uint8_t bitNum, uint8_t *data)
{
	uint8_t byte;
	u8 status;

	status = i2cdevRead(dev, devAddress, memAddress, 1, &byte);
	*data = byte & (1 << bitNum);

	return status;
}

u8 i2cdevWriteByte(u8 dev, uint8_t devAddress, uint8_t memAddress, uint8_t data)
{
	SOFT_I2C_Start();
	SOFT_I2C_Send_Byte((devAddress << 1) | 0); //����������ַ+д����
	if (SOFT_I2C_Wait_Ack())					//�ȴ�Ӧ��
	{
		SOFT_I2C_Stop();
		return 1;
	}
	SOFT_I2C_Send_Byte(memAddress);	 //д�Ĵ�����ַ
	SOFT_I2C_Wait_Ack();		 //�ȴ�Ӧ��
	SOFT_I2C_Send_Byte(data); //��������
	if (SOFT_I2C_Wait_Ack())	 //�ȴ�ACK
	{
		SOFT_I2C_Stop();
		return 1;
	}
	SOFT_I2C_Stop();
	return 0;
}

u8 i2cdevWrite(u8 dev, uint8_t devAddress, uint8_t memAddress, uint16_t len, uint8_t *data)
{
	u8 i;
	SOFT_I2C_Start();
	SOFT_I2C_Send_Byte((devAddress << 1) | 0); //����������ַ+д����
	if (SOFT_I2C_Wait_Ack())				//�ȴ�Ӧ��
	{
		SOFT_I2C_Stop();
		return 1;
	}
	SOFT_I2C_Send_Byte(memAddress); //д�Ĵ�����ַ
	SOFT_I2C_Wait_Ack();		//�ȴ�Ӧ��
	for (i = 0; i < len; i++)
	{
		SOFT_I2C_Send_Byte(data[i]); //��������
		if (SOFT_I2C_Wait_Ack())	   //�ȴ�ACK
		{
			SOFT_I2C_Stop();
			return 1;
		}
	}
	SOFT_I2C_Stop();
	return 0;
}


u8 i2cdevRead(u8 dev, uint8_t devAddress, uint8_t memAddress, uint16_t len, uint8_t *data)
{
	SOFT_I2C_Start();
	SOFT_I2C_Send_Byte((devAddress << 1) | 0); //����������ַ+д����
	if (SOFT_I2C_Wait_Ack())				//�ȴ�Ӧ��
	{
		SOFT_I2C_Stop();
		return 1;
	}
	SOFT_I2C_Send_Byte(memAddress); //д�Ĵ�����ַ
	SOFT_I2C_Wait_Ack();		//�ȴ�Ӧ��
	SOFT_I2C_Start();
	SOFT_I2C_Send_Byte((devAddress << 1) | 1); //����������ַ+������
	SOFT_I2C_Wait_Ack();					//�ȴ�Ӧ��
	while (len)
	{
		if (len == 1)
			*data = SOFT_I2C_Read_Byte(0); //������,����nACK
		else
			*data = SOFT_I2C_Read_Byte(1); //������,����ACK
		len--;
		data++;
	}
	SOFT_I2C_Stop(); //����һ��ֹͣ����
	return 0;
}

u8 i2cdevWriteBit(u8 dev, uint8_t devAddress, uint8_t memAddress, uint8_t bitNum, uint8_t data)
{
	uint8_t byte;
	i2cdevReadByte( dev,  devAddress,  memAddress,  &byte);
	byte = (data != 0) ? (byte | (1 << bitNum)) : (byte & ~(1 << bitNum));
	return i2cdevWriteByte(dev, devAddress, memAddress, byte);
}

u8 i2cdevWriteBits(u8 dev, uint8_t devAddress, uint8_t memAddress, uint8_t bitStart, uint8_t length, uint8_t data)
{
	u8 status;
	uint8_t byte;

	if ((status = i2cdevReadByte(dev, devAddress, memAddress, &byte)) == 1)
	{
		uint8_t mask = ((1 << length) - 1) << (bitStart - length + 1);
		data <<= (bitStart - length + 1); // shift data into correct position
		data &= mask; // zero all non-important bits in data
		byte &= ~(mask); // zero all important bits in existing byte
		byte |= data; // combine data with existing byte
		status = i2cdevWriteByte(dev, devAddress, memAddress, byte);
	}

	return status;
}

void iicsearch(void)
	{
		u8 i,j=0,count,add[10]={0,0,0,0,0,0,0,0,0,0};
for(i=0x00;i<0x7F;i++)
	 {
			SOFT_I2C_Start();
		 SOFT_I2C_Send_Byte((i << 1) | 0);
		
		if(SOFT_I2C_Wait_Ack()==0) 
		{
			add[j]=i;
			j++;
		}
		SOFT_I2C_Stop();
		delay_ms(1);
	 }
	 
 }