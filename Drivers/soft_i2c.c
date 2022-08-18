#include "main.h"
#include "soft_i2c.h"
#include "gpio.h"
//IO方向设置
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
	

//IO操作函数	 
#define SOFT_I2C_SCL     PBout(6) //SCL
#define SOFT_I2C_SDA     PBout(7) //SDA	
#define MPU_READ_SDA   	PBin(7)
//PBin(7) //SDA	



void SOFT_I2C_Delay(void)
{
	//HAL_Delay(1);
	delay_quarter_us(1);
}
//产生IIC
void SOFT_I2C_Start(void)
{
	//SOFT_I2C_SDA_OUT(); // sda线输出
	
//	SOFT_I2C_SCL=0;
//	SOFT_I2C_SDA=0;
//	SOFT_I2C_Delay();
	
	SOFT_I2C_SDA=1;
	SOFT_I2C_SCL=1;
	SOFT_I2C_Delay();
	
	SOFT_I2C_SDA=0; // START:when CLK is high,DATA change form high to low
	SOFT_I2C_Delay();
	SOFT_I2C_SCL=0; //钳住I2C总线，准备发送或接收数据
}
//产生IIC停止信号
void SOFT_I2C_Stop(void)
{
//	SOFT_I2C_SCL=0;
	//SOFT_I2C_SDA_OUT(); // sda线输出
//	SOFT_I2C_Delay();
	
	SOFT_I2C_SDA=0;
//	SOFT_I2C_SCL=0; // STOP:when CLK is high DATA change form low to high
//	SOFT_I2C_Delay();
	
	SOFT_I2C_SCL=1;
	SOFT_I2C_Delay();
	SOFT_I2C_SDA=1; //发送I2C总线结束信号
	SOFT_I2C_Delay();
//	SOFT_I2C_SCL=0;
	
}
//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
u8 SOFT_I2C_Wait_Ack(void)
{
	u8 ucErrTime = 0;
	
	SOFT_I2C_SDA=1;
	SOFT_I2C_SDA_IN(); // SDA设置为输入
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
	SOFT_I2C_SCL=0; //时钟输出0
	SOFT_I2C_SDA_OUT();
	return 0;
}
//产生ACK应答
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
//不产生ACK应答
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


// IIC发送一个字节
//返回从机有无应答
// 1，有应答
// 0，无应答
void SOFT_I2C_Send_Byte(u8 txd)
{
	u8 t;
//	SOFT_I2C_SDA_OUT();
	SOFT_I2C_SCL=0; //拉低时钟开始数据传输
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
//读1个字节，ack=1时，发送ACK，ack=0，发送nACK
u8 SOFT_I2C_Read_Byte(unsigned char ack)
{
	unsigned char i, receive = 0;
			SOFT_I2C_SCL=0;
		SOFT_I2C_Delay();
	SOFT_I2C_SDA_IN(); // SDA设置为输入
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
		SOFT_I2C_NAck(); //发送nACK
	else
		SOFT_I2C_Ack(); //发送ACK
	SOFT_I2C_SDA_OUT();
	return receive;
}

u8 i2cdevReadByte(u8 dev, uint8_t devAddress, uint8_t memAddress, uint8_t *data)
{
	u8 res;
	SOFT_I2C_Start();
	SOFT_I2C_Send_Byte((devAddress << 1) | 0); //发送器件地址+写命令
	SOFT_I2C_Wait_Ack();						//等待应答
	SOFT_I2C_Send_Byte(memAddress);					//写寄存器地址
	SOFT_I2C_Wait_Ack();						//等待应答
	SOFT_I2C_Start();
	SOFT_I2C_Send_Byte((devAddress << 1) | 1); //发送器件地址+读命令
	SOFT_I2C_Wait_Ack();						//等待应答
	*data = SOFT_I2C_Read_Byte(0);				//读取数据,发送nACK
	SOFT_I2C_Stop();							//产生一个停止条件
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
	SOFT_I2C_Send_Byte((devAddress << 1) | 0); //发送器件地址+写命令
	if (SOFT_I2C_Wait_Ack())					//等待应答
	{
		SOFT_I2C_Stop();
		return 1;
	}
	SOFT_I2C_Send_Byte(memAddress);	 //写寄存器地址
	SOFT_I2C_Wait_Ack();		 //等待应答
	SOFT_I2C_Send_Byte(data); //发送数据
	if (SOFT_I2C_Wait_Ack())	 //等待ACK
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
	SOFT_I2C_Send_Byte((devAddress << 1) | 0); //发送器件地址+写命令
	if (SOFT_I2C_Wait_Ack())				//等待应答
	{
		SOFT_I2C_Stop();
		return 1;
	}
	SOFT_I2C_Send_Byte(memAddress); //写寄存器地址
	SOFT_I2C_Wait_Ack();		//等待应答
	for (i = 0; i < len; i++)
	{
		SOFT_I2C_Send_Byte(data[i]); //发送数据
		if (SOFT_I2C_Wait_Ack())	   //等待ACK
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
	SOFT_I2C_Send_Byte((devAddress << 1) | 0); //发送器件地址+写命令
	if (SOFT_I2C_Wait_Ack())				//等待应答
	{
		SOFT_I2C_Stop();
		return 1;
	}
	SOFT_I2C_Send_Byte(memAddress); //写寄存器地址
	SOFT_I2C_Wait_Ack();		//等待应答
	SOFT_I2C_Start();
	SOFT_I2C_Send_Byte((devAddress << 1) | 1); //发送器件地址+读命令
	SOFT_I2C_Wait_Ack();					//等待应答
	while (len)
	{
		if (len == 1)
			*data = SOFT_I2C_Read_Byte(0); //读数据,发送nACK
		else
			*data = SOFT_I2C_Read_Byte(1); //读数据,发送ACK
		len--;
		data++;
	}
	SOFT_I2C_Stop(); //产生一个停止条件
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