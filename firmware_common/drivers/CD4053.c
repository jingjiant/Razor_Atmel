#include "configuration.h"
void CD4053Initialize(void)
{
  AT91C_BASE_PIOA->PIO_PER    = 0X00005800;
  AT91C_BASE_PIOA->PIO_OER    = 0X00005800;
}
void SelectState(u32 u32State)
{
    AT91C_BASE_PIOA->PIO_CODR   = OutPutClear;
    AT91C_BASE_PIOA->PIO_SODR   = u32State;
}