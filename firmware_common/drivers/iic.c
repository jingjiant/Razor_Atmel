

#include "configuration.h"

//Function:初始化IO口
//Description：GPIO初始化为高,通知器件总线空闲
//Input:无
//Output:无
//Return:无
//Others:无
void IO_Init(void)
{
    AT91C_BASE_PIOA->PIO_SODR = scl;
    Delay(10);
    AT91C_BASE_PIOA->PIO_SODR = sda;
    Delay(10);
}

//Function:启动IIC传输
//Description：IIC_SCL为高时,IIC_SDA由高到低
//Input:无
//Output:无
//Return:无
//Others:无
void IIC_Start(void)
{
    AT91C_BASE_PIOA->PIO_SODR = sda;
    AT91C_BASE_PIOA->PIO_SODR = scl;
    Delay(10);
    AT91C_BASE_PIOA->PIO_CODR = sda;
    Delay(10);
    AT91C_BASE_PIOA->PIO_CODR = scl;
}

//Function:停止IIC传输
//Description：IIC_SCL为高时,IIC_SDA由低到高
//Input:无
//Output:无
//Return:无
//Others:无
void IIC_Stop(void)
{
    AT91C_BASE_PIOA->PIO_CODR = scl;
    AT91C_BASE_PIOA->PIO_CODR = sda;
    Delay(10);
    AT91C_BASE_PIOA->PIO_SODR = scl;
    AT91C_BASE_PIOA->PIO_SODR = sda;
    Delay(10);
}

//Function:产生应答信号
//Description：IIC_SCL为高时,IIC_SDA被拉低并保持一定时间的低电平
//Input:无
//Output:无
//Return:无
//Others:无
void IIC_Ack(void)
{
    u8 i=0;

    AT91C_BASE_PIOA->PIO_SODR = scl;//在scl为高电平期间等待应答
    Delay(10);
    while((AT91C_BASE_PIOA->PIO_PDSR & sda) && i < 250)//若为应答0即退出，从机向主机发送应答信号
        i++;//通过i自增等待一段时间   
    AT91C_BASE_PIOA->PIO_CODR = scl;//得到应答,拉低时钟
    Delay(10);
}

//Function:产生非应答信号
//Description：IIC_SCL为高时,IIC_SDA为高
//Input:无
//Output:无
//Return:无
//Others:无
void IIC_NAck(void)
{
    AT91C_BASE_PIOA->PIO_SODR = scl;
    Delay(10);
    AT91C_BASE_PIOA->PIO_SODR = sda;
    AT91C_BASE_PIOA->PIO_CODR = scl;
    Delay(10);
}

//Function:I2C写一个字节数据，返回ACK或者NACK
//Description：从高位开始依次传输
//Input:写的字节
//Output:无
//Return:无
//Others:无
void IIC_Write_Byte(u8 txd)
{
   AT91C_BASE_PIOA->PIO_OER    = 0x00000200;
    u8 len;

    for (len=0;len<8;len++)
    {
        AT91C_BASE_PIOA->PIO_CODR = scl;//只有在IIC_SCL为低电平时,才允许IIC_SDA上的数据变化
        Delay(10);
        if(txd & 0x80)
            AT91C_BASE_PIOA->PIO_SODR = sda;
        else
            AT91C_BASE_PIOA->PIO_CODR = sda;
        Delay(10);
        AT91C_BASE_PIOA->PIO_SODR = scl;//在IIC_SCL为高电平时,不允许IIC_SDA上的数据变化，使数据稳定
        txd <<= 1;
        Delay(10);
        AT91C_BASE_PIOA->PIO_CODR = scl;   
        Delay(10);
    }
    AT91C_BASE_PIOA->PIO_SODR = sda;//释放总线
    AT91C_BASE_PIOA->PIO_SODR = scl;
    Delay(10);
}

//Function:I2C读一个字节数据，返回ACK或者NACK
//Description：从高到低,依次接收
//Input:是否应答
//Output:无
//Return:接收的数据
//Others:无
u8 IIC_Read_Byte(void)
{
  AT91C_BASE_PIOA->PIO_ODR    = 0x00000200;
   u8 len,dat = 0;

    AT91C_BASE_PIOA->PIO_SODR = sda;//准备读取数据
    for(len=0;len<8;len++) 
    {

        AT91C_BASE_PIOA->PIO_CODR = scl;
        Delay(10);
        AT91C_BASE_PIOA->PIO_SODR = scl;//不允许IIC_SDA变化
        Delay(10);
        dat <<= 1;
        dat |= (AT91C_BASE_PIOA->PIO_PDSR & sda)>>9; //读数据 
        Delay(10);
        AT91C_BASE_PIOA->PIO_CODR = scl;//允许IIC_SDA变化等待下一位数据的到来
        Delay(10);
    }   

    return dat;
}
