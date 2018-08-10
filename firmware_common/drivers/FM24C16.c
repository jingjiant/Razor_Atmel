


#include "configuration.h"

//Function:��ʼ��FM24C16
//Description��IO�����ڿ���,׼����ʼͨ��
//Input:��
//Output:��
//Return:��
//Others:��
void AT24C02_Init(void)
{
    IO_Init();
}

//Function:дһ���ֽ�
//Description����FM24C16�������ַд��һ���ֽ�����
//Input:д��ĵ�ַ������
//Output:��
//Return:��
//Others:��
void At24c02_Write_Byte(u8 addr,u8 dat,u8 write)
{

    IIC_Start();    //����
    IIC_Write_Byte(write);  //���ʹ��豸��ַ
    IIC_Ack();  //�ȴ����豸����Ӧ
    IIC_Write_Byte(addr);   //����оƬ�ڵ�ַ
    IIC_Ack();  //�ȴ����豸����Ӧ
    IIC_Write_Byte(dat);    //��������
    IIC_Ack();  //�ȴ����豸����Ӧ
    IIC_Stop(); //ֹͣ
    Delay(10);   // д������
}

//Function:ָ����ַ��һ���ֽ�
//Description����FM24C16��ָ����ַ����һ���ֽ�����
//Input:����ַ
//Output:��
//Return:����������
//Others:��
u8 At24c02_Read_Byte(u8 addr,u8 read,u8 write)
{
  
    u8 dat;

    IIC_Start();
    IIC_Write_Byte(write);//���ͷ��ʹ��豸��ַ д����
    IIC_Ack();
    IIC_Write_Byte(addr);//����оƬ�ڵ�ַ
    IIC_Ack();
    IIC_Start();
    IIC_Write_Byte(read);//���ͷ��ʹ��豸��ַ ������
    IIC_Ack();
    dat = IIC_Read_Byte();//��ȡ����
    IIC_Stop();

    return dat;
}

//Function:�����ַд��һ������
//Description����FM24C16д��һ������
//Input:д��ַ,д����,���ݳ���
//Output:��
//Return:��
//Others:��
void At24c02_Serial_Write(u8 Writeaddr,u8 *Str,u16 Len,u8 write)
{
    while(Len--)
    {
        At24c02_Write_Byte(Writeaddr,*Str,write);
        Writeaddr++;
        Str++;
    }
}

//Function:ָ����ַ����һ������
//Description����FM24C16����һ������
//Input:����ַ,������,���ݳ���
//Output:��
//Return:��
//Others:��
void At24c02_Serial_Read(u8 Readaddr,u8 *Str,u16 Len,u8 read,u8 write)
{
    while(Len)
    {
        *Str++ = At24c02_Read_Byte(Readaddr++,read,write);
        Len--;
    }
}
