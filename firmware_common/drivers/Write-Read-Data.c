

#include "configuration.h"

//函数名：SendData()
//功能： 向串转并芯片发送一个直接的数据
 void SendData(u8 u8Data)         //发送一个字节
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

//函数名：ABCD()
//功能：改变点亮的行数，每1ms切换下一行
void ABCD(u8 d,u8 c,u8 b,u8 a)   //选择点亮哪一行
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

//函数名：display()
//功能：LED屏显示汉字
void display(u8 u8Paral,u8 au8Message[16][10])  //刷屏显示
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