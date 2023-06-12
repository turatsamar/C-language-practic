/* --COPYRIGHT--,BSD
 * Copyright (c) 2017, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * --/COPYRIGHT--*/
#include "driverlib.h"

//*****************************************************************************
//
//! i2c_master_rx_multi_int
//!
//! This example shows how to configure the I2C module as a master for
//! multi-byte reception with teh help of interrupts. The address of the slave module
//! that the master communicating with also set in this example. This example uses
//! the interrupt driven mode to receive data.
//!
//! This example uses the following peripherals and I/O signals.  You must
//! review these and change as needed for your own board:
//! - I2C peripheral
//! - GPIO Port peripheral (for I2C pins)
//! - SCL2
//! - SDA
//!
//! This example uses the following interrupt handlers.  To use this example
//! in your own application you must add these interrupt handlers to your
//! vector table.
//! - USCI_B0_VECTOR
//!
//
//*****************************************************************************

//*****************************************************************************
//
//Set the address for slave module. This is a 7-bit address sent in the
//following format:
//[A6:A5:A4:A3:A2:A1:A0:RS]
//
//A zero in the "RS" position of the first byte means that the master
//transmits (sends) data to the selected slave, and a one in this position
//means that the master receives data from the slave.
//
//*****************************************************************************
#define SLAVE_ADDRESS 0x40

//*****************************************************************************
//
//Specify Expected Receive data count.
//
//*****************************************************************************



//******************************************************************************
//  MSP430FR59xx Demo - USCI_B0 I2C Master RX multiple bytes from MSP430 Slave
//
//  Description: This demo connects two MSP430's via the I2C bus. The master
//  reads 5 bytes from the slave. This is the MASTER CODE. The data from the slave
//  transmitter begins at 0 and increments with each transfer.
//  The USCI_B0 RX interrupt is used to know when new data has been received.
//  ACLK = n/a, MCLK = SMCLK = BRCLK =  DCO = 1MHz
//
//                                /|\  /|\
//               MSP430FR5969      10k  10k     MSP430F5969
//                   slave         |    |        master
//             -----------------   |    |   -----------------
//           -|XIN  P1.6/UCB0SDA|<-|----+->|P1.6/UCB0SDA  XIN|-
//            |                 |  |       |                 | 32kHz
//           -|XOUT             |  |       |             XOUT|-
//            |     P1.7/UCB0SCL|<-+------>|P1.7/UCB0SCL     |
//            |                 |          |             P1.0|--> LED
//
//******************************************************************************
#include <string.h>
#include <stdio.h>


#define ARRAY_SIZE     4


