#include "configuration.h"

static bool bDerection = TRUE;

//������X9C103Initialize();
//ʹ�ܵ�λ����INC��U/D����CS�ͼ̵�����RE����//
//��ʼ��ʱ����λ������ֵ������С//
void X9C103Initialize(void)
{
  static u8 u8i,u8j;
  AT91C_BASE_PIOA->PIO_PER    = 0X0001A000;
  AT91C_BASE_PIOA->PIO_OER    = 0X0001A000;
  AT91C_BASE_PIOA->PIO_CODR   = 0X00010000;
  AT91C_BASE_PIOB->PIO_PER    = 0X00000010;
  AT91C_BASE_PIOB->PIO_OER    = 0X00000010;
  
  AT91C_BASE_PIOA->PIO_CODR   = 0X0008000;      //����λ����ֵ������С
  for(u8j=0;u8j<100;u8j++)
  {
  AT91C_BASE_PIOA->PIO_SODR   = 0X00002000;
  
  for(u8i=0;u8i<100;u8i++);
  
  AT91C_BASE_PIOA->PIO_CODR   = 0X00002000;
  }
}

//������IncreaseSound()
//����һ�β���ʮ��INC���ŵ��½��أ�����λ����ֵ����ʮ��֮һ//
void IncreaseSound()
{
  static u8 u8i,u8j;
  
  if(bDerection)
  {
    bDerection = FALSE;
    AT91C_BASE_PIOA->PIO_SODR   = 0X0008000;
  }
  
  for(u8j=0;u8j<10;u8j++)
  {
  AT91C_BASE_PIOA->PIO_SODR   = 0X00002000;
  
  for(u8i=0;u8i<100;u8i++);

  AT91C_BASE_PIOA->PIO_CODR   = 0X00002000;
  }
}

//������DecreaseSound()
//����һ�β���10���½��أ�����λ����ֵ��Сʮ��֮һ
void DecreaseSound()
{
  static u8 u8i,u8j;
  
  if(!bDerection)
  {
    bDerection = TRUE;
    AT91C_BASE_PIOA->PIO_CODR   = 0X0008000;
  }
  
  for(u8j=0;u8j<10;u8j++)
  {
    AT91C_BASE_PIOA->PIO_SODR   = 0X00002000;
    
  for(u8i=0;u8i<100;u8i++);
  
  
  AT91C_BASE_PIOA->PIO_CODR   = 0X00002000;
  }
}