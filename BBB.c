
#include <HDC2021DEBT.h>
#include "driverlib.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>


#define INTERRUPT_DRDY 0x04
#define INTERRUPT_CONFIG 0x07



//Define Register Map
#define TEMP_LOW 0x00
#define TEMP_HIGH 0x01
#define HUMID_LOW 0x02
#define HUMID_HIGH 0x03
#define CONFIG 0x0E
#define MEASUREMENT_CONFIG 0x0F
#define DEVICE_ID_L 0xFE
#define DEVICE_ID_H 0xFF

#define READ_REG 0x00



#define SLAVE_ADDRESS 0x40

uint8_t rx[4]={0};


void writeReg(uint16_t baseAddress, uint8_t reg, uint8_t data)
{
    //Store current TXIE status
        uint16_t txieStatus = HWREG16(baseAddress + OFS_UCBxIE) & UCTXIE;

        //Disable transmit interrupt enable
        HWREG16(baseAddress + OFS_UCBxIE) &= ~(UCTXIE);

        //Send start condition.
        HWREG16(baseAddress + OFS_UCBxCTLW0) |= UCTR + UCTXSTT;

        //Poll for transmit interrupt flag.
        while (!(HWREG16(baseAddress + OFS_UCBxIFG) & UCTXIFG)) ;

        //Send single byte data.
        HWREG16(baseAddress + OFS_UCBxTXBUF) = reg;

        //Poll for transmit interrupt flag.
        while (!(HWREG16(baseAddress + OFS_UCBxIFG) & UCTXIFG)) ;

        //Send single byte data.
        HWREG16(baseAddress + OFS_UCBxTXBUF) = data;

            //Poll for transmit interrupt flag.
        while (!(HWREG16(baseAddress + OFS_UCBxIFG) & UCTXIFG)) ;

        //Send stop condition.
        HWREG16(baseAddress + OFS_UCBxCTLW0) |= UCTXSTP;

        //Clear transmit interrupt flag before enabling interrupt again
        HWREG16(baseAddress + OFS_UCBxIFG) &= ~(UCTXIFG);

        //Reinstate transmit interrupt enable
        HWREG16(baseAddress + OFS_UCBxIE) |= txieStatus;

}
void openReg(uint8_t reg)
{
    UCB0IE &= ~UCTXIE;
    UCB0CTLW0 |= UCTR + UCTXSTT;
    UCB0IFG & UCTXIFG;
    UCB0TXBUF = reg;              // point to specified register

}

void requestFrom(uint8_t quantity, uint8_t *buffer)
{
    UCB0CTLW0 &= ~UCTR;
    UCB0CTLW0 |= UCTXSTT;
    UCB0CTLW0 & UCTXSTT;
    uint8_t i = 0;
    while(i < quantity){
        if(i == quantity - 1)
            UCB0CTLW0 |= UCTXSTP;
        buffer[i] = UCB0RXBUF;
        i++;
    }
    //while (!(UCB0IFG & UCRXIFG));
    //return UCB0RXBUF;
    UCB0CTLW0 & UCTXSTP;
}

void readRegis(uint16_t baseAddress)
{

    //Set USCI in Receive mode
    uint16_t txieStatus = HWREG16(baseAddress + OFS_UCBxIE) & UCTXIE;

      //Disable transmit interrupt enable
    HWREG16(baseAddress + OFS_UCBxIE) &= ~(UCTXIE);

    //Send start condition
    HWREG16(baseAddress + OFS_UCBxCTLW0) |= UCTXSTT;

    //Poll for start condition transmission
    while(HWREG16(baseAddress + OFS_UCBxCTLW0) & UCTXSTT);

    while (!(HWREG16(baseAddress + OFS_UCBxIFG) & UCTXIFG));
    UCB0TXBUF = READ_REG;


    //Set USCI in Receive mode
    HWREG16(baseAddress + OFS_UCBxCTLW0) &= ~UCTR;

        //Send start condition
    HWREG16(baseAddress + OFS_UCBxCTLW0) |= UCTXSTT;


    while (!(HWREG16(baseAddress + OFS_UCBxIFG) & UCRXIFG));
   rx[0]=UCB0RXBUF;

    while (!(HWREG16(baseAddress + OFS_UCBxIFG) & UCRXIFG));
    rx[1] =UCB0RXBUF;

    while (!(HWREG16(baseAddress + OFS_UCBxIFG) & UCRXIFG));
    rx[2] = UCB0RXBUF;

    while (!(HWREG16(baseAddress + OFS_UCBxIFG) & UCRXIFG));
    rx[3]= UCB0RXBUF;

          //Send stop condition.
    HWREG16(baseAddress + OFS_UCBxCTLW0) |= UCTXSTP;
    //Clear transmit interrupt flag before enabling interrupt again
}