// set AMM rate
 uint8_t config;
 uint8_t hz;
 uint8_t meas_config;
 uint8_t start_conversion;
 uint8_t register_int;
 uint8_t enable_int;
 int i = 0;;


 void data() {
     int rxBuffer[ARRAY_SIZE] = {0x5a, 0x65, 0x84, 0xb9};
     int dataArray[ARRAY_SIZE];
     uint8_t i = 0;
     while ( i < ARRAY_SIZE) {
         dataArray[i] = rxBuffer[i];
         i++;
     }

     while (i < ARRAY_SIZE) {
         printf("Data[%d]: 0x%x\n", i, dataArray[i]);
         i++;
     }
 }


 void i2c_Puts(const char* src, size_t size)
 {
     while(size--)
     {
         EUSCI_B_I2C_slavePutData(EUSCI_A0_BASE, *src);
         src++;
     }
 }
 int EUSCI_B_I2C_masterReceive(uint16_t baseAddress)
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

      GPIO_setOutputHighOnPin (GPIO_PORT_P3, GPIO_PIN4);
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
       HWREG16(baseAddress + OFS_UCBxIFG) &= ~(UCRXIFG);

       //Reinstate transmit interrupt enable
       return 0;
}
 void EUSCI_B_I2C_masterSendSingleByt (uint16_t baseAddress,
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
void main (void)
{
    WDT_A_hold(WDT_A_BASE);

    UCB0CTLW0 |= UCSWRST;// put into soft reset

    //Set DCO frequency to 1MHz
    CS_setDCOFreq(CS_DCORSEL_0,CS_DCOFSEL_0);
    //Set ACLK = VLO with frequency divider of 1
    CS_initClockSignal(CS_ACLK,CS_VLOCLK_SELECT,CS_CLOCK_DIVIDER_1);
    //Set SMCLK = DCO with frequency divider of 1
    CS_initClockSignal(CS_SMCLK,CS_DCOCLK_SELECT,CS_CLOCK_DIVIDER_1);
    //Set MCLK = DCO with frequency divider of 1
    CS_initClockSignal(CS_MCLK,CS_DCOCLK_SELECT,CS_CLOCK_DIVIDER_1);

    // Configure Pins for I2C
    //Set P1.6 and P1.7 as Secondary Module Function Input.
    /*

    * Select Port 1
    * Set Pin 6, 7 to input Secondary Module Function, (UCB0SIMO/UCB0SDA, UCB0SOMI/UCB0SCL).
    */
    GPIO_setAsPeripheralModuleFunctionInputPin(
        GPIO_PORT_P1,
        GPIO_PIN6 + GPIO_PIN7,
        GPIO_SECONDARY_MODULE_FUNCTION
    );

    //Set P1.0 as an output pin.
    /*

     * Select Port 1
     * Set Pin 0 as output
     */
    GPIO_setAsOutputPin(
        GPIO_PORT_P3,
        GPIO_PIN4
    );

    /*
     * Disable the GPIO power-on default high-impedance mode to activate
     * previously configured port settings
     */
   // PMM_unlockLPM5();

    EUSCI_B_I2C_initMasterParam param = {0};
    param.selectClockSource = EUSCI_B_I2C_CLOCKSOURCE_SMCLK;
    param.i2cClk = CS_getSMCLK();
    param.dataRate = EUSCI_B_I2C_SET_DATA_RATE_400KBPS;
    param.byteCounterThreshold = 0;
    param.autoSTOPGeneration = EUSCI_B_I2C_SEND_STOP_AUTOMATICALLY_ON_BYTECOUNT_THRESHOLD;
    EUSCI_B_I2C_initMaster(EUSCI_B0_BASE, &param);

    //Specify slave address
    EUSCI_B_I2C_setSlaveAddress(EUSCI_B0_BASE,
        SLAVE_ADDRESS
        );

    PM5CTL0 &= ~LOCKLPM5;  //turn on I/O
    UCB0CTLW0 &= ~ UCSWRST; //// turn off soft reset;

    //Set Master in TX mode
    EUSCI_B_I2C_setMode(EUSCI_B0_BASE,
            EUSCI_B_I2C_TRANSMIT_MODE
            );

    //Enable I2C Module to start operations
    EUSCI_B_I2C_enable(EUSCI_B0_BASE);
    GPIO_setOutputLowOnPin (GPIO_PORT_P3, GPIO_PIN4);

    config = 0x0e;
    hz = 0x50;
    meas_config = 0x0f;
    start_conversion = 0x01;
    register_int = 0x07;
    enable_int = 0x80;
     //char* str1 = "TEMP" ;
     //char* str2 = "HUM" ;




    EUSCI_B_I2C_masterSendSingleByt (EUSCI_B0_BASE, config, hz);

    EUSCI_B_I2C_masterSendSingleByt (EUSCI_B0_BASE,  meas_config, start_conversion);

    EUSCI_B_I2C_masterSendSingleByt (EUSCI_B0_BASE,  register_int, enable_int);
    //GPIO_setOutputHighOnPin (GPIO_PORT_P3, GPIO_PIN4);
    while(1){




        EUSCI_B_I2C_masterReceive(EUSCI_B0_BASE);
        data();




        __bis_SR_register(CPUOFF+GIE);
    }

}




