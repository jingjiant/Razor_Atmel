

#ifndef _IIC_H
#define _IIC_H

#define scl (1<<10)
#define sda (1<<9)

void IO_Init(void);
void IIC_Start(void);
void IIC_Stop(void);
void IIC_Ack(void);
void IIC_NAck(void);
void IIC_Write_Byte(u8 txd);
u8 IIC_Read_Byte(void);

#endif
