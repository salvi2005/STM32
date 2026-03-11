#include "stm32f10x.h"

// 1. UART Configuration Function
void UART1_Config(void) {
    // Clock Enable: UART1 and Port A dono
    RCC->APB2ENR |= (1 << 14) | (1 << 2); 

    // GPIO Setup: PA9 (TX) as 'Alternate Function Output'
    // 0xB mean: Output 50MHz + Push-Pull AF
    GPIOA->CRH &= ~(0xF << 4); //hexa
    GPIOA->CRH |= (0xB << 4); 

    // Baud Rate: at 72MHz clock par 115200 speed ke liye 0x271
    USART1->BRR = 0x271; 

    // UART Enable: Transmitter (TE) and Receiver (RE) ON 
    USART1->CR1 |= (1 << 13) | (1 << 3) | (1 << 2);
}

// 2. Ek character bhejne ka function
void UART1_SendChar(char ch) {
    // Wait karo jab tak Transmit Buffer khali na ho (TXE bit) 
    while (!(USART1->SR & (1 << 7)));
    USART1->DR = ch;
}

// 3. Poori line as a (send String) bhejne ka function
void UART1_SendString(char *str) {
    while (*str) {
        UART1_SendChar(*str++); // Ek-ek karke saare characters send hoge
    }
}

int main(void) {
    UART1_Config();
    
    UART1_SendString("Drone System Initialized...\n"); [cite: 01]
    
    while(1) {
        // Yahan main logic in loop
    }
}
