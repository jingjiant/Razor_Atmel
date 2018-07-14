#include "configuration.h"

static bool bDerection = TRUE;

//函数：X9C103Initialize();
//使能电位器的INC，U/D，！CS和继电器的RE引脚//
//初始化时将电位器的阻值调到最小//
void X9C103Initialize(void)
{
  static u8 u8i,u8j;
  AT91C_BASE_PIOA->PIO_PER    = 0X0001A000;
  AT91C_BASE_PIOA->PIO_OER    = 0X0001A000;
  AT91C_BASE_PIOA->PIO_CODR   = 0X00010000;
  AT91C_BASE_PIOB->PIO_PER    = 0X00000010;
  AT91C_BASE_PIOB->PIO_OER    = 0X00000010;
  
  AT91C_BASE_PIOA->PIO_CODR   = 0X0008000;      //将电位器阻值调到最小
  for(u8j=0;u8j<100;u8j++)
  {
  AT91C_BASE_PIOA->PIO_SODR   = 0X00002000;
  
  for(u8i=0;u8i<100;u8i++);
  
  AT91C_BASE_PIOA->PIO_CODR   = 0X00002000;
  }
}

//函数：IncreaseSound()
//调用一次产生十个INC引脚的下降沿，即电位器阻值增加十分之一//
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

//函数：DecreaseSound()
//调用一次产生10个下降沿，即电位器阻值减小十分之一
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