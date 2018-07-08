#include "configuration.h"
static bool bDerection = TRUE;
void X9C103Initialize(void)
{
  AT91C_BASE_PIOA->PIO_PER    = 0X0001A200;
  AT91C_BASE_PIOA->PIO_OER    = 0X0001A200;
  AT91C_BASE_PIOA->PIO_CODR   = 0X00010000;
}
void IncreaseSound()
{
  static u8 u8i,u8j;
  if(bDerection)
  {
    bDerection = FALSE;
    AT91C_BASE_PIOA->PIO_SODR   = 0X0008000;
  }
  AT91C_BASE_PIOA->PIO_SODR   = 0X00002000;
  for(u8i=0;u8i<100;u8i++)
  {
    u8j++;
  }
  AT91C_BASE_PIOA->PIO_CODR   = 0X00002000;
}
void DecreaseSound()
{
  static u8 u8i,u8j;
  if(!bDerection)
  {
    bDerection = TRUE;
    AT91C_BASE_PIOA->PIO_CODR   = 0X0008000;
  }
    AT91C_BASE_PIOA->PIO_SODR   = 0X00002000;
  for(u8i=0;u8i<100;u8i++)
  {
    u8j++;
  }
  AT91C_BASE_PIOA->PIO_CODR   = 0X00002000;
}