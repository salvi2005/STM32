#include "stm32f10x.h"

// 1. UART Configuration with Interrupt Enable
void UART1_Interrupt_Init(void) {
    // Clock Enable: UART1(14) aur GPIOA(2)
    RCC->APB2ENR |= (1 << 14) | (1 << 2); 

    // GPIO Setup: PA9 (TX) as AF Output, PA10 (RX) as Floating Input PA8:0-3, PA9:4-7, PA10:8-11
    GPIOA->CRH &= ~(0xFF << 4);  // Pin 9 aur 10 make clean box
    GPIOA->CRH |= (0x4B << 4);   // PA9: 0xB (AF Out), PA10: 0x4 (Input Float)

    // Baud Rate: 115200 at 72MHz
    USART1->BRR = 0x271; 

    // CR1 Setup: Enable UART, TX, RX aur RXNE Interrupt
    // Bit 5 (RXNEIE) = 1 (Receive Register Not Empty Interrupt Enable)-INTRRUPT TRIGGER
    USART1->CR1 |= (1 << 13) | (1 << 3) | (1 << 2) | (1 << 5);
    //CONTROL BIT: 13-UE(UART ENABLE), 3-TE(TX ENABLE), 2-RE(RX ENABLE, 5-RXNEIE(RECIVE TNTERRUPT ENABLE)

    // NVIC Setup: UART1 Interrupt ko active karo
    NVIC_EnableIRQ(USART1_IRQn);
    NVIC_SetPriority(USART1_IRQn, 1); // Drone balancing se thodi kam priority
}

// 2. ISR (Interrupt Service Routine) - Ye function hardware call karega
void USART1_IRQHandler(void) {
    // Check karo ki kya sach mein data receive hua hai (RXNE flag)
    if (USART1->SR & (1 << 5)) {
        char received = (char)USART1->DR; // Data utha lo (Ye apne aap flag clear kar dega)
        
        // Logic: Agar laptop se '1' aaye toh Motor ON, '0' aaye toh OFF
        if (received == '1') {
            GPIOA->ODR |= (1 << 5); // Example Pin
        } else if (received == '0') {
            GPIOA->ODR &= ~(1 << 5);
        }
    }
}

int main(void) {
    UART1_Interrupt_Init();
    
    while(1) {
        // CPU yahan drone ka PID/Sensor fusion logic chalayega
        // Use UART ka intezar karne ki zaroorat nahi hai!
    }
}
