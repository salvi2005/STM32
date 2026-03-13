# STM32 SPI Communication with OLED (SSD1306) 🚀

This project demonstrates **SPI communication using STM32F103 microcontroller** and interfacing it with an **OLED display (SSD1306 controller)** using **register-level programming**.

The purpose of this project is to understand:

* SPI protocol fundamentals
* SPI registers in STM32
* SPI transmit and receive mechanism
* SPI modes (CPOL / CPHA)
* Chip Select control
* OLED command and data communication
* Display initialization and pixel control

This implementation avoids HAL libraries and focuses on **bare-metal embedded firmware development**.

---

# 1. What is SPI?

SPI stands for **Serial Peripheral Interface**.

It is a **high-speed synchronous serial communication protocol** used to communicate with external devices like sensors, displays, and memory chips.

SPI is widely used in embedded systems due to:

* High data speed
* Simple hardware interface
* Full-duplex communication

---

# 2. SPI Communication Lines

![Image](https://www.analog.com/en/_/media/images/analog-dialogue/en/volume-52/number-3/articles/introduction-to-spi-interface/205973_fig_01.svg?la=en\&rev=f03bda0b77f94822a05ed2e7e8f8070d\&sc_lang=en)

![Image](https://www.digikey.com/maker-media/1fee0785-6250-461e-b972-f077d6dc04e4)

![Image](https://udamonic.com/uploads/1/2/1/3/121343112/spi-orig_orig.jpg)

![Image](https://media.licdn.com/dms/image/v2/D5612AQGPMiM0bpwnKQ/article-cover_image-shrink_600_2000/article-cover_image-shrink_600_2000/0/1716104470697?e=2147483647\&t=a7l68aZmeeKtlO0FMQ6LhDzAwbbKy0u2m93dUqncm-4\&v=beta)

SPI communication uses **four wires**:

| Line    | Meaning             |
| ------- | ------------------- |
| MOSI    | Master Out Slave In |
| MISO    | Master In Slave Out |
| SCK     | Serial Clock        |
| CS / SS | Chip Select         |

Example connection:

```text
STM32 MOSI → OLED SDA
STM32 SCK  → OLED SCL
STM32 CS   → OLED CS
```

---

# 3. Master and Slave Concept

| Device       | Role   |
| ------------ | ------ |
| STM32        | Master |
| OLED Display | Slave  |

The **master device controls communication** by generating the clock signal.

Communication sequence:

```text
CS LOW
↓
Clock starts
↓
Data transmitted
↓
CS HIGH
```

---

# 4. SPI Modes

SPI communication depends on two parameters:

* Clock Polarity (CPOL)
* Clock Phase (CPHA)

| Mode   | CPOL | CPHA |
| ------ | ---- | ---- |
| Mode 0 | 0    | 0    |
| Mode 1 | 0    | 1    |
| Mode 2 | 1    | 0    |
| Mode 3 | 1    | 1    |

Most OLED displays use:

```text
SPI Mode 0
```

---

# 5. STM32 SPI Pins (SPI1)

| Pin | Function |
| --- | -------- |
| PA5 | SCK      |
| PA6 | MISO     |
| PA7 | MOSI     |
| PA4 | CS       |

Additional OLED pins:

| Pin   | Purpose             |
| ----- | ------------------- |
| DC    | Command/Data select |
| RESET | Display reset       |

---

# 6. Important SPI Registers

| Register | Function                      |
| -------- | ----------------------------- |
| CR1      | SPI configuration             |
| CR2      | Interrupt / DMA configuration |
| SR       | Status register               |
| DR       | Data register                 |

These registers control the entire SPI peripheral.

---

# 7. SPI Status Flags

| Flag | Meaning                  |
| ---- | ------------------------ |
| TXE  | Transmit buffer empty    |
| RXNE | Receive buffer not empty |
| BSY  | SPI busy                 |

Typical transmission flow:

```text
TXE = 1
↓
Write data to DR
↓
Data transmitted
↓
RXNE = 1
↓
Read DR
```

---

# 8. SPI Data Transfer

![Image](https://community.st.com/t5/image/serverpage/image-id/51966iB7140F9DEE52945F?v=v2)

![Image](https://developer.electricimp.com/sites/default/files/2020-06/SPI.Diagrams.002.jpeg)

![Image](https://europe1.discourse-cdn.com/arduino/original/4X/f/f/a/ffa06b03e1196509bea3181f8f0c6f339bc5c504.png)

![Image](https://www.avrfreaks.net/servlet/rtaImage?eid=a583l00000012ts\&feoid=00N3l00000FRqlI\&refid=0EM3l000008mudk)

SPI transfers data using **shift registers**.

When the master sends data via MOSI, the slave simultaneously sends data via MISO.

This is called:

```text
Full Duplex Communication
```

---

# 9. OLED Display Overview

![Image](https://components101.com/sites/default/files/component_pin/Monochrome-OLED-display-Pinout.jpg)

![Image](https://images.openai.com/static-rsc-3/ZCazAc6ZozN7Gy2dINI8J3gEoY3WdJGt86LY5WRcdeEsqU9PJEro-RbfmRYtD3BObsra2JjJ14Lkv_0oClWKV2aTyrITqBsObDPV9voC6s4?purpose=fullsize\&v=1)

![Image](https://wiki.sunfounder.cc/images/1/17/Rer.png)

![Image](https://abacasstorageaccnt.blob.core.windows.net/cirkit/ed3f6d58-3092-4102-8011-fabab1c44ddc.png)

OLED = **Organic Light Emitting Diode Display**

Common features:

| Feature    | Value     |
| ---------- | --------- |
| Resolution | 128 × 64  |
| Controller | SSD1306   |
| Interface  | SPI / I2C |

Applications:

* Sensor display
* Drone telemetry
* Embedded UI
* Debug output

---

# 10. OLED Command vs Data

OLED communication has two types of transmissions.

| Type    | Purpose           |
| ------- | ----------------- |
| Command | Configure display |
| Data    | Pixel data        |

Control signal:

```text
DC = 0 → Command
DC = 1 → Data
```

Example commands:

| Command | Meaning      |
| ------- | ------------ |
| 0xAE    | Display OFF  |
| 0xAF    | Display ON   |
| 0x20    | Address mode |

---

# 11. OLED Initialization Sequence

Typical initialization commands:

```text
Display OFF
↓
Set addressing mode
↓
Configure scan direction
↓
Set contrast
↓
Display ON
```

Example commands:

```text
0xAE
0x20
0xA1
0xAF
```

These commands prepare the display for pixel data.

---

# 12. OLED Pixel Data Concept

The OLED screen consists of **pixels arranged in pages**.

Display size:

```text
128 columns × 64 rows
```

Each byte controls **8 vertical pixels**.

Example pixel values:

| Data | Result     |
| ---- | ---------- |
| 0x00 | Pixels OFF |
| 0xFF | Pixels ON  |

---

# 13. Clearing the Display

To clear the display:

1. Set page address
2. Set column address
3. Send zeros

Example logic:

```text
for each page
    for each column
        send 0x00
```

This turns off all pixels.

---

# 14. Example Display Operation

Example display sequence:

```text
Initialize SPI
↓
Reset OLED
↓
Send OLED initialization commands
↓
Clear display
↓
Send pixel data
```

Example output:

```text
HELLO
TEMP: 27C
BATTERY: 11.8V
```

---

# 15. Embedded System Applications

SPI + OLED is widely used in:

| Device                  | Application       |
| ----------------------- | ----------------- |
| Drone flight controller | Telemetry display |
| Weather station         | Sensor readings   |
| Robotics                | Status display    |
| IoT devices             | Debug information |

Example drone display:

```text
Pitch: 2.1°
Roll: -0.8°
Altitude: 15m
Battery: 11.7V
```

---

# 16. Learning Outcomes

This project demonstrates:

* SPI protocol fundamentals
* Register-level SPI programming
* SPI data transfer mechanisms
* OLED command/data communication
* Embedded display control

Understanding this project helps in developing:

* Sensor interfaces
* Embedded user interfaces
* Real-time telemetry systems

---

# Author

Salvi Kashyap
Embedded Systems Learner
STM32 Firmware Development
