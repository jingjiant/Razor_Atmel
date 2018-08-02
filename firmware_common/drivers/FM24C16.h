

#ifndef _AT24C02_H
#define _AT24C02_H


#define IIC_WADDR   0XA0
#define IIC_RADDR   0XA1

void AT24C02_Init(void);

void At24c02_Write_Byte(u8 addr,u8 dat,u8 write);
u8 At24c02_Read_Byte(u8 addr,u8 read,u8 write);

void At24c02_Serial_Write(u8 Writeaddr,u8 *Str,u16 Len,u8 write);
void At24c02_Serial_Read(u8 Readaddr,u8 *Str,u16 Len,u8 read,u8 write);

#endif
