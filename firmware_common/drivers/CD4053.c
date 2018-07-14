#include "configuration.h"

//函数：CD4053Initialize()
//使能CD4053的A，B，C，引脚
void CD4053Initialize(void)
{
  AT91C_BASE_PIOA->PIO_PER    = 0X00005800;
  AT91C_BASE_PIOA->PIO_OER    = 0X00005800;
}
//函数：SelectState()
//选择通道-AUD1，AUD2，GND，3.3V//
void SelectState(u32 u32State)
{
    AT91C_BASE_PIOA->PIO_CODR   = OutPutClear;
    AT91C_BASE_PIOA->PIO_SODR   = u32State;
}