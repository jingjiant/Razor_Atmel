
#include "configuration.h"


static fnCode_type spi_slave_pfStateMachine;               
static u8  spi_slave_au8RxBuffer[128];     //接受过程中存放51422发过来的数据
static u8* spi_slave_pu8RxBufferNextChar;  //指向spi_slave_au8RxBuffer[128]中存储下一个字节的地址               
static u8 u8SendByteNumber = 0;            //SAM3U2C向51422发送数据的长度     
static u8* CurrentTxByte;                  //指向当前需要发送的字节的地址     
static AT91PS_USART SPI_SLAVE_ = AT91C_BASE_US2; //USART2的地址

u8 spi_slave_au8Message[128];             //存放最终收到的数据
bool bNewMessage=FALSE;                   //收到消息的标志

//函数名：SpiInitialize()
//功能： 对51422进行复位，并初始化USART2的寄存器
//      使能接受中断RXRDY和发送输入变化清除中断CTSIC
void SpiInitialize(void)
{
  AT91C_BASE_PIOB->PIO_CODR = PB_21_ANT_RESET;          //对51422进行复位
  AT91C_BASE_PIOB->PIO_OER  = PB_21_ANT_RESET;
  
  spi_slave_pu8RxBufferNextChar = spi_slave_au8RxBuffer;//初始化的接受的下一个字节指向存放数据数组的首地址
  AT91C_BASE_PIOB->PIO_SODR = PB_23_ANT_MRDY;
  AT91C_BASE_PIOB->PIO_SODR = PB_24_ANT_SRDY;
  
  AT91C_BASE_PMC->PMC_PCER |= (1 << AT91C_ID_US2);
  SPI_SLAVE_->US_CR = USART2_US_CR_INIT;                //配置寄存器
  SPI_SLAVE_->US_MR = USART2_US_MR_INIT;
  SPI_SLAVE_->US_IDR = USART2_US_IDR_INIT;
  SPI_SLAVE_->US_IER = (AT91C_US_CTSIC | AT91C_US_RXRDY);
  SPI_SLAVE_->US_BRGR = USART2_US_BRGR_INIT;
  
  NVIC_ClearPendingIRQ( (IRQn_Type)AT91C_ID_US2);
  NVIC_EnableIRQ( (IRQn_Type)AT91C_ID_US2 );
  
  AT91C_BASE_PIOB->PIO_ODR = PB_21_ANT_RESET;
  
  spi_slave_pfStateMachine = SPI_SLAVE_Idle;
}


//函数名：SPI_SLAVE_Idle()
//功能： 等待cs被拉低，当cs被拉低时判断MRDY引脚状态
//       MRDY引脚为0时进入发送函数，否则进入接受函数
void SPI_SLAVE_Idle(void)
{
  if((AT91C_BASE_PIOB->PIO_PDSR & PB_22_ANT_USPI2_CS) ==0)
  {
    if((AT91C_BASE_PIOB->PIO_PDSR & PB_23_ANT_MRDY) == 0)
    {
      for(u32 i = 0; i < 100; i++);
      AT91C_BASE_PIOB->PIO_CODR = PB_24_ANT_SRDY;
      
      spi_slave_pfStateMachine = SPI_SLAVE_TX;
    }
    
    else
    {
      for(u32 i = 0; i < 100; i++);
      AT91C_BASE_PIOB->PIO_CODR = PB_24_ANT_SRDY;
      spi_slave_pfStateMachine = SPI_SLAVE_RX;
      spi_slave_pu8RxBufferNextChar = spi_slave_au8RxBuffer;
    }
  }
}



void SPI_SLAVE_RunActiveState(void)
{
  spi_slave_pfStateMachine();
}

//函数名：SPI_SLAVE_TX()
//功能：等待数据发送完成
static void SPI_SLAVE_TX(void)
{
  if(AT91C_BASE_PIOB->PIO_PDSR & PB_22_ANT_USPI2_CS)
  {
    AT91C_BASE_PIOB->PIO_SODR = PB_23_ANT_MRDY;
    spi_slave_pfStateMachine = SPI_SLAVE_Idle;
  }
}


//函数名：SPI_SLAVE_RX()
//功能：等待数据接受完成，接受完成时判断收到的第一个字节
//      是否为同步字节，如果是则将接收到的数据放入spi_slave_au8Message[128]中
//      并将新数据标志位置1
static void SPI_SLAVE_RX(void)
{
  static u8 u8Length;
  
  if(AT91C_BASE_PIOB->PIO_PDSR & PB_22_ANT_USPI2_CS)
  {
    if(spi_slave_au8RxBuffer[0]==0x5A)
    {
        u8Length = spi_slave_au8RxBuffer[1];
        u8Length += 2;
        memcpy(spi_slave_au8Message, (const u8*)spi_slave_au8RxBuffer, u8Length);
        bNewMessage = TRUE;
    }
    spi_slave_pu8RxBufferNextChar = spi_slave_au8RxBuffer;
    AT91C_BASE_PIOB->PIO_SODR = PB_24_ANT_SRDY;
    spi_slave_pfStateMachine = SPI_SLAVE_Idle;
  }
}


//函数名：SPI_SLAVE_SendData()
//功能：向51422发送数据，发送数据的格式为:
//      第一个字节为同步字节0x5A，第二个直接为长度
//      后面的字节为数据
void SPI_SLAVE_SendData(u8* Message)
{
  static u8 u8Length;
  
  u8Length = *(Message+1)+2;    //得到需要发送数据的长度
  u8SendByteNumber = u8Length;
  CurrentTxByte = Message;      //当前发送字节指向需要发送的第一个字节的地址
  SPI_SLAVE_->US_IER = AT91C_US_TXEMPTY;        //使能发送器空中断
  AT91C_BASE_PIOB->PIO_CODR = PB_23_ANT_MRDY;
  AT91C_BASE_PIOB->PIO_SODR = PB_24_ANT_SRDY;
}

//函数名：spi_slave_interupt()
//功能：当使能RXRDY和TXEMPTY后，每当接收器就绪或发送器空的时候会产生中断
//      进入当该函数执行中断处理
void spi_slave_interupt(void)
{
  
  static u32 u32Byte;
  
  if((SPI_SLAVE_->US_IMR & AT91C_US_RXRDY) && 
     (SPI_SLAVE_->US_CSR & AT91C_US_RXRDY))     //判断是否为接受中断，
  {                                             //如果是，进行接收处理
    u32Byte = 0x000000FF & SPI_SLAVE_->US_RHR;
    u32Byte = __RBIT(u32Byte)>>24;
    
    *(spi_slave_pu8RxBufferNextChar) = (u8)u32Byte;

    AT91C_BASE_PIOB->PIO_SODR = PB_24_ANT_SRDY;
    spi_slave_pu8RxBufferNextChar++;
  }
  
  
    if( (SPI_SLAVE_->US_IMR & AT91C_US_TXEMPTY) && 
      (SPI_SLAVE_->US_CSR & AT91C_US_TXEMPTY) )         //判断是否为发送中断
  {
                                                        //如果是，进行发送处理
    if(u8SendByteNumber != 0)
    {
      u8SendByteNumber--;
      u32Byte = 0x000000FF & *CurrentTxByte;
      CurrentTxByte++;
      u32Byte = __RBIT(u32Byte)>>24;
      SPI_SLAVE_->US_THR = (u8)u32Byte; 
      
      
    }
    else
    {
      SPI_SLAVE_->US_IDR = AT91C_US_TXEMPTY;
    }
    
  }
}
