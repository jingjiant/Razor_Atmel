


#include "configuration.h"

//函数名：GB2312_Addr()
//功能：通过汉字的内码得到其在字库芯片中
//      16*16点阵数据的地址
u32  GB2312_Addr(u8 MSB,u8 LSB)
{
	static u32 Address;
        

	if(MSB >=0xA4 && MSB <= 0Xa8 && LSB >=0xA1) 
	Address = BaseAdd; 
	else if(MSB >=0xA1 && MSB <= 0Xa9 && LSB >=0xA1) 
	Address =( (MSB - 0xA1) * 94 + (LSB - 0xA1))*32+ BaseAdd; 
	else if(MSB >=0xB0 && MSB <= 0xF7 && LSB >=0xA1)     
	Address = ((MSB - 0xB0) * 94 + (LSB - 0xA1)+ 846)*32+ BaseAdd;

	return 	Address;
}

//函数名：SendComdData()
//功能：向字库芯片发送命令
void  SendComdData(u8 u8data)
{
    u8 i;
  //AT91C_BASE_PIOB->PIO_SODR = F_clk;  // SCLK=1;
  
  for(i=0;i<8;i++)	  
  {
    AT91C_BASE_PIOB->PIO_CODR = F_clk; // SCLK=0;
   if(u8data&0X80)
   {
      AT91C_BASE_PIOB->PIO_SODR = F_in;  //SI=1 
   }
   else
   {
      AT91C_BASE_PIOB->PIO_CODR = F_in; //SI=0
   }
   
   AT91C_BASE_PIOB->PIO_SODR = F_clk;  // SCLK=1;
   u8data<<=1;
  }
  
}

//函数名：ReceiveData()
//功能：从字库芯片读取数据
u8 ReceiveData(void)
{
   u8 RT_data = 0;
  AT91C_BASE_PIOB->PIO_SODR = F_clk; // SCLK=1;
  for(u8 i=0;i<8;i++)	   //读一个字节 
  {

   AT91C_BASE_PIOB->PIO_CODR = F_clk;  // SCLK=0;

   if(AT91C_BASE_PIOB->PIO_PDSR&F_out)
   {
     RT_data = RT_data|0X01;
   }
   else
   {
     RT_data = RT_data&0xFE;
   } 
   if(i<7)
   {
   RT_data<<=1;
   }
   AT91C_BASE_PIOB->PIO_SODR = F_clk; // SCLK=1;
  }
  //AT91C_BASE_PIOB->PIO_SODR = 	F_cs;  // CS=1;	
  return (RT_data);
}