
#ifndef SPI_H
#define SPI_H


#define U8_NRF_BUFFER_SIZE      (u8)128 


void SPI_TxFlowControlCallback(void);
void SPI_RxFlowControlCallback(void);
void SPI_SLAVE_SendData(u8* Message);
void SPI_SLAVE_RunActiveState(void);

void SpiInitialize(void);
static void SPI_SLAVE_Idle(void); 

static void CS_ASSERT(void);

static void SPI_SLAVE_RX(void);
static void SPI_SLAVE_TX(void);

void spi_slave_interupt(void);
#endif