
/*
 * File:   newmain1.c
 * Author: timoth dev
 *
 * Created on 21 December, 2020, 6:51 PM
 */
#pragma config WDTEN = ON       // Watchdog Timer (Enabled)
#pragma config PLLDIV = 1       // 96MHz PLL Prescaler Selection (PLLSEL=0) (No prescale (4 MHz oscillator input drives PLL directly))
#pragma config CFGPLLEN = OFF   // PLL Enable Configuration Bit (PLL Disabled)
#pragma config STVREN = ON      // Stack Overflow/Underflow Reset (Enabled)
#pragma config XINST = OFF       // Extended Instruction Set (Enabled)

// CONFIG1H
#pragma config CP0 = OFF        // Code Protect (Program memory is not code-protected)

// CONFIG2L
#pragma config OSC = ECPLL      // Oscillator (EC+PLL (CLKO-RA6))
#pragma config SOSCSEL = HIGH   // T1OSC/SOSC Power Selection Bits (High Power T1OSC/SOSC circuit selected)
#pragma config CLKOEC = ON      // EC Clock Out Enable Bit  (CLKO output enabled on the RA6 pin)
#pragma config FCMEN = ON       // Fail-Safe Clock Monitor (Enabled)
#pragma config IESO = ON        // Internal External Oscillator Switch Over Mode (Enabled)

// CONFIG2H
#pragma config WDTPS = 32768    // Watchdog Postscaler (1:32768)

// CONFIG3L
#pragma config DSWDTOSC = INTOSCREF// DSWDT Clock Select (DSWDT uses INTRC)
#pragma config RTCOSC = T1OSCREF// RTCC Clock Select (RTCC uses T1OSC/T1CKI)
#pragma config DSBOREN = ON     // Deep Sleep BOR (Enabled)
#pragma config DSWDTEN = ON     // Deep Sleep Watchdog Timer (Enabled)
#pragma config DSWDTPS = G2     // Deep Sleep Watchdog Postscaler (1:2,147,483,648 (25.7 days))

// CONFIG3H
#pragma config IOL1WAY = ON     // IOLOCK One-Way Set Enable bit (The IOLOCK bit (PPSCON<0>) can be set once)
#pragma config ADCSEL = BIT10   // ADC 10 or 12 Bit Select (10 - Bit ADC Enabled)
#pragma config PLLSEL = PLL4X   // PLL Selection Bit (Selects 4x PLL)
#pragma config MSSP7B_EN = MSK7 // MSSP address masking (7 Bit address masking mode)

// CONFIG4L
#pragma config WPFP = PAGE_127  // Write/Erase Protect Page Start/End Location (Write Protect Program Flash Page 127)
#pragma config WPCFG = OFF      // Write/Erase Protect Configuration Region  (Configuration Words page not erase/write-protected)

// CONFIG4H
#pragma config WPDIS = OFF      // Write Protect Disable bit (WPFP<6:0>/WPEND region ignored)
#pragma config WPEND = PAGE_WPFP// Write/Erase Protect Region Select bit (valid when WPDIS = 0) (Pages WPFP<6:0> through Configuration Words erase/write protected)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
char data;

void delay(unsigned int nMilliseconds)
{
#define CYCLES_PER_MS 100 /* Number of decrement-and-test cycles. */
unsigned long nCycles = nMilliseconds * CYCLES_PER_MS;
while (nCycles--);
}

void __interrupt() ISR(void)
{
//RB7=1;
if(SSP1IF)  // check if interrupt occurs due to SPI module
{
    
//    SSP1IF -->Master Synchronous Serial Port 1 Interrupt Flag bit
    
    data = SSPBUF; // copy data from receive buffer into a global variable data
    SSP1IF = 0; // reset flag interrupt for SPI module
}
}

void SPI_init()
{
RB7=0;
// Set Spi Mode To Slave + SS Enabled 
SSPCON1bits.SSPM0 = 0;
SSPCON1bits.SSPM1 = 0;
SSPCON1bits.SSPM2 = 1;
SSPCON1bits.SSPM3 = 0;
// Enable The Synchronous Serial Port
SSPCON1bits.SSPEN = 1;
// Configure The Clock Polarity & Phase (SPI Mode Num. 1)
SSPCON1bits.CKP = 0;
SSPSTATbits.CKE = 0;
// Clear The SMP Bit
SSPSTATbits.SMP = 0;
// Configure The IO Pins For SPI Master Mode
TRISC5 = 0; // SDO -> Output
TRISC4 = 1; // SDI -> Input
TRISC3 = 1; // SCK -> Intput
TRISA5 = 1; // SS -> Input
// Enable Interrupts. Comment Them To Disable interrupts(NOT Recommended)

INTCONbits.PEIE = 1; // Peripheral Interrupt Enable bit
INTCONbits.GIE = 1;  //Global Interrupt Enable bit
PIE1bits.SSP1IE=1; //Master Synchronous Serial Port 1 Interrupt Enable bit
IPR1bits.SSP1IP=1; // Master Synchronous Serial Port Interrupt Priority bit (MSSP1 module)
PIR1bits.SSP1IF = 0; // reset flag interrupt for SPI module
}

void main(void) 
{
SPI_init();
TRISB = 0x00;

while(1)
{
//    data = SSPBUF;
//    if(SSPSTATbits.BF) // check if buffer is full 
//        RB7=1;
    if (data==0x07)     
    {
        RB7=1;
        RB0=1;       //if received data = 0x01, turn on LED
    }
    else if (data==0x08)
    {
        RB7=0;
        RB0=0;  //if received data = 0x02, turn off LED
    }
}
return;
}
