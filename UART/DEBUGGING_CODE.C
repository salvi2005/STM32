#include "stm32f10x.h"

// UART initialization function
void UART1_Config(void)
{
    // 1️⃣ Enable clock for USART1
    RCC->APB2ENR |= (1<<14); 
    // Bit14 = USART1 clock enable

    // 2️⃣ Enable clock for GPIOA
    RCC->APB2ENR |= (1<<2);  
    // GPIOA use ho raha hai as TX/RX pins 

    // 3️⃣ Configure PA9 as TX
    GPIOA->CRH &= ~(0xF << 4); 
    // Pin9 ke bits clear kiye

    GPIOA->CRH |= (0xB << 4);  
    // 0xB = Alternate function push pull
    // Matlab UART TX output

    // 4️⃣ Configure PA10 as RX
    GPIOA->CRH &= ~(0xF << 8);
    GPIOA->CRH |= (0x4 << 8);
    // Input floating mode for RX

    // 5️⃣ Set baudrate
    USART1->BRR = 0x271; 
    // 115200 baud (72MHz clock)

    // 6️⃣ Enable UART
    USART1->CR1 |= (1<<13); 
    // UE = USART enable

    // 7️⃣ Enable transmitter
    USART1->CR1 |= (1<<3);  
    // TE = Transmitter enable
}

// Function to send one character
void UART1_SendChar(char ch)
{
    // Wait until TX buffer empty
    while(!(USART1->SR & (1<<7)));
    // Bit7 = TXE (Transmit empty)

    USART1->DR = ch;
    // Character DR register me dala
    // UART hardware usko serial line par bhejega(on wire it send)
}

// Send string function
void UART1_SendString(char *str)
{
    while(*str)          // Jab tak string khatam nahi hoti
    {
        UART1_SendChar(*str++); 
        // Ek ek character send karo
    }
}

int main(void)
{
    UART1_Config();   // UART start

    // Debug message
    UART1_SendString("System Booted\r\n");

    // Next debug checkpoint
    UART1_SendString("Initializing Sensors\r\n");

    // Suppose sensor initialized
    UART1_SendString("Sensor Init OK\r\n");

    while(1)
    {
        // Program running message
        UART1_SendString("Main Loop Running\r\n");

        for(int i=0;i<1000000;i++);
        // Delay just to slow printing
    }
}
