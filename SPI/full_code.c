#include "stm32f10x.h"

/*
====================================================
1️⃣ SPI MODE DEFINITIONS
====================================================
SPI modes define clock polarity and clock phase
*/

#define SPI_MODE0 0   // CPOL=0 CPHA=0
#define SPI_MODE1 1   // CPOL=0 CPHA=1
#define SPI_MODE2 2   // CPOL=1 CPHA=0
#define SPI_MODE3 3   // CPOL=1 CPHA=1


/*
====================================================
2️⃣ GPIO + SPI INITIALIZATION
====================================================
PA5 -> SCK
PA6 -> MISO
PA7 -> MOSI
PA4 -> CS
*/

void SPI1_Init(uint8_t mode)
{
    // Enable GPIOA clock
    RCC->APB2ENR |= (1<<2);

    // Enable SPI1 clock
    RCC->APB2ENR |= (1<<12);


    /*
    Configure SPI pins
    */

    GPIOA->CRL &= ~(0xFFFF << 20);

    // PA5 = SCK (Alternate function push pull)
    GPIOA->CRL |= (0xB << 20);

    // PA6 = MISO (Input floating)
    GPIOA->CRL |= (0x4 << 24);

    // PA7 = MOSI (Alternate function push pull)
    GPIOA->CRL |= (0xB << 28);

    // PA4 = CS (General output)
    GPIOA->CRL &= ~(0xF << 16);
    GPIOA->CRL |=  (0x3 << 16);


    /*
    SPI CONTROL REGISTER SETUP
    */

    SPI1->CR1 = 0;

    // Master mode
    SPI1->CR1 |= (1<<2);

    // Software slave management
    SPI1->CR1 |= (1<<9);
    SPI1->CR1 |= (1<<8);

    // Baud rate prescaler (slow clock for stability)
    SPI1->CR1 |= (0x3 << 3);


    /*
    SPI MODE SELECTION
    */

    if(mode == SPI_MODE1)
        SPI1->CR1 |= (1<<0);

    if(mode == SPI_MODE2)
        SPI1->CR1 |= (1<<1);

    if(mode == SPI_MODE3)
        SPI1->CR1 |= (1<<1) | (1<<0);


    // Enable SPI
    SPI1->CR1 |= (1<<6);
}


/*
====================================================
3️⃣ CHIP SELECT CONTROL
====================================================
*/

void SPI_CS_LOW()
{
    GPIOA->ODR &= ~(1<<4);
}

void SPI_CS_HIGH()
{
    GPIOA->ODR |= (1<<4);
}


/*
====================================================
4️⃣ SPI TRANSMIT (TX)
====================================================
Send single byte
*/

void SPI_TX(uint8_t data)
{
    // Wait until transmit buffer empty
    while(!(SPI1->SR & (1<<1)));   // TXE flag

    // Write data to SPI data register
    SPI1->DR = data;

    // Wait until SPI not busy
    while(SPI1->SR & (1<<7));      // BSY flag
}


/*
====================================================
5️⃣ SPI RECEIVE (RX)
====================================================
Receive single byte
*/

uint8_t SPI_RX()
{
    // Send dummy data to generate clock
    SPI1->DR = 0xFF;

    // Wait until data received
    while(!(SPI1->SR & (1<<0)));   // RXNE flag

    return SPI1->DR;
}


/*
====================================================
6️⃣ FULL DUPLEX TRANSFER
====================================================
Send and receive simultaneously
*/

uint8_t SPI_TRANSFER(uint8_t data)
{
    // Wait TXE
    while(!(SPI1->SR & (1<<1)));

    SPI1->DR = data;

    // Wait RXNE
    while(!(SPI1->SR & (1<<0)));

    return SPI1->DR;
}


/*
====================================================
7️⃣ SEND MULTIPLE BYTES
====================================================
*/

void SPI_SendBuffer(uint8_t *data, int length)
{
    int i;

    for(i=0;i<length;i++)
    {
        SPI_TX(data[i]);
    }
}


/*
====================================================
8️⃣ RECEIVE MULTIPLE BYTES
====================================================
*/

void SPI_ReadBuffer(uint8_t *buffer, int length)
{
    int i;

    for(i=0;i<length;i++)
    {
        buffer[i] = SPI_RX();
    }
}


/*
====================================================
9️⃣ MAIN PROGRAM EXAMPLE
====================================================
*/

int main()
{
    uint8_t received;
    uint8_t buffer[5];

    // Initialize SPI in MODE0
    SPI1_Init(SPI_MODE0);

    while(1)
    {
        SPI_CS_LOW();   // Select SPI device

        // Send one byte
        SPI_TX(0xA5);

        // Receive one byte
        received = SPI_RX();

        // Full duplex transfer
        received = SPI_TRANSFER(0x55);

        // Send buffer
        uint8_t data[3] = {0x11,0x22,0x33};
        SPI_SendBuffer(data,3);

        // Receive buffer
        SPI_ReadBuffer(buffer,5);

        SPI_CS_HIGH();  // Release device

        for(int i=0;i<100000;i++); // delay
    }
}
