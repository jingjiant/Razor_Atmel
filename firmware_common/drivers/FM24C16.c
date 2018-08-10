


#include "configuration.h"

//Function:初始化FM24C16
//Description：IO口属于空闲,准备开始通信
//Input:无
//Output:无
//Return:无
//Others:无
void AT24C02_Init(void)
{
    IO_Init();
}

//Function:写一个字节
//Description：向FM24C16的任意地址写入一个字节数据
//Input:写入的地址和数据
//Output:无
//Return:无
//Others:无
void At24c02_Write_Byte(u8 addr,u8 dat,u8 write)
{

    IIC_Start();    //启动
    IIC_Write_Byte(write);  //发送从设备地址
    IIC_Ack();  //等待从设备的响应
    IIC_Write_Byte(addr);   //发出芯片内地址
    IIC_Ack();  //等待从设备的响应
    IIC_Write_Byte(dat);    //发送数据
    IIC_Ack();  //等待从设备的响应
    IIC_Stop(); //停止
    Delay(10);   // 写入周期
}

//Function:指定地址读一个字节
//Description：向FM24C16的指定地址读出一个字节数据
//Input:读地址
//Output:无
//Return:读到的数据
//Others:无
u8 At24c02_Read_Byte(u8 addr,u8 read,u8 write)
{
  
    u8 dat;

    IIC_Start();
    IIC_Write_Byte(write);//发送发送从设备地址 写操作
    IIC_Ack();
    IIC_Write_Byte(addr);//发送芯片内地址
    IIC_Ack();
    IIC_Start();
    IIC_Write_Byte(read);//发送发送从设备地址 读操作
    IIC_Ack();
    dat = IIC_Read_Byte();//获取数据
    IIC_Stop();

    return dat;
}

//Function:任意地址写入一串数据
//Description：向FM24C16写入一串数据
//Input:写地址,写数据,数据长度
//Output:无
//Return:无
//Others:无
void At24c02_Serial_Write(u8 Writeaddr,u8 *Str,u16 Len,u8 write)
{
    while(Len--)
    {
        At24c02_Write_Byte(Writeaddr,*Str,write);
        Writeaddr++;
        Str++;
    }
}

//Function:指定地址读出一串数据
//Description：向FM24C16读出一串数据
//Input:读地址,读数据,数据长度
//Output:无
//Return:无
//Others:无
void At24c02_Serial_Read(u8 Readaddr,u8 *Str,u16 Len,u8 read,u8 write)
{
    while(Len)
    {
        *Str++ = At24c02_Read_Byte(Readaddr++,read,write);
        Len--;
    }
}
