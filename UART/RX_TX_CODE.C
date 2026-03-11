#include "stm32f10x.h"

// UART configuration
void UART1_Config(void)
{
    // Enable clocks
    RCC->APB2ENR |= (1<<14); // USART1
    RCC->APB2ENR |= (1<<2);  // GPIOA

    // PA9 = TX
    GPIOA->CRH &= ~(0xF << 4);
    GPIOA->CRH |=  (0xB << 4);

    // PA10 = RX
    GPIOA->CRH &= ~(0xF << 8);
    GPIOA->CRH |=  (0x4 << 8);

    // Baud rate 115200
    USART1->BRR = 0x271;

    // Enable TX RX UART
    USART1->CR1 |= (1<<13);
    USART1->CR1 |= (1<<3);
    USART1->CR1 |= (1<<2);
}

// Send single character
void UART1_SendChar(char ch)
{
    while(!(USART1->SR & (1<<7))); // wait TXE
    USART1->DR = ch;
}

// Send string
void UART1_SendString(char *str)
{
    while(*str)
    {
        UART1_SendChar(*str++);
    }
}

// Receive character
char UART1_ReceiveChar(void)
{
    while(!(USART1->SR & (1<<5))); // wait RXNE
    return USART1->DR;
}

// Main program
int main(void)
{
    char data;

    UART1_Config();

    UART1_SendString("System Start\r\n");

    while(1)
    {
        data = UART1_ReceiveChar();

        UART1_SendString("Received: ");
        UART1_SendChar(data);
        UART1_SendString("\r\n");
    }
}