float readHumidity(void)
{
    uint16_t humidity;
    humidity = rx[3];
    humidity = (humidity << 8) | rx[2];
    float f = humidity;
    f = (f / 65536.0f) * 100.0f;

    return f;
}

float readTemp(void)
{
    uint16_t temp;
    temp = rx[1];
    temp = (temp << 8) | rx[0];
    float f = temp;
    f = ((f * 165.0f) / 65536.0f) - 40.0f;
    return f;
}
/*  Bits 5 and 6 of the MEASUREMENT_CONFIG register controls
    the humidity resolution*/
void setHumidRes(int resolution)
{
    uint8_t configContents;


    switch (resolution)
    {
    case FOURTEEN_BIT:
        configContents =  0xCF;
        break;
    case ELEVEN_BIT:
        configContents =   0x10;
        break;

    case NINE_BIT:
        configContents =  0x20;
        break;

    default:
        configContents = 0xCF;
    }

    writeReg(EUSCI_B0_BASE, MEASUREMENT_CONFIG, configContents);
}

void setTempRes(int resolution)
{
    uint8_t configContents;
    configContents = readReg(MEASUREMENT_CONFIG);

    switch (resolution)
    {
    case FOURTEEN_BIT:
        configContents = (configContents & 0x3F);
        break;

    case ELEVEN_BIT:
        configContents = (configContents & 0x7F);
        configContents = (configContents | 0x40);
        break;

    case NINE_BIT:
        configContents = (configContents & 0xBF);
        configContents = (configContents | 0x80);
        break;

    default:
        configContents = (configContents & 0x3F);
    }

    writeReg(EUSCI_B0_BASE, MEASUREMENT_CONFIG, configContents);
}
void setMeasurementMode(int mode)
{
    uint8_t configContents;
    configContents = readReg(MEASUREMENT_CONFIG);

    switch (mode)
    {
    case TEMP_AND_HUMID:
        configContents = (configContents & 0xF9);
        break;

    case TEMP_ONLY:
        configContents = (configContents & 0xFC);
        configContents = (configContents | 0x02);
        break;

    case HUMID_ONLY:
        configContents = (configContents & 0xFD);
        configContents = (configContents | 0x04);
        break;

    default:
        configContents = (configContents & 0xF9);
    }

    writeReg(EUSCI_B0_BASE, MEASUREMENT_CONFIG, configContents);
}

void setRate(int rate)
{
    uint8_t configContents;

    switch (rate)
    {
    case MANUAL:
        configContents = 0x8F;
        break;

    case TWO_MINS:
        configContents = 0x10;
        break;
    case ONE_MINS:
        configContents = 0x20;
        break;

    case TEN_SECONDS:

        configContents =  0x30;
        break;

    case FIVE_SECONDS:

        configContents =  0x40;
        break;

    case ONE_HZ:

        configContents = 0x50;
        break;

    case TWO_HZ:

        configContents =  0x60;
        break;

    case FIVE_HZ:
        configContents =  0x70;
        break;

    default:
        configContents = 0x8F;
    }

    writeReg(EUSCI_B0_BASE, CONFIG, configContents);
}

/*  Bit 1 of the CONFIG register can be used to control the
    the interrupt pins polarity */
void setInterruptPolarity(int polarity)
{
    uint8_t configContents;
    switch (polarity)
    {
    case ACTIVE_LOW:
        configContents = 0xFD;
        break;

    case ACTIVE_HIGH:
        configContents =  0x02;
        break;

    default:
        configContents =  0xFD;
    }

    writeReg(EUSCI_B0_BASE, CONFIG, configContents);
}
void setInterruptMode(int mode)
{
    uint8_t configContents;
    switch (mode)
    {
    case LEVEL_MODE:
        configContents =  0xFE;
        break;

    case COMPARATOR_MODE:
        configContents =  0x01;
        break;

    default:
        configContents =  0xFE;
    }

    writeReg(EUSCI_B0_BASE, CONFIG, configContents);
}

uint8_t readInterruptStatus(void)
{
    uint8_t regContents;
    regContents = readReg(INTERRUPT_DRDY);
    return regContents;
}

// Disables the interrupt pin for comfort zone operation
void disableThresholdInterrupt(void)
{
    uint8_t regContents;
    regContents = readReg(INTERRUPT_CONFIG);

    regContents = (regContents & 0x87);

    writeReg(EUSCI_B0_BASE, INTERRUPT_CONFIG, regContents);
}

// enables the interrupt pin for DRDY operation
void enableDRDYInterrupt(void)
{
    uint8_t regContents;
    regContents =  0x80;

    writeReg(EUSCI_B0_BASE, INTERRUPT_CONFIG, regContents);
}

