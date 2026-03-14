#include "stm32f10x.h"

/*
FULL FLOW:
SPI Init
↓
OLED Reset
↓
OLED Init commands
↓
Clear display
↓
Send pixel data

CODE STUCTURE: SPI1_Init()
SPI_Send()

OLED_Command()
OLED_Data()

OLED_Init()
OLED_Clear()

START CODE:
============================================
PIN CONFIGURATION
============================================

PA5 -> SCK
PA7 -> MOSI
PA4 -> CS
PA3 -> DC
PA2 -> RESET

*/

void delay()
{
    for(int i=0;i<50000;i++);
}

/*
============================================
SPI INITIALIZATION
============================================
*/

void SPI1_Init()
{
    // Enable GPIOA clock
    RCC->APB2ENR |= (1<<2);

    // Enable SPI1 clock
    RCC->APB2ENR |= (1<<12);

    // Configure SPI pins

    GPIOA->CRL &= ~(0xFFFFF << 8);

    // PA5 SCK
    GPIOA->CRL |= (0xB << 20);

    // PA7 MOSI
    GPIOA->CRL |= (0xB << 28);

    // CS
    GPIOA->CRL |= (0x3 << 16);

    // DC
    GPIOA->CRL |= (0x3 << 12);

    // RESET
    GPIOA->CRL |= (0x3 << 8);


    /*
    SPI MASTER CONFIG
    */

    SPI1->CR1 =
    (1<<2) |   // Master mode
    (1<<9) |   // Software slave management
    (1<<8) |   // Internal slave select
    (1<<6);    // SPI enable
}

/*
============================================
SPI SEND BYTE
============================================
*/

void SPI_Send(uint8_t data)
{
    while(!(SPI1->SR & (1<<1))); // TXE

    SPI1->DR = data;

    while(SPI1->SR & (1<<7)); // BSY
}

/*
============================================
OLED COMMAND
============================================
*/

void OLED_Command(uint8_t cmd)
{
    GPIOA->ODR &= ~(1<<3); // DC LOW (command)

    GPIOA->ODR &= ~(1<<4); // CS LOW

    SPI_Send(cmd);

    GPIOA->ODR |= (1<<4); // CS HIGH
}

/*
============================================
OLED DATA
============================================
*/

void OLED_Data(uint8_t data)
{
    GPIOA->ODR |= (1<<3); // DC HIGH (data)

    GPIOA->ODR &= ~(1<<4); // CS LOW

    SPI_Send(data);

    GPIOA->ODR |= (1<<4); // CS HIGH
}

/*
============================================
OLED RESET
============================================
*/

void OLED_Reset()
{
    GPIOA->ODR &= ~(1<<2);

    delay();

    GPIOA->ODR |= (1<<2);

    delay();
}

/*
============================================
OLED INITIALIZATION
============================================
*/

void OLED_Init()
{
    OLED_Reset();

    OLED_Command(0xAE); // display off

    OLED_Command(0x20); // addressing mode
    OLED_Command(0x00);

    OLED_Command(0xB0); // page start

    OLED_Command(0xC8); // COM scan direction

    OLED_Command(0x00); // low column
    OLED_Command(0x10); // high column

    OLED_Command(0x40); // start line

    OLED_Command(0x81); // contrast
    OLED_Command(0xFF);

    OLED_Command(0xA1); // segment remap

    OLED_Command(0xA6); // normal display

    OLED_Command(0xA8); // multiplex
    OLED_Command(0x3F);

    OLED_Command(0xAF); // display ON
}

/*
============================================
CLEAR DISPLAY
============================================
*/

void OLED_Clear()
{
    for(int page=0; page<8; page++)
    {
        OLED_Command(0xB0 + page);
        OLED_Command(0x00);
        OLED_Command(0x10);

        for(int col=0; col<128; col++)
        {
            OLED_Data(0x00);
        }
    }
}

/*
============================================
MAIN PROGRAM
============================================
*/

int main()
{
    SPI1_Init();

    OLED_Init();

    OLED_Clear();

    while(1)
    {
        // Example pattern

        for(int i=0;i<128;i++)
        {
            OLED_Data(0xFF); // light pixels
        }

        delay();

        OLED_Clear();
    }
}
