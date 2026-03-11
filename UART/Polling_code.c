#include "stm32f10x.h"

void UART1_Config(void)
{
    RCC->APB2ENR |= (1<<14); // USART1 clock
    RCC->APB2ENR |= (1<<2);  // GPIOA clock

    // TX pin PA9
    GPIOA->CRH &= ~(0xF<<4);
    GPIOA->CRH |= (0xB<<4);

    // RX pin PA10
    GPIOA->CRH &= ~(0xF<<8);
    GPIOA->CRH |= (0x4<<8);

    USART1->BRR = 0x271;  // 115200 baud

    USART1->CR1 |= (1<<13); // USART enable
    USART1->CR1 |= (1<<3);  // TX enable
    USART1->CR1 |= (1<<2);  // RX enable
}

// Send character (Polling)
void UART_SendChar(char ch)
{
    // Poll TXE bit
    while(!(USART1->SR & (1<<7)));
    // CPU check karta rahega jab tak TXE=1 na ho

    USART1->DR = ch;
    // Data send
}

// Receive character (Polling)
char UART_ReceiveChar(void)
{
    // Poll RXNE bit
    while(!(USART1->SR & (1<<5)));
    // CPU wait karta hai jab tak data receive na ho

    return USART1->DR;
}

// Send string
void UART_SendString(char *str)
{
    while(*str)
    {
        UART_SendChar(*str++);
    }
}

int main(void)
{
    char data;

    UART1_Config();

    UART_SendString("UART Polling Mode\r\n");

    while(1)
    {
        data = UART_ReceiveChar();
        // User ne terminal me jo type kiya wahi receive hua

        UART_SendString("You Typed: ");

        UART_SendChar(data);

        UART_SendString("\r\n");
    }
}
