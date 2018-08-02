

#include "configuration.h"

//Function:��ʼ��IO��
//Description��GPIO��ʼ��Ϊ��,֪ͨ�������߿���
//Input:��
//Output:��
//Return:��
//Others:��
void IO_Init(void)
{
    AT91C_BASE_PIOA->PIO_SODR = scl;
    Delay(10);
    AT91C_BASE_PIOA->PIO_SODR = sda;
    Delay(10);
}

//Function:����IIC����
//Description��IIC_SCLΪ��ʱ,IIC_SDA�ɸߵ���
//Input:��
//Output:��
//Return:��
//Others:��
void IIC_Start(void)
{
    AT91C_BASE_PIOA->PIO_SODR = sda;
    AT91C_BASE_PIOA->PIO_SODR = scl;
    Delay(10);
    AT91C_BASE_PIOA->PIO_CODR = sda;
    Delay(10);
    AT91C_BASE_PIOA->PIO_CODR = scl;
}

//Function:ֹͣIIC����
//Description��IIC_SCLΪ��ʱ,IIC_SDA�ɵ͵���
//Input:��
//Output:��
//Return:��
//Others:��
void IIC_Stop(void)
{
    AT91C_BASE_PIOA->PIO_CODR = scl;
    AT91C_BASE_PIOA->PIO_CODR = sda;
    Delay(10);
    AT91C_BASE_PIOA->PIO_SODR = scl;
    AT91C_BASE_PIOA->PIO_SODR = sda;
    Delay(10);
}

//Function:����Ӧ���ź�
//Description��IIC_SCLΪ��ʱ,IIC_SDA�����Ͳ�����һ��ʱ��ĵ͵�ƽ
//Input:��
//Output:��
//Return:��
//Others:��
void IIC_Ack(void)
{
    u8 i=0;

    AT91C_BASE_PIOA->PIO_SODR = scl;//��sclΪ�ߵ�ƽ�ڼ�ȴ�Ӧ��
    Delay(10);
    while((AT91C_BASE_PIOA->PIO_PDSR & sda) && i < 250)//��ΪӦ��0���˳����ӻ�����������Ӧ���ź�
        i++;//ͨ��i�����ȴ�һ��ʱ��   
    AT91C_BASE_PIOA->PIO_CODR = scl;//�õ�Ӧ��,����ʱ��
    Delay(10);
}

//Function:������Ӧ���ź�
//Description��IIC_SCLΪ��ʱ,IIC_SDAΪ��
//Input:��
//Output:��
//Return:��
//Others:��
void IIC_NAck(void)
{
    AT91C_BASE_PIOA->PIO_SODR = scl;
    Delay(10);
    AT91C_BASE_PIOA->PIO_SODR = sda;
    AT91C_BASE_PIOA->PIO_CODR = scl;
    Delay(10);
}

//Function:I2Cдһ���ֽ����ݣ�����ACK����NACK
//Description���Ӹ�λ��ʼ���δ���
//Input:д���ֽ�
//Output:��
//Return:��
//Others:��
void IIC_Write_Byte(u8 txd)
{
   AT91C_BASE_PIOA->PIO_OER    = 0x00000200;
    u8 len;

    for (len=0;len<8;len++)
    {
        AT91C_BASE_PIOA->PIO_CODR = scl;//ֻ����IIC_SCLΪ�͵�ƽʱ,������IIC_SDA�ϵ����ݱ仯
        Delay(10);
        if(txd & 0x80)
            AT91C_BASE_PIOA->PIO_SODR = sda;
        else
            AT91C_BASE_PIOA->PIO_CODR = sda;
        Delay(10);
        AT91C_BASE_PIOA->PIO_SODR = scl;//��IIC_SCLΪ�ߵ�ƽʱ,������IIC_SDA�ϵ����ݱ仯��ʹ�����ȶ�
        txd <<= 1;
        Delay(10);
        AT91C_BASE_PIOA->PIO_CODR = scl;   
        Delay(10);
    }
    AT91C_BASE_PIOA->PIO_SODR = sda;//�ͷ�����
    AT91C_BASE_PIOA->PIO_SODR = scl;
    Delay(10);
}

//Function:I2C��һ���ֽ����ݣ�����ACK����NACK
//Description���Ӹߵ���,���ν���
//Input:�Ƿ�Ӧ��
//Output:��
//Return:���յ�����
//Others:��
u8 IIC_Read_Byte(void)
{
  AT91C_BASE_PIOA->PIO_ODR    = 0x00000200;
   u8 len,dat = 0;

    AT91C_BASE_PIOA->PIO_SODR = sda;//׼����ȡ����
    for(len=0;len<8;len++) 
    {

        AT91C_BASE_PIOA->PIO_CODR = scl;
        Delay(10);
        AT91C_BASE_PIOA->PIO_SODR = scl;//������IIC_SDA�仯
        Delay(10);
        dat <<= 1;
        dat |= (AT91C_BASE_PIOA->PIO_PDSR & sda)>>9; //������ 
        Delay(10);
        AT91C_BASE_PIOA->PIO_CODR = scl;//����IIC_SDA�仯�ȴ���һλ���ݵĵ���
        Delay(10);
    }   

    return dat;
}
