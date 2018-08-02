
#include "configuration.h"


volatile u8 au8DataMessage[16][10];
extern volatile u8 u8CharNumber;

u8 au8CharSave[128][16]={
  {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},//"未命名文件",0
  {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},//"未命名文件",0
  
};

void CharData(void)      //对数据移位实现滚动
{
  
  static u8 au8yichudata[16];                             
  static u8 u8a,u8b,u8d=0;
  static bool bFalg = FALSE;
  
  
  
  u8d = u8CharNumber;
  if(u8d<10)
  {
    u8d = 10;
  }
  
  if(bFalg)
  {
    for(u8a=0;u8a<16;u8a++)
    {
      au8yichudata[u8a]=(au8CharSave[0][u8a]&0x80);
    }
    
    for(u8a=0;u8a<u8d;u8a++)
    {
      for(u8b=0;u8b<16;u8b++)
      {
        au8CharSave[u8a][u8b]=(au8CharSave[u8a][u8b]<<1);
      }
      if(u8a<(u8d-1))
      {
        for(u8b=0;u8b<16;u8b++)
        {
          au8CharSave[u8a][u8b]=au8CharSave[u8a][u8b]|((au8CharSave[u8a+1][u8b]&0X80)>>7);
        }
      }
    }
    
    for(u8a=0;u8a<16;u8a++)
    {
      au8CharSave[u8d-1][u8a]=au8CharSave[u8d-1][u8a]|(au8yichudata[u8a]>>7);
    }
  }
  
  bFalg = TRUE;
  
  for(u8a=0;u8a<10;u8a++)
  {
    for(u8b=0;u8b<16;u8b++)
    {
      au8DataMessage[u8b][u8a]=au8CharSave[u8a][u8b];
    }
  }
  
}

u8 *GetZiKuWord(u8 MSB,u8 LSB)
{
  static u32 u32Count = 0;
  static u8 au8[32];
  static u8 u8adr;
  static u32 u32adressd = 0;
  
  u32adressd = GB2312_Addr(MSB,LSB);
  AT91C_BASE_PIOB->PIO_CODR = F_cs;  //CS-----0
  
  SendComdData(0X03);
  u8adr = ((u8)(u32adressd>>16)&0xff);
  SendComdData(u8adr);
  u8adr = ((u8)(u32adressd>>8)&0xff);
  SendComdData(u8adr);
  u8adr = ((u8)(u32adressd>>0)&0xff);
  SendComdData(u8adr);
  for(u32Count=0;u32Count<32;u32Count++)
  {
    au8[u32Count] = ReceiveData();
  }
  
  AT91C_BASE_PIOB->PIO_SODR = 	F_cs; 
  
  return au8;
}