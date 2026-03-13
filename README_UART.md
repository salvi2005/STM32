# STM32 UART Communication (Register-Level Implementation) 🚀

This repository demonstrates **UART communication using STM32F103 microcontroller with register-level programming**.
The goal of this project is to understand **how UART works internally without using HAL libraries**.

This README explains:

• UART fundamentals
• UART data frame
• UART registers
• Polling communication
• Interrupt-based communication
• Serial debugging
• UART error handling

---

# 1. What is UART?

UART stands for **Universal Asynchronous Receiver Transmitter**.

It is a **serial communication protocol** used for data transfer between devices.

Examples:

* Microcontroller ↔ Computer
* Microcontroller ↔ GPS module
* Microcontroller ↔ WiFi module
* Microcontroller ↔ Telemetry system

UART is **asynchronous**, meaning it does not require a shared clock.

---

# 2. UART Communication Lines

![Image](https://datacapturecontrol.com/articles/static/images/data-communication/wired/uart/intro/uart-intro-title-image.svg)

![Image](https://images.theengineeringprojects.com/image/main/2021/11/1-7.jpg)

![Image](https://europe1.discourse-cdn.com/arduino/original/4X/a/b/d/abd25471e12519a7b076948635639e21328b6391.png)

![Image](https://community.nxp.com/t5/image/serverpage/image-id/318724i706640F5CB31A35C/image-size/large?px=999\&v=v2)

UART requires **two main lines**:

| Line | Function      |
| ---- | ------------- |
| TX   | Transmit data |
| RX   | Receive data  |

Connection example:

```id="c9qg2k"
STM32 TX → PC RX
STM32 RX → PC TX
GND → GND
```

For STM32F103:

| Pin  | Function |
| ---- | -------- |
| PA9  | TX       |
| PA10 | RX       |

---

# 3. UART Data Frame

![Image](https://www.researchgate.net/publication/340062601/figure/fig24/AS%3A871193624191015%401584720154984/UART-Data-Structure-The-start-bit-is-logical-LOW-and-the-stop-bit-is-logical-HIGH-These.ppm)

![Image](https://www.researchgate.net/publication/4253336/figure/fig1/AS%3A668966062153730%401536505346066/N1-Data-Transmission.png)

![Image](https://vanhunteradams.com/Protocols/UART/uart_timing.png)

![Image](https://developer.electricimp.com/sites/default/files/attachments/images/uart/uart3.png)

UART transmits data in frames.

Frame structure:

```id="px6xov"
START | DATA BITS | PARITY | STOP
```

Most common format:

```id="8kb9df"
8N1
```

Meaning:

| Parameter | Value |
| --------- | ----- |
| Data bits | 8     |
| Parity    | None  |
| Stop bit  | 1     |

---

# 4. Baud Rate

Baud rate determines **communication speed**.

Examples:

| Baud Rate |
| --------- |
| 9600      |
| 57600     |
| 115200    |

Common debugging speed:

```id="crtn27"
115200 baud
```

Baud rate formula:

```id="m9tq4b"
USARTDIV = Fclk / (16 × BaudRate)
```

Example:

```id="th8j1b"
72MHz / (16 × 115200)
```

Result:

```id="ytn3bt"
BRR = 0x271
```

---

# 5. Important UART Registers

| Register | Purpose            |
| -------- | ------------------ |
| SR       | Status register    |
| DR       | Data register      |
| BRR      | Baud rate register |
| CR1      | Control register   |

---

# 6. Important UART Status Flags

UART communication uses **status flags**.

| Flag | Meaning                  |
| ---- | ------------------------ |
| TXE  | Transmit buffer empty    |
| RXNE | Receive buffer not empty |
| TC   | Transmission complete    |

Examples:

TXE flag:

```id="qglv1o"
Transmit register empty
```

RXNE flag:

```id="g9j8od"
Data received
```

---

# 7. UART Initialization (Register Level)

Steps performed during initialization:

1. Enable peripheral clocks
2. Configure TX and RX pins
3. Set baud rate
4. Enable transmitter and receiver

Example configuration steps:

```id="1v29j1"
Enable USART1 clock
Enable GPIOA clock
Configure PA9 TX
Configure PA10 RX
Set BRR
Enable UART
```

---

# 8. UART Polling Communication

![Image](https://user-images.githubusercontent.com/62213019/114440221-baf90a80-9b7e-11eb-8a0e-417cfbf72be0.png)

![Image](https://www.xanthium.in/sites/default/files/inline-images/8051-uart-baudrate-configuration-clock.jpg)

![Image](https://www.allelcoelec.com/upfile/images/bd/20250105123408522.png)

![Image](https://www.analog.com/en/_/media/images/analog-dialogue/en/volume-54/number-4/articles/uart-a-hardware-communication-protocol/335962-fig-02.svg?la=en\&rev=7d55981f85ba4f1fb8f2c41635303994)

Polling means:

```id="3db6pg"
CPU continuously checks status flags
```

Example:

```id="y9o8h5"
while(!(USART1->SR & TXE))
```

Flow:

```id="3v6l2f"
Check TXE
↓
Wait until empty
↓
Write data
```

---

# 9. Sending Data (Transmit)

Character transmit flow:

```id="n6qv0o"
Check TXE flag
↓
Write data to DR register
↓
Hardware sends serial data
```

Example concept:

```id="6g3j0e"
DR = 'A'
```

UART converts it to serial bits.

---

# 10. Receiving Data

Receive flow:

```id="9e4d6n"
Wait for RXNE flag
↓
Read DR register
```

Example:

```id="n27l2t"
char data = USART1->DR
```

---

# 11. UART Interrupt Mode

![Image](https://repository-images.githubusercontent.com/564580457/c8b3ebe8-7feb-4db6-a3c7-1fe8278c324b)

![Image](https://cdn.sparkfun.com/assets/e/9/7/5/4/50d24680ce395f7172000000.png)

![Image](https://users.ece.utexas.edu/~valvano/Volume1/E-Book/C12_Interrupts_files/c12-image001.png)

![Image](https://miro.medium.com/v2/resize%3Afit%3A1200/1%2ARNBYkQ193P2tODrXv82Lcw.png)

Interrupt mode removes the need for polling.

Flow:

```id="7x4t12"
Data received
↓
RXNE flag set
↓
Interrupt triggered
↓
ISR executed
```

Interrupt enable:

```id="3k0exv"
USART1->CR1 |= RXNEIE
```

ISR example:

```id="ah4cv2"
USART1_IRQHandler()
```

---

# 12. Serial Debugging

Serial debugging is widely used in embedded systems.

Example debug messages:

```id="3o1pqu"
System Boot
Sensor Init OK
Main Loop Running
```

Tools used:

* PuTTY
* TeraTerm
* Arduino Serial Monitor

Debugging helps track program execution.

---

# 13. UART Error Handling

![Image](https://community.st.com/t5/image/serverpage/image-id/64214i35C4C6F96DE71486?v=v2)

![Image](https://europe1.discourse-cdn.com/arduino/original/4X/0/2/c/02c482a50375a67e6a5f4ad5e6f7394aa56da793.png)

![Image](https://forums.ghielectronics.com/uploads/db3298/original/2X/b/bb0c4ef9e3906991a4c53a53196874c435ac6c55.jpeg)

![Image](https://community.st.com/t5/image/serverpage/image-id/83499iD51C7729575C9FB1?v=v2)

UART hardware can detect communication errors.

Important error flags:

| Error | Meaning       |
| ----- | ------------- |
| ORE   | Overrun error |
| NE    | Noise error   |
| FE    | Framing error |

Examples:

Overrun Error:

```id="34cwcc"
New data arrives before previous data is read
```

Noise Error:

```id="civqk8"
Electrical interference corrupts data
```

Framing Error:

```id="d2z0v5"
Incorrect baud rate causes invalid frame detection
```

---

# 14. Applications of UART

UART is widely used in embedded systems.

Examples:

| Device           | Use                     |
| ---------------- | ----------------------- |
| GPS module       | Navigation              |
| Bluetooth module | Wireless communication  |
| Telemetry system | Drone data transmission |
| Debug console    | Firmware debugging      |

---

# 15. Summary

This project demonstrates:

* Register-level UART programming
* Polling communication
* Interrupt-driven communication
* Serial debugging techniques
* Error detection in UART

These concepts are fundamental for **embedded firmware development**.

---

# Author

Salvi Kashyap
Embedded Systems Learner | STM32 Firmware Development

