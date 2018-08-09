
#include "configuration.h"
extern SspPeripheralType* SSP_psCurrentISR; 

static fnCode_type spi_slave_pfStateMachine;               
static SspConfigurationType spi_slave_sSspConfig;        
static SspPeripheralType* spi_slave_Ssp;                  

static u8  spi_slave_au8RxBuffer[128];     
static u8* spi_slave_pu8RxBufferNextChar;                 

static u8 u8SendByteNumber = 0;
static u8* CurrentTxByte;

u8 spi_slave_au8Message[128];       

bool bNewMessage=FALSE;

static AT91PS_USART SPI_SLAVE_ = AT91C_BASE_US2;

void SpiInitialize(void)
{
  static u8 au8Start[]={0x5A,1,0x81};
  AT91C_BASE_PIOB->PIO_CODR = PB_21_ANT_RESET;
  AT91C_BASE_PIOB->PIO_OER  = PB_21_ANT_RESET;
  
  spi_slave_pu8RxBufferNextChar = spi_slave_au8RxBuffer;
  AT91C_BASE_PIOB->PIO_SODR = PB_23_ANT_MRDY;
  AT91C_BASE_PIOB->PIO_SODR = PB_24_ANT_SRDY;
  
  /*spi_slave_sSspConfig.SspPeripheral      = BLE_SPI;
  spi_slave_sSspConfig.pCsGpioAddress     = BLE_SPI_CS_GPIO;
  spi_slave_sSspConfig.u32CsPin           = BLE_SPI_CS_PIN;
  spi_slave_sSspConfig.eBitOrder          = LSB_FIRST;
  spi_slave_sSspConfig.eSspMode           = SPI_SLAVE_FLOW_CONTROL;
  spi_slave_sSspConfig.fnSlaveTxFlowCallback = SPI_TxFlowControlCallback;
  spi_slave_sSspConfig.fnSlaveRxFlowCallback = SPI_RxFlowControlCallback;
  spi_slave_sSspConfig.pu8RxBufferAddress = spi_slave_au8RxBuffer;
  spi_slave_sSspConfig.ppu8RxNextByte     = &spi_slave_pu8RxBufferNextChar;
  spi_slave_sSspConfig.u16RxBufferSize    = U8_NRF_BUFFER_SIZE;

  spi_slave_Ssp = SspRequest(&spi_slave_sSspConfig);*/
  
  CurrentTxByte = au8Start;
  u8SendByteNumber = 0;
  AT91C_BASE_PMC->PMC_PCER |= (1 << AT91C_ID_US2);
  SPI_SLAVE_->US_CR = USART2_US_CR_INIT;
  SPI_SLAVE_->US_MR = USART2_US_MR_INIT;
  SPI_SLAVE_->US_IDR = USART2_US_IDR_INIT;
  SPI_SLAVE_->US_IER = (AT91C_US_CTSIC | AT91C_US_RXRDY);
  SPI_SLAVE_->US_BRGR = USART2_US_BRGR_INIT;
  
  NVIC_ClearPendingIRQ( (IRQn_Type)AT91C_ID_US2);
  NVIC_EnableIRQ( (IRQn_Type)AT91C_ID_US2 );
  
  AT91C_BASE_PIOB->PIO_ODR = PB_21_ANT_RESET;
  
  spi_slave_pfStateMachine = SPI_SLAVE_Idle;
}



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

static void SPI_SLAVE_TX(void)
{
  if(AT91C_BASE_PIOB->PIO_PDSR & PB_22_ANT_USPI2_CS)
  {
    AT91C_BASE_PIOB->PIO_SODR = PB_23_ANT_MRDY;
    spi_slave_pfStateMachine = SPI_SLAVE_Idle;
  }
}



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



void SPI_SLAVE_SendData(u8* Message)
{
  static u8 u8Length;
  
  u8Length = *(Message+1)+2;
  //SspWriteData(spi_slave_Ssp,u8Length,Message);
  u8SendByteNumber = u8Length;
  CurrentTxByte = Message;
  SPI_SLAVE_->US_IER = AT91C_US_TXEMPTY;
  AT91C_BASE_PIOB->PIO_CODR = PB_23_ANT_MRDY;
  AT91C_BASE_PIOB->PIO_SODR = PB_24_ANT_SRDY;
}

void spi_slave_interupt(void)
{
  
  static u32 u32Byte;
  
  if((SPI_SLAVE_->US_IMR & AT91C_US_RXRDY) && 
     (SPI_SLAVE_->US_CSR & AT91C_US_RXRDY))
  {
    u32Byte = 0x000000FF & SPI_SLAVE_->US_RHR;
    u32Byte = __RBIT(u32Byte)>>24;
    
    *(spi_slave_pu8RxBufferNextChar) = (u8)u32Byte;

    AT91C_BASE_PIOB->PIO_SODR = PB_24_ANT_SRDY;
    spi_slave_pu8RxBufferNextChar++;
  }
  
  
    if( (SPI_SLAVE_->US_IMR & AT91C_US_TXEMPTY) && 
      (SPI_SLAVE_->US_CSR & AT91C_US_TXEMPTY) )
  {
    
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
      //SPI_TxFlowControlCallback();
    }
    
  }
}
