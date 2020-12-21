#include <xc.h>
//#include "newxc8_header.h"  // generated configuration bits header file
void delay(unsigned int nMilliseconds)
{
#define CYCLES_PER_MS 100 /* Number of decrement-and-test cycles. */
unsigned long nCycles = nMilliseconds * CYCLES_PER_MS;
while (nCycles--);
}
void SPI_init()
{
// this setting selects master mode with frequency fosc/4
SSPCON1bits.SSPM0 = 0;
SSPCON1bits.SSPM1 = 0;
SSPCON1bits.SSPM2 = 0;
SSPCON1bits.SSPM3 = 0;
// Enable SPI Port
SSPCON1bits.SSPEN = 1;
// Configure The Clock Polarity & Phase
SSPCON1bits.CKP = 0;
SSPSTATbits.CKE = 0;
//  Slew rate control enabled for High Speed mode
SSPSTATbits.SMP = 0;
// Set SPI pins as digital I/O
TRISC5 = 0; // SDO -> Output
TRISC4 = 1; // SDI -> Input
TRISC3 = 0; // SCK -> Output
//Uncommnent this line, if you want to send send over interrupt
// SSPIE = 1; PEIE = 1; GIE = 1;
}
//------------------------------------------------------------------------------
void SPI_write(char data)
{
SSPBUF = data;
while(SSPBUF == 0);
}
//-----------------------------------------------------------------------------

void main(void) 
{
SPI_init();
TRISB=0x00;
while(1)
{ 
//if(RB0) 
delay(1000);
SPI_write(0x01);    // if switch pressed, send 0x01
//else 
delay(1000);
SPI_write(0x02);    // if switch is pressed pressed, send 0x02
}

return;
}