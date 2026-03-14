#include "stm32f10x.h"

/*
USES: SPI protocol
SPI modes
SPI registers
SPI TX
SPI RX
SPI full duplex
SPI interrupt concept
SPI DMA concept
SPI OLED interface

CODE STRUCTURE: SPI_Init()
SPI_TX()
SPI_RX()
SPI_TRANSFER()

SPI_INTERRUPT()
SPI_DMA()

OLED_Interface()

START:
=====================================================
SPI MODES
=====================================================

Mode 0 : CPOL=0 CPHA=0
Mode 1 : CPOL=0 CPHA=1
Mode 2 : CPOL=1 CPHA=0
Mode 3 : CPOL=1 CPHA=1
*/

#define SPI_MODE0 0
#define SPI_MODE1 1
#define SPI_MODE2 2
#define SPI_MODE3 3


/*
=====================================================
GPIO + SPI INITIALIZATION
=====================================================
PA5 → SCK
PA6 → MISO
PA7 → MOSI
PA4 → CS
PA3 → DC (OLED command/data)
*/

void SPI1_Init(uint8_t mode)
{
    // Enable clock for GPIOA
    RCC->APB2ENR |= (1<<2);

    // Enable clock for SPI1 peripheral
    RCC->APB2ENR |= (1<<12);

    /*
    Configure SPI pins
    */

    GPIOA->CRL &= ~(0xFFFF << 20);

    // PA5 = SCK (Alternate Function Push Pull)
    GPIOA->CRL |= (0xB << 20);

    // PA6 = MISO (Input floating)
    GPIOA->CRL |= (0x4 << 24);

    // PA7 = MOSI (Alternate Function Push Pull)
    GPIOA->CRL |= (0xB << 28);

    // PA4 = CS output
    GPIOA->CRL &= ~(0xF << 16);
    GPIOA->CRL |=  (0x3 << 16);

    // PA3 = DC output
    GPIOA->CRL &= ~(0xF << 12);
    GPIOA->CRL |=  (0x3 << 12);


    /*
    SPI CONTROL REGISTER CONFIGURATION
    */

    SPI1->CR1 = 0;

    // Master mode
    SPI1->CR1 |= (1<<2);

    // Software slave management
    SPI1->CR1 |= (1<<9);
    SPI1->CR1 |= (1<<8);

    // Baud rate prescaler
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
=====================================================
CHIP SELECT CONTROL
=====================================================
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
=====================================================
SPI TRANSMIT FUNCTION
=====================================================
*/

void SPI_TX(uint8_t data)
{
    // Wait until TX buffer empty
    while(!(SPI1->SR & (1<<1)));   // TXE flag

    // Write data to SPI data register
    SPI1->DR = data;

    // Wait until SPI not busy
    while(SPI1->SR & (1<<7));      // BSY flag
}


/*
=====================================================
SPI RECEIVE FUNCTION
=====================================================
*/

uint8_t SPI_RX()
{
    // Send dummy byte to generate clock
    SPI1->DR = 0xFF;

    // Wait until data received
    while(!(SPI1->SR & (1<<0)));   // RXNE flag

    return SPI1->DR;
}


/*
=====================================================
SPI FULL DUPLEX TRANSFER
=====================================================
Send and receive simultaneously
*/

uint8_t SPI_TRANSFER(uint8_t data)
{
    while(!(SPI1->SR & (1<<1)));   // TXE

    SPI1->DR = data;

    while(!(SPI1->SR & (1<<0)));   // RXNE

    return SPI1->DR;
}


/*
=====================================================
SPI INTERRUPT ENABLE
=====================================================
*/

void SPI1_EnableInterrupt()
{
    // Enable TXE interrupt
    SPI1->CR2 |= (1<<7);

    // Enable SPI interrupt in NVIC
    NVIC_EnableIRQ(SPI1_IRQn);
}


/*
=====================================================
SPI INTERRUPT HANDLER
=====================================================
*/

void SPI1_IRQHandler()
{
    // If TXE interrupt occurs
    if(SPI1->SR & (1<<1))
    {
        // Example interrupt action
        SPI1->DR = 0x55;
    }
}


/*
=====================================================
SPI DMA CONCEPT
=====================================================
Enable SPI TX DMA
*/

void SPI_EnableDMA()
{
    // Enable DMA request for TX
    SPI1->CR2 |= (1<<1);
}


/*
=====================================================
OLED COMMAND
=====================================================
*/

void OLED_Command(uint8_t cmd)
{
    GPIOA->ODR &= ~(1<<3); // DC LOW → command

    SPI_CS_LOW();

    SPI_TX(cmd);

    SPI_CS_HIGH();
}


/*
=====================================================
OLED DATA
=====================================================
*/

void OLED_Data(uint8_t data)
{
    GPIOA->ODR |= (1<<3); // DC HIGH → data

    SPI_CS_LOW();

    SPI_TX(data);

    SPI_CS_HIGH();
}


/*
=====================================================
OLED INITIALIZATION
=====================================================
*/

void OLED_Init()
{
    OLED_Command(0xAE); // display OFF
    OLED_Command(0x20); // addressing mode
    OLED_Command(0x00);

    OLED_Command(0xB0);
    OLED_Command(0xC8);

    OLED_Command(0xA1);
    OLED_Command(0xAF); // display ON
}


/*
=====================================================
MAIN PROGRAM
=====================================================
*/

int main()
{
    uint8_t data;

    // Initialize SPI mode0
    SPI1_Init(SPI_MODE0);

    // Enable interrupt example
    SPI1_EnableInterrupt();

    // Enable DMA example
    SPI_EnableDMA();

    // Initialize OLED display
    OLED_Init();

    while(1)
    {
        SPI_CS_LOW();

        // send byte
        SPI_TX(0xAA);

        // receive byte
        data = SPI_RX();

        // full duplex transfer
        data = SPI_TRANSFER(0x55);

        SPI_CS_HIGH();

        // send pixel data to OLED
        OLED_Data(0xFF);
    }
}
