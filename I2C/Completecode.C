#include "stm32f10x.h"

//--------------------------------------------------
// 1️⃣ I2C Initialization
//--------------------------------------------------

void I2C1_Init(void)
{
    // GPIOB clock enable (I2C pins yaha par hain)
    RCC->APB2ENR |= (1<<3);

    // I2C1 peripheral clock enable
    RCC->APB1ENR |= (1<<21);

    // PB6 = SCL , PB7 = SDA
    // Inko Alternate Function Open Drain mode me set karna padta hai

    GPIOB->CRL &= ~(0xFF << 24);   // PB6 aur PB7 bits clear
    GPIOB->CRL |=  (0xFF << 24);   // AF Open Drain mode set

    // CR2 register me peripheral clock frequency set karte hain
    // APB1 clock = 36MHz
    I2C1->CR2 = 36;
    // I2C clock speed set karna
    // 100kHz standard mode
    I2C1->CCR = 180;
    // Rise time configure
    I2C1->TRISE = 37;
    // I2C enable
    I2C1->CR1 |= (1<<0);
}

 //--------------------------------------------------
 // 2️⃣ I2C START Condition
 //--------------------------------------------------

void I2C_Start(void)
{
    // START bit set
    I2C1->CR1 |= (1<<8);

    // Wait until start condition generated
    while(!(I2C1->SR1 & (1<<0)));
}

 //--------------------------------------------------
 // 3️⃣ Send Slave Address
 //--------------------------------------------------

void I2C_Address(uint8_t address)
{
    // DR register me slave address send
    I2C1->DR = address;

    // Wait until address transmitted
    while(!(I2C1->SR1 & (1<<1)));

    // SR1 aur SR2 read karna zaroori hai
    // taki ADDR flag clear ho jaye
    volatile int temp = I2C1->SR1;
    temp = I2C1->SR2;
}

 //--------------------------------------------------
 // 4️⃣ Write Data
 //--------------------------------------------------

void I2C_Write(uint8_t data)
{
    // DR register me data send
    I2C1->DR = data;

    // Wait until data transmitted
    while(!(I2C1->SR1 & (1<<7)));
}

 //--------------------------------------------------
 // 5️⃣ Read Data
 //--------------------------------------------------

uint8_t I2C_Read(void)
{
    // Wait until data received
    while(!(I2C1->SR1 & (1<<6)));

    // DR register se data read
    return I2C1->DR;
}

 //--------------------------------------------------
 // 6️⃣ STOP Condition
 //--------------------------------------------------

void I2C_Stop(void)
{
    // STOP bit set
    I2C1->CR1 |= (1<<9);
}

 //--------------------------------------------------
 // 7️⃣ Example Function: Sensor Register Write
 //--------------------------------------------------

void I2C_WriteRegister(uint8_t deviceAddr, uint8_t reg, uint8_t data)
{
    I2C_Start();               // Communication start

    I2C_Address(deviceAddr);   // Device address send

    I2C_Write(reg);            // Register address send

    I2C_Write(data);           // Data write

    I2C_Stop();                // Communication stop
}

 //--------------------------------------------------
 // 8️⃣ Example Function: Sensor Register Read
 //--------------------------------------------------

uint8_t I2C_ReadRegister(uint8_t deviceAddr, uint8_t reg)
{
    uint8_t data;

    I2C_Start();

    // Write mode address
    I2C_Address(deviceAddr);

    // Register address send
    I2C_Write(reg);

    // Restart condition
    I2C_Start();

    // Read mode address
    I2C_Address(deviceAddr | 1);

    // Data read
    data = I2C_Read();

    I2C_Stop();

    return data;
}

 //--------------------------------------------------
 // 9️⃣ Main Function Example
 //--------------------------------------------------

int main(void)
{
    uint8_t sensorData;

    // I2C initialize
    I2C1_Init();

    while(1)
    {
        // Example: MPU6050 register read
        sensorData = I2C_ReadRegister(0xD0, 0x75);

        for(int i=0;i<100000;i++); // small delay
    }
}