// disables the interrupt pin for DRDY operation
void disableDRDYInterrupt(void)
{
    uint8_t regContents;


    regContents = (INTERRUPT_CONFIG & 0x7F);

    writeReg(EUSCI_B0_BASE, INTERRUPT_CONFIG, regContents);
}

bool uartInit(void)
{

    GPIO_setAsPeripheralModuleFunctionInputPin(
    GPIO_PORT_P2,
    GPIO_PIN0 + GPIO_PIN1,
    GPIO_SECONDARY_MODULE_FUNCTION
    );

    EUSCI_A_UART_initParam param = {0};
    param.selectClockSource = EUSCI_A_UART_CLOCKSOURCE_ACLK;
    param.clockPrescalar = 3;
    param.firstModReg = 0;
    param.secondModReg = 0x92;
    param.parity = EUSCI_A_UART_NO_PARITY;
    param.msborLsbFirst = EUSCI_A_UART_LSB_FIRST;
    param.numberofStopBits = EUSCI_A_UART_ONE_STOP_BIT;
    param.uartMode = EUSCI_A_UART_MODE;
    param.overSampling = EUSCI_A_UART_LOW_FREQUENCY_BAUDRATE_GENERATION;

    if (STATUS_FAIL == EUSCI_A_UART_init(EUSCI_A0_BASE, &param)) {
        return false;
    }

    EUSCI_A_UART_enable(EUSCI_A0_BASE);

    return true;
}
void clockSet(void)
{
    GPIO_setAsPeripheralModuleFunctionInputPin(
        GPIO_PORT_PJ,
        GPIO_PIN4 + GPIO_PIN5,
        GPIO_PRIMARY_MODULE_FUNCTION
    );

    //Set DCO frequency to 1 MHz
        CS_setDCOFreq(CS_DCORSEL_0,CS_DCOFSEL_0);
        //Set external clock frequency to 32.768 KHz
        CS_setExternalClockSource(32768,0);
        //Set ACLK=LFXT
        CS_initClockSignal(CS_ACLK,CS_LFXTCLK_SELECT,CS_CLOCK_DIVIDER_1);
        //Set SMCLK = DCO with frequency divider of 1
        CS_initClockSignal(CS_SMCLK,CS_DCOCLK_SELECT,CS_CLOCK_DIVIDER_1);
        //Set MCLK = DCO with frequency divider of 1
        CS_initClockSignal(CS_MCLK,CS_DCOCLK_SELECT,CS_CLOCK_DIVIDER_1);
        //Start XT1 with no time out
        CS_turnOnLFXT(CS_LFXT_DRIVE_0);
}
void UART_SendString(char* str) {
    int i = 0;
    while (str[i] != '\0') {
        UART_SendByte(str[i]); // Send each character of the string
        i++;
    }
}
void UART_SendByte(uint16_t byte) {
    while (!(UCA0IFG & UCTXIFG));
    UCA0TXBUF = byte;
}

void init_i2c_Master(void){
    GPIO_setAsPeripheralModuleFunctionInputPin(
        GPIO_PORT_P1,
        GPIO_PIN6 + GPIO_PIN7,
        GPIO_SECONDARY_MODULE_FUNCTION
    );

    GPIO_setAsInputPin(
        GPIO_PORT_P3,
        GPIO_PIN4
    );// PMM_unlockLPM5();

    EUSCI_B_I2C_initMasterParam param = {0};
    param.selectClockSource = EUSCI_B_I2C_CLOCKSOURCE_SMCLK;
    param.i2cClk = CS_getSMCLK();
    param.dataRate = EUSCI_B_I2C_SET_DATA_RATE_400KBPS;
    param.byteCounterThreshold = 4;
    param.autoSTOPGeneration = EUSCI_B_I2C_SEND_STOP_AUTOMATICALLY_ON_BYTECOUNT_THRESHOLD;
    EUSCI_B_I2C_initMaster(EUSCI_B0_BASE, &param);

    //Specify slave address
    EUSCI_B_I2C_setSlaveAddress(EUSCI_B0_BASE,
        SLAVE_ADDRESS
        );

    PM5CTL0 &= ~LOCKLPM5;                       //turn on I/O
    UCB0CTLW0 &= ~ UCSWRST;                     // turn off soft reset;


    EUSCI_B_I2C_setMode(EUSCI_B0_BASE,
            EUSCI_B_I2C_TRANSMIT_MODE    //Set Master in receive mode
                 );

    EUSCI_B_I2C_enable(EUSCI_B0_BASE);            //Enable I2C Module to start operations
}
/*  Bit 0 of the MEASUREMENT_CONFIG register can be used
    to trigger measurements  */
void triggerMeasurement(void)
{
    uint8_t configContents;
    configContents =  0x01;
    writeReg(EUSCI_B0_BASE, MEASUREMENT_CONFIG, configContents);
}






