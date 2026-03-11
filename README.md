# STM32 UART Communication (Register Level) 🚀

This project demonstrates **UART communication using STM32F103 microcontroller** with **register-level programming**.
The goal of this repository is to clearly explain:

* UART initialization
* Serial debugging
* Polling-based UART communication
* Sending and receiving data

This project is useful for **embedded systems beginners** and those learning **bare-metal STM32 programming**.

---

# 1. What is UART?

UART stands for **Universal Asynchronous Receiver Transmitter**.

It is a **serial communication protocol** used to exchange data between devices.

Examples:

* Microcontroller ↔ Computer
* Microcontroller ↔ Sensor
* Microcontroller ↔ WiFi module
* Microcontroller ↔ GPS module

UART communication requires **two lines**:

| Line | Purpose       |
| ---- | ------------- |
| TX   | Transmit data |
| RX   | Receive data  |

For STM32F103:

| Pin  | Function |
| ---- | -------- |
| PA9  | UART TX  |
| PA10 | UART RX  |

Connection example:

STM32 TX → USB-TTL RX
STM32 RX → USB-TTL TX
GND → GND

---

# 2. UART Data Frame

UART sends data in the form of a **frame**.

Frame structure:

START BIT → DATA BITS → STOP BIT

Common configuration:

8N1

Meaning:

8 data bits
No parity
1 stop bit

---

# 3. Baud Rate

Baud rate defines the **communication speed**.

Example baud rates:

* 9600
* 57600
* 115200

In this project:

Baud rate = **115200**

---

# 4. Important UART Registers

The following registers are used in this project:

| Register | Purpose            |
| -------- | ------------------ |
| SR       | Status Register    |
| DR       | Data Register      |
| BRR      | Baud Rate Register |
| CR1      | Control Register   |

Important bits:

TXE → Transmit data register empty
RXNE → Receive data register not empty

---

# 5. UART Initialization

The UART initialization function performs these steps:

1. Enable clock for USART1
2. Enable clock for GPIOA
3. Configure TX pin (PA9)
4. Configure RX pin (PA10)
5. Set baud rate
6. Enable transmitter and receiver

Example initialization:

```c
void UART1_Config(void)
```

This prepares the microcontroller for UART communication.

---

# 6. Serial Debugging

Serial debugging is a very common technique used in **embedded systems development**.

Instead of using complex debugging tools, developers send **debug messages through UART**.

Example debug messages:

```
System Booted
Sensor Init OK
Main Loop Running
```

These messages appear in a **serial terminal** such as:

* PuTTY
* TeraTerm
* Arduino Serial Monitor

This helps developers understand:

* Whether the program started correctly
* If sensors initialized successfully
* Where an error occurs

---

# 7. Polling Concept

Polling means the **CPU continuously checks a status flag**.

Example:

```
while(!(USART1->SR & (1<<7)));
```

Explanation:

The CPU repeatedly checks the **TXE flag** until the transmit buffer becomes empty.

Flow:

CPU checks flag → flag not ready → wait → check again → send data.

Polling is simple but **not efficient for large systems**, because the CPU stays busy checking the flag.

---

# 8. Sending Data using UART

To send a character:

```
UART1_SendChar('A');
```

The function waits until the TX buffer becomes empty and then places the data into the **Data Register (DR)**.

---

# 9. Sending Strings

To send a complete message:

```
UART1_SendString("Hello World");
```

The function sends characters **one by one** using the UART transmit function.

---

# 10. Receiving Data

UART can also receive data from the serial terminal.

Example receive function:

```
char data = UART1_ReceiveChar();
```

The function waits until **RXNE flag becomes 1**, indicating new data has arrived.

---

# 11. Example Output

If the user types:

```
A
```

The program responds:

```
You Typed: A
```

---

# 12. Why This Project is Important

This project helps understand:

* Bare-metal register programming
* UART communication fundamentals
* Debugging embedded systems
* Polling-based communication

These concepts are widely used in **embedded firmware development**.

---

# 13. Future Improvements

Possible upgrades to this project:

* UART interrupt mode
* DMA based communication
* printf redirection
* sensor data logging
* telemetry systems for drones

---

# 14. Author

Salvi Kashyap
Embedded Systems Enthusiast
