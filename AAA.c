
#include "driverlib.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#define SLAVE_ADDRESS 0x40
static void init_i2c (void);
static void masterSend2byte (uint16_t baseAddress, uint8_t config, uint8_t hz);
static void  MasterReceiveByte(uint16_t baseAddress);
static void clockSet(void);
static bool uartInit(void);
static void UART_SendByte(uint16_t byte);
//static void UART_SendBuffer(uint16_t* buffer, uint16_t length);
static void UART_SendString(char* str);
void uartPuts( const char* src, size_t size);
void Seriel_output(char *str);
void USCIB0_ISR(void);


// set AMM rate
 uint8_t config = 0x0e;
 uint8_t  hz = 0x50;
 uint8_t meas_config = 0x0f;
 uint8_t start_conversion = 0x01;
 uint8_t register_int = 0x07;
 uint8_t enable_int = 0x80;
#define RXCOUNT 0x04
#define MAX_BUFFER_SIZE     20
 uint8_t RXByteCtr = 0;


volatile int rxdata;
//volatile int receive_val = 0;

uint8_t ReceiveBuffer[20] = {};

uint16_t ReceiveIndex = 0;
uint8_t rxcount = 0;
int  temperature_raw;
float          temp_C;
float rel_H;
int humidity_raw;




char rx_char[];
char rx_char1[];

int main(){

    WDTCTL = WDTPW | WDTHOLD;


    clockSet();
    uartInit();
    init_i2c();

    //Set Master in TX mode
        EUSCI_B_I2C_setMode(EUSCI_B0_BASE,
                EUSCI_B_I2C_TRANSMIT_MODE
                );

        //Enable I2C Module to start operations
        EUSCI_B_I2C_enable(EUSCI_B0_BASE);
        GPIO_setOutputLowOnPin (GPIO_PORT_P3, GPIO_PIN4);


        masterSend2byte (EUSCI_B0_BASE, config, hz);

        masterSend2byte (EUSCI_B0_BASE,  meas_config, start_conversion);

        masterSend2byte (EUSCI_B0_BASE,  register_int, enable_int);


        MasterReceiveByte(EUSCI_B0_BASE);
        while(!(UCB0IFG&UCRXIFG));
        UCA0TXBUF = rxdata++;

        ReceiveBuffer[ReceiveIndex++] = rxdata;




    //uint16_t bufferLength = sizeof(ReceiveBuffer) / sizeof(ReceiveBuffer[0]);
    //UART_SendBuffer(ReceiveBuffer, bufferLength);
    temperature_raw = (ReceiveBuffer[1] << 8) + ReceiveBuffer[0];
    temp_C = (temperature_raw/65536.0)*165-40;

    humidity_raw = (ReceiveBuffer[3] << 8) + ReceiveBuffer[2];
    rel_H = (humidity_raw/65536.0)*100;

    ltoa(temp_C, rx_char, 10);
    UART_SendString("\n Temperature: ");
    Seriel_output(rx_char);

    ltoa( rel_H, rx_char1, 10);
    UART_SendString("\n Humidity: ");
    uartPuts(rx_char1, strlen(rx_char1));
    Seriel_output(rx_char1);





}
void Seriel_output(char *str)
    {
        while(*str !=0)
        {
            while(!(UCA0IFG&UCTXIFG));
            UCA0TXBUF = *str++;
        }
    }

void uartPuts( const char* src, size_t size)
{
    while(size--)
    {
        EUSCI_A_UART_transmitData(EUSCI_A0_BASE, *src);
        src++;
    }
}

void UART_SendString(char* str) {
    int i = 0;
    while (str[i] != '\0') {
        UART_SendByte(str[i]); // Send each character of the string
        i++;
    }
}
void UART_SendByte(uint16_t byte) {
    while (!(UCA0IFG & UCTXIFG)); // Ждем готовности для передачи
    UCA0TXBUF = byte; }

 /*void UART_SendBuffer(uint16_t* buffer, uint16_t length) {
     int i = 0;
     while ( i < length) {
            UART_SendByte(buffer[i]);
            i++;
        }
    } */

