#include "configuration.h"

#define OutPutAUD2 0X00000000
#define OutPutAUD1 0X00000800
#define OutPutGND  0X00001800
#define OutPutHigh 0X00004000
#define OutPutClear 0X00005800

void CD4053Initialize(void);

void SelectState(u32 u32State);