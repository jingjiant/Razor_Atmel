
#include "configuration.h"


static fnCode_type spi_slave_pfStateMachine;               
static u8  spi_slave_au8RxBuffer[128];     //���ܹ����д��51422������������
static u8* spi_slave_pu8RxBufferNextChar;  //ָ��spi_slave_au8RxBuffer[128]�д洢��һ���ֽڵĵ�ַ               
static u8 u8SendByteNumber = 0;            //SAM3U2C��51422�������ݵĳ���     
static u8* CurrentTxByte;                  //ָ��ǰ��Ҫ���͵��ֽڵĵ�ַ     
static AT91PS_USART SPI_SLAVE_ = AT91C_BASE_US2; //USART2�ĵ�ַ

u8 spi_slave_au8Message[128];             //��������յ�������
bool bNewMessage=FALSE;                   //�յ���Ϣ�ı�־

//��������SpiInitialize()
//���ܣ� ��51422���и�λ������ʼ��USART2�ļĴ���
//      ʹ�ܽ����ж�RXRDY�ͷ�������仯����ж�CTSIC
void SpiInitialize(void)
{
  AT91C_BASE_PIOB->PIO_CODR = PB_21_ANT_RESET;          //��51422���и�λ
  AT91C_BASE_PIOB->PIO_OER  = PB_21_ANT_RESET;
  
  spi_slave_pu8RxBufferNextChar = spi_slave_au8RxBuffer;//��ʼ���Ľ��ܵ���һ���ֽ�ָ��������������׵�ַ
  AT91C_BASE_PIOB->PIO_SODR = PB_23_ANT_MRDY;
  AT91C_BASE_PIOB->PIO_SODR = PB_24_ANT_SRDY;
  
  AT91C_BASE_PMC->PMC_PCER |= (1 << AT91C_ID_US2);
  SPI_SLAVE_->US_CR = USART2_US_CR_INIT;                //���üĴ���
  SPI_SLAVE_->US_MR = USART2_US_MR_INIT;
  SPI_SLAVE_->US_IDR = USART2_US_IDR_INIT;
  SPI_SLAVE_->US_IER = (AT91C_US_CTSIC | AT91C_US_RXRDY);
  SPI_SLAVE_->US_BRGR = USART2_US_BRGR_INIT;
  
  NVIC_ClearPendingIRQ( (IRQn_Type)AT91C_ID_US2);
  NVIC_EnableIRQ( (IRQn_Type)AT91C_ID_US2 );
  
  AT91C_BASE_PIOB->PIO_ODR = PB_21_ANT_RESET;
  
  spi_slave_pfStateMachine = SPI_SLAVE_Idle;
}


//��������SPI_SLAVE_Idle()
//���ܣ� �ȴ�cs�����ͣ���cs������ʱ�ж�MRDY����״̬
//       MRDY����Ϊ0ʱ���뷢�ͺ��������������ܺ���
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

//��������SPI_SLAVE_TX()
//���ܣ��ȴ����ݷ������
static void SPI_SLAVE_TX(void)
{
  if(AT91C_BASE_PIOB->PIO_PDSR & PB_22_ANT_USPI2_CS)
  {
    AT91C_BASE_PIOB->PIO_SODR = PB_23_ANT_MRDY;
    spi_slave_pfStateMachine = SPI_SLAVE_Idle;
  }
}


//��������SPI_SLAVE_RX()
//���ܣ��ȴ����ݽ�����ɣ��������ʱ�ж��յ��ĵ�һ���ֽ�
//      �Ƿ�Ϊͬ���ֽڣ�������򽫽��յ������ݷ���spi_slave_au8Message[128]��
//      ���������ݱ�־λ��1
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


//��������SPI_SLAVE_SendData()
//���ܣ���51422�������ݣ��������ݵĸ�ʽΪ:
//      ��һ���ֽ�Ϊͬ���ֽ�0x5A���ڶ���ֱ��Ϊ����
//      ������ֽ�Ϊ����
void SPI_SLAVE_SendData(u8* Message)
{
  static u8 u8Length;
  
  u8Length = *(Message+1)+2;    //�õ���Ҫ�������ݵĳ���
  u8SendByteNumber = u8Length;
  CurrentTxByte = Message;      //��ǰ�����ֽ�ָ����Ҫ���͵ĵ�һ���ֽڵĵ�ַ
  SPI_SLAVE_->US_IER = AT91C_US_TXEMPTY;        //ʹ�ܷ��������ж�
  AT91C_BASE_PIOB->PIO_CODR = PB_23_ANT_MRDY;
  AT91C_BASE_PIOB->PIO_SODR = PB_24_ANT_SRDY;
}

//��������spi_slave_interupt()
//���ܣ���ʹ��RXRDY��TXEMPTY��ÿ�������������������յ�ʱ�������ж�
//      ���뵱�ú���ִ���жϴ���
void spi_slave_interupt(void)
{
  
  static u32 u32Byte;
  
  if((SPI_SLAVE_->US_IMR & AT91C_US_RXRDY) && 
     (SPI_SLAVE_->US_CSR & AT91C_US_RXRDY))     //�ж��Ƿ�Ϊ�����жϣ�
  {                                             //����ǣ����н��մ���
    u32Byte = 0x000000FF & SPI_SLAVE_->US_RHR;
    u32Byte = __RBIT(u32Byte)>>24;
    
    *(spi_slave_pu8RxBufferNextChar) = (u8)u32Byte;

    AT91C_BASE_PIOB->PIO_SODR = PB_24_ANT_SRDY;
    spi_slave_pu8RxBufferNextChar++;
  }
  
  
    if( (SPI_SLAVE_->US_IMR & AT91C_US_TXEMPTY) && 
      (SPI_SLAVE_->US_CSR & AT91C_US_TXEMPTY) )         //�ж��Ƿ�Ϊ�����ж�
  {
                                                        //����ǣ����з��ʹ���
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