void init_i2c (void)
{


    GPIO_setAsPeripheralModuleFunctionInputPin(
        GPIO_PORT_P1,
        GPIO_PIN6 + GPIO_PIN7,
        GPIO_SECONDARY_MODULE_FUNCTION
    );

    GPIO_setAsOutputPin(
        GPIO_PORT_P3,
        GPIO_PIN4
    );// PMM_unlockLPM5();

    EUSCI_B_I2C_initMasterParam param = {0};
    param.selectClockSource = EUSCI_B_I2C_CLOCKSOURCE_SMCLK;
    param.i2cClk = CS_getSMCLK();
    param.dataRate = EUSCI_B_I2C_SET_DATA_RATE_400KBPS;
    param.byteCounterThreshold = RXCOUNT;
    param.autoSTOPGeneration = EUSCI_B_I2C_SEND_STOP_AUTOMATICALLY_ON_BYTECOUNT_THRESHOLD;
    EUSCI_B_I2C_initMaster(EUSCI_B0_BASE, &param);

    //Specify slave address
    EUSCI_B_I2C_setSlaveAddress(EUSCI_B0_BASE,
        SLAVE_ADDRESS
        );

    PM5CTL0 &= ~LOCKLPM5;  //turn on I/O
    UCB0CTLW0 &= ~ UCSWRST; //// turn off soft reset;


}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USCI_B0_VECTOR
__interrupt
#elif defined(__GNUC__)
__attribute__((interrupt(USCI_B0_VECTOR)))
#endif
void USCIB0_ISR(void)
{
    //uint8_t count = 0;
    switch(__even_in_range(UCB0IV, USCI_I2C_UCBIT9IFG))
    {
        case USCI_NONE:             // No interrupts break;
            break;
        case USCI_I2C_UCALIFG:      // Arbitration lost
            break;
        case USCI_I2C_UCNACKIFG:    // NAK received (master only)
            EUSCI_B_I2C_masterReceiveStart(EUSCI_B0_BASE);
            break;
        case USCI_I2C_UCSTTIFG:     // START condition detected with own address (slave mode only)
            break;
        case USCI_I2C_UCSTPIFG:     // STOP condition detected (master & slave mode)
            break;
        case USCI_I2C_UCRXIFG3:     // RXIFG3
            break;
        case USCI_I2C_UCTXIFG3:     // TXIFG3
            break;
        case USCI_I2C_UCRXIFG2:     // RXIFG2
            break;
        case USCI_I2C_UCTXIFG2:

            break;
        case USCI_I2C_UCRXIFG1:
            // RXIFG1
            break;
        case USCI_I2C_UCTXIFG1:

            break;
        case USCI_I2C_UCRXIFG0:     // RXIFG0
            break; // Vector 24: RXIFG0 break;
        case USCI_I2C_UCTXIFG0:
            masterSend2byte (EUSCI_B0_BASE, config, hz);// TXIFG0
            break;
        case USCI_I2C_UCBCNTIFG:
            if (rxcount == 4)
                          {
                              //Must send stop since this is the N-1 byte
                              while((UCB0CTLW0 & UCTXSTT));
                              UCB0CTLW0 |= UCTXSTP;      // Send stop condition
                          }// Byte count limit reached (UCBxTBCNT)
            break;
        case USCI_I2C_UCCLTOIFG:    // Clock low timeout - clock held low too long
            break;
        case USCI_I2C_UCBIT9IFG:    // Generated on 9th bit of a transmit (for debugging)
            break;
        default:
            break;
    }
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






void MasterReceiveByte(uint16_t baseAddress)
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
    HWREG16(baseAddress + OFS_UCBxTXBUF) = 0x00;

        //Poll for transmit interrupt flag.
    while (!(HWREG16(baseAddress + OFS_UCBxIFG) & UCTXIFG)) ;

    //Set USCI in Receive mode
    HWREG16(baseAddress + OFS_UCBxCTLW0) &= ~UCTR;

        //Send start condition
    HWREG16(baseAddress + OFS_UCBxCTLW0) |= UCTXSTT;


    while (!(HWREG16(baseAddress + OFS_UCBxIFG) & UCRXIFG));
    HWREG16(baseAddress + OFS_UCBxRXBUF);

      while (!(HWREG16(baseAddress + OFS_UCBxIFG) & UCRXIFG));
          HWREG16(baseAddress + OFS_UCBxRXBUF);

       while (!(HWREG16(baseAddress + OFS_UCBxIFG) & UCRXIFG));
            HWREG16(baseAddress + OFS_UCBxRXBUF);

       while (!(HWREG16(baseAddress + OFS_UCBxIFG) & UCRXIFG));
               HWREG16(baseAddress + OFS_UCBxRXBUF);

          //Send stop condition.
    HWREG16(baseAddress + OFS_UCBxCTLW0) |= UCTXSTP;
    //Clear transmit interrupt flag before enabling interrupt again
}
 void masterSend2byte (uint16_t baseAddress,
     uint8_t config, uint8_t hz
     )
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
     HWREG16(baseAddress + OFS_UCBxTXBUF) = config;

     //Poll for transmit interrupt flag.
     while (!(HWREG16(baseAddress + OFS_UCBxIFG) & UCTXIFG)) ;

     //Send single byte data.
     HWREG16(baseAddress + OFS_UCBxTXBUF) =hz;

         //Poll for transmit interrupt flag.
     while (!(HWREG16(baseAddress + OFS_UCBxIFG) & UCTXIFG)) ;

     //Send stop condition.
     HWREG16(baseAddress + OFS_UCBxCTLW0) |= UCTXSTP;

     //Clear transmit interrupt flag before enabling interrupt again
     HWREG16(baseAddress + OFS_UCBxIFG) &= ~(UCTXIFG);

     //Reinstate transmit interrupt enable
     HWREG16(baseAddress + OFS_UCBxIE) |= txieStatus;
 }



