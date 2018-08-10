

#include "configuration.h"

//��������SendData()
//���ܣ� ��ת��оƬ����һ��ֱ�ӵ�����
 void SendData(u8 u8Data)         //����һ���ֽ�
{
  static u8 u8DataSize = 0;
  
  AT91C_BASE_PIOA->PIO_SODR   = OE; 
  AT91C_BASE_PIOA->PIO_SODR   = LE;
  AT91C_BASE_PIOA->PIO_CODR   = M_CLK;
  //Delay(10);
  
  for(u8DataSize=0;u8DataSize<8;u8DataSize++)
  {
    if((u8Data&0X01)==0X01)
    {
      AT91C_BASE_PIOA->PIO_SODR   = M_SDI;
    }
    
    else
    {
      AT91C_BASE_PIOA->PIO_CODR   = M_SDI;
    }
    
    AT91C_BASE_PIOA->PIO_CODR   = M_CLK;
    //Delay(10);
    AT91C_BASE_PIOA->PIO_SODR   = M_CLK;
    //Delay(10);
    
    u8Data = u8Data>>1;
  } 
  
}

//��������ABCD()
//���ܣ��ı������������ÿ1ms�л���һ��
void ABCD(u8 d,u8 c,u8 b,u8 a)   //ѡ�������һ��
{
  if(a==1)
  {
    AT91C_BASE_PIOA->PIO_SODR   = A;
  }
  
  else
  {
    AT91C_BASE_PIOA->PIO_CODR   = A;
  }
  
  if(b==1)
  {
    AT91C_BASE_PIOA->PIO_SODR   = B;
  }
  
  else
  {
    AT91C_BASE_PIOA->PIO_CODR   = B;
  }
  
  if(c==1)
  {
    AT91C_BASE_PIOA->PIO_SODR   = C;
  }
  
  else
  {
    AT91C_BASE_PIOA->PIO_CODR   = C;
  }
  
  if(d==1)
  {
    AT91C_BASE_PIOA->PIO_SODR   = D;
  }
  
  else
  {
    AT91C_BASE_PIOA->PIO_CODR   = D;
  }
}

//��������display()
//���ܣ�LED����ʾ����
void display(u8 u8Paral,u8 au8Message[16][10])  //ˢ����ʾ
{
  switch(u8Paral)
  {
  case 0:
    ABCD(0,0,0,0);
    break;
  case 1:
    ABCD(0,0,0,1);
    break;
  case 2:
    ABCD(0,0,1,0);
    break;
  case 3:
    ABCD(0,0,1,1);
    break;
  case 4:
    ABCD(0,1,0,0);
    break;
  case 5:
    ABCD(0,1,0,1);
    break;
  case 6:
    ABCD(0,1,1,0);
    break;
  case 7:
    ABCD(0,1,1,1);
    break;
  case 8:
    ABCD(1,0,0,0);
    break;
  case 9:
    ABCD(1,0,0,1);
    break;
  case 10: 
    ABCD(1,0,1,0);
    break;
  case 11: 
    ABCD(1,0,1,1);
    break;
  case 12: 
    ABCD(1,1,0,0);
    break;
  case 13:
    ABCD(1,1,0,1);
    break;
  case 14:
    ABCD(1,1,1,0);
    break;
  case 15:
    ABCD(1,1,1,1);
    break; 
  }
  
  for(u8 u8i=0;u8i<10;u8i++)
  {
    SendData(au8Message[u8Paral][9-u8i]);
  }
  AT91C_BASE_PIOA->PIO_CODR   = LE;
  AT91C_BASE_PIOA->PIO_CODR   = OE; 
}