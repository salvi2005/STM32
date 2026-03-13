# STM32 I2C Communication (Register-Level Driver) 🚀

This repository demonstrates **I2C communication on STM32F103 using bare-metal register programming**.
The goal is to clearly explain **how the I2C protocol works internally**, including:

• Initialization
• START / STOP conditions
• Address phase
• Data write / read
• Status flags
• Error handling

This type of implementation is useful for **embedded firmware engineers working without HAL libraries**.

---

# 1. What is I2C?

I2C stands for **Inter-Integrated Circuit**.
It is a **two-wire synchronous communication protocol** used to connect microcontrollers with sensors and peripherals.

Common devices using I2C:

* IMU Sensors (MPU6050)
* Barometers (BMP280)
* EEPROM
* OLED Displays

I2C allows **multiple devices on the same bus**.

---

# 2. I2C Bus Architecture

![Image](https://static.designandreuse.com/img20/20230911c_1.jpg)

![Image](https://i.sstatic.net/ZkR5L.png)

![Image](https://www.totalphase.com/media/blog/2021/02/10260243_diagram-original.jpg)

![Image](https://cdn-learn.adafruit.com/assets/assets/000/109/385/medium800/sensors_rpi_pullups.jpg?1646157336=)

The I2C bus contains two lines:

| Line    | Purpose      |
| ------- | ------------ |
| **SCL** | Serial Clock |
| **SDA** | Serial Data  |

Important points:

* The **master generates the clock**
* All devices share the same two wires
* Each slave has a **unique address**

Example bus:

STM32 → MPU6050 → BMP280 → EEPROM

---

# 3. I2C Pins on STM32F103

| Pin | Function |
| --- | -------- |
| PB6 | SCL      |
| PB7 | SDA      |

Both pins must be configured as:

```
Alternate Function Open Drain
```

Pull-up resistors are required on the I2C lines.

---

# 4. I2C Communication Sequence

![Image](https://www.analog.com/en/_/media/analog/en/landing-pages/technical-articles/i2c-primer-what-is-i2c-part-1-/36685.png?la=en\&rev=bebf30881dea46a19f86abcbd41d2e0a)

![Image](https://www.analog.com/en/_/media/analog/en/landing-pages/technical-articles/i2c-timing-definition-and-specification-guide-part-2-/36749.png?la=en\&rev=0c08f68bb87742fe95ceeb3cdef1b981\&w=900)

![Image](https://onlinedocs.microchip.com/oxy/GUID-04DAA7D3-9FC2-45F2-B757-157190106490-en-US-2/GUID-408E1711-336C-4AE3-998C-27C64BA254FC-low.png)

![Image](https://vanhunteradams.com/Protocols/I2C/repeat_start.png)

Typical communication flow:

```
START
↓
Slave Address
↓
ACK
↓
Register Address
↓
Data Transfer
↓
STOP
```

When reading a register:

```
START
↓
Slave Address (Write)
↓
Register Address
↓
REPEATED START
↓
Slave Address (Read)
↓
Receive Data
↓
STOP
```

---

# 5. Important STM32 I2C Registers

| Register | Purpose                               |
| -------- | ------------------------------------- |
| CR1      | Control register (START / STOP / ACK) |
| CR2      | Peripheral clock configuration        |
| SR1      | Status flags                          |
| SR2      | Additional status                     |
| DR       | Data register                         |
| CCR      | Clock control                         |
| TRISE    | Rise time configuration               |

These registers control the entire I2C peripheral.

---

# 6. Important Status Flags

I2C communication progresses using **status flags**.

| Flag | Meaning                   |
| ---- | ------------------------- |
| SB   | Start condition generated |
| ADDR | Address acknowledged      |
| TXE  | Transmit buffer empty     |
| RXNE | Receive buffer not empty  |
| BTF  | Byte transfer finished    |

Example event sequence:

```
START → SB flag
Address sent → ADDR flag
Data send → TXE flag
Data received → RXNE flag
```

---

# 7. Error Flags (Error Handling)

![Image](https://media.licdn.com/dms/image/v2/D4D12AQEF9jq32yXzfQ/article-cover_image-shrink_720_1280/B4DZXmN5qMG8AI-/0/1743324159707?e=2147483647\&t=EdSGLXBR_M_22o06KvZpROq-l1Gb1blj0_X4HNmFUfo\&v=beta)

![Image](https://i.sstatic.net/xhEk3.png)

![Image](https://chibiforge.org/doc/19.1/full_rm/dot_inline_dotgraph_10.png)

![Image](https://i.sstatic.net/P3joV.png)

Real embedded systems must detect errors.

Important error flags:

| Flag     | Meaning             |
| -------- | ------------------- |
| **BERR** | Bus error           |
| **ARLO** | Arbitration lost    |
| **AF**   | Acknowledge failure |
| **OVR**  | Overrun error       |

Example causes:

Bus Error → noise on the bus
ACK Failure → wrong slave address
Overrun → CPU didn't read data in time

Professional firmware always checks these flags.

---

# 8. I2C Driver Structure

A typical I2C driver contains these functions:

```
I2C_Init()
I2C_Start()
I2C_Address()
I2C_Write()
I2C_Read()
I2C_Stop()
I2C_CheckErrors()
```

This modular structure makes the driver reusable.

---

# 9. Example Sensor Read

Example: reading a register from **MPU6050**.

Device Address:

```
0x68
```

Register example:

```
WHO_AM_I = 0x75
```

Read sequence:

```
START
↓
0xD0 (Address + Write)
↓
Register Address
↓
REPEATED START
↓
0xD1 (Address + Read)
↓
Receive Data
↓
STOP
```

Expected result:

```
0x68
```

---

# 10. Applications in Embedded Systems

I2C is widely used for sensor communication.

Examples:

| Device               | Protocol |
| -------------------- | -------- |
| IMU Sensor (MPU6050) | I2C      |
| Barometer (BMP280)   | I2C      |
| Magnetometer         | I2C      |
| EEPROM               | I2C      |

In drone flight controllers, STM32 reads **gyro, accelerometer, and altitude sensors via I2C**.

---

# 11. Polling vs Interrupt vs DMA

| Method    | CPU Usage |
| --------- | --------- |
| Polling   | High      |
| Interrupt | Medium    |
| DMA       | Very Low  |

Small sensor drivers usually use **polling**, while high-speed data acquisition may use **DMA**.

---

# 12. Why This Project Matters

This project demonstrates:

* Bare-metal STM32 programming
* I2C protocol internals
* Register-level firmware development
* Embedded communication with sensors

Understanding this driver helps in building:

* Sensor interfaces
* Drone flight controllers
* Robotics systems
* Data acquisition devices

---

# 13. Author

Salvi Kashyap
Embedded Systems Learner | STM32 Firmware Development
