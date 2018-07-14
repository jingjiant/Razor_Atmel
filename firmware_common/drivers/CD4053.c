#include "configuration.h"

//������CD4053Initialize()
//ʹ��CD4053��A��B��C������
void CD4053Initialize(void)
{
  AT91C_BASE_PIOA->PIO_PER    = 0X00005800;
  AT91C_BASE_PIOA->PIO_OER    = 0X00005800;
}
//������SelectState()
//ѡ��ͨ��-AUD1��AUD2��GND��3.3V//
void SelectState(u32 u32State)
{
    AT91C_BASE_PIOA->PIO_CODR   = OutPutClear;
    AT91C_BASE_PIOA->PIO_SODR   = u32State;
}