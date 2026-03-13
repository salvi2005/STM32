#include "stm32f10x.h"

/*
-------------------------------------------------------
1️⃣ I2C1 Initialization
-------------------------------------------------------
Configure GPIO + I2C peripheral
PB6 → SCL
PB7 → SDA
*/

void I2C1_Init(void)
{
    // Enable clock for GPIOB
    RCC->APB2ENR |= (1<<3);

    // Enable clock for I2C1 peripheral
    RCC->APB1ENR |= (1<<21);

    // Configure PB6 & PB7 as Alternate Function Open Drain
    // Required for I2C communication
    GPIOB->CRL &= ~(0xFF<<24);
    GPIOB->CRL |=  (0xFF<<24);

    // Peripheral clock frequency (APB1 = 36 MHz)
    I2C1->CR2 = 36;

    // Configure I2C speed (100kHz)
    I2C1->CCR = 180;

    // Maximum rise time
    I2C1->TRISE = 37;

    // Enable I2C peripheral
    I2C1->CR1 |= (1<<0);
}

/*
-------------------------------------------------------
2️⃣ I2C Error Check Function
-------------------------------------------------------
Check important error flags
*/

void I2C_CheckErrors(void)
{
    // Bus Error
    if(I2C1->SR1 & (1<<8))
    {
        I2C1->SR1 &= ~(1<<8);
    }

    // Arbitration Lost
    if(I2C1->SR1 & (1<<9))
    {
        I2C1->SR1 &= ~(1<<9);
    }

    // ACK Failure
    if(I2C1->SR1 & (1<<10))
    {
        I2C1->SR1 &= ~(1<<10);
    }

    // Overrun Error
    if(I2C1->SR1 & (1<<11))
    {
        I2C1->SR1 &= ~(1<<11);
    }
}

/*
-------------------------------------------------------
3️⃣ START Condition
-------------------------------------------------------
Generate START and wait for SB flag
*/

void I2C_Start(void)
{
    // Generate START condition
    I2C1->CR1 |= (1<<8);

    // Wait until Start Bit flag set
    while(!(I2C1->SR1 & (1<<0)));

    // SB flag indicates START generated
}

/*
-------------------------------------------------------
4️⃣ Send Slave Address
-------------------------------------------------------
*/

void I2C_Address(uint8_t address)
{
    // Send slave address
    I2C1->DR = address;

    // Wait until ADDR flag set
    while(!(I2C1->SR1 & (1<<1)));

    // Clear ADDR flag
    volatile int temp = I2C1->SR1;
    temp = I2C1->SR2;
}

/*
-------------------------------------------------------
5️⃣ Write Data
-------------------------------------------------------
*/

void I2C_Write(uint8_t data)
{
    // Wait until TXE flag (data register empty)
    while(!(I2C1->SR1 & (1<<7)));

    // Send data
    I2C1->DR = data;
}

/*
-------------------------------------------------------
6️⃣ Read Data
-------------------------------------------------------
*/

uint8_t I2C_Read(void)
{
    // Wait until RXNE flag (data received)
    while(!(I2C1->SR1 & (1<<6)));

    return I2C1->DR;
}

/*
-------------------------------------------------------
7️⃣ STOP Condition
-------------------------------------------------------
*/

void I2C_Stop(void)
{
    // Generate STOP
    I2C1->CR1 |= (1<<9);
}

/*
-------------------------------------------------------
8️⃣ Read Register from Sensor
Example: MPU6050 register read
-------------------------------------------------------
*/

uint8_t I2C_ReadRegister(uint8_t deviceAddr, uint8_t regAddr)
{
    uint8_t data;

    // Check errors before communication
    I2C_CheckErrors();

    // START condition
    I2C_Start();

    // Send device address (WRITE mode)
    I2C_Address(deviceAddr<<1);

    // Send register address
    I2C_Write(regAddr);

    // Repeated START
    I2C_Start();

    // Send device address (READ mode)
    I2C_Address((deviceAddr<<1) | 1);

    // Disable ACK (only one byte read)
    I2C1->CR1 &= ~(1<<10);

    // STOP condition
    I2C_Stop();

    // Read received data
    data = I2C_Read();

    return data;
}

/*
-------------------------------------------------------
9️⃣ Example Main Program
-------------------------------------------------------
*/

int main(void)
{
    uint8_t sensorData;

    // Initialize I2C
    I2C1_Init();

    while(1)
    {
        // Example: Read MPU6050 WHO_AM_I register
        sensorData = I2C_ReadRegister(0x68,0x75);

        for(int i=0;i<100000;i++); // small delay
    }
}
