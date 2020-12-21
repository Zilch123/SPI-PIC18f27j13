#include <xc.h>

char data;
void __interrupt() ISR(void)
{
if(SSPIF)  // check if interrupt occurs due to SPI module
{
data = SSPBUF; // copy data from receive buffer into a global variable data
SSPIF = 0; // reset flag interrupt for SPI module
}
}
void delay(unsigned int nMilliseconds)
{
#define CYCLES_PER_MS 100 /* Number of decrement-and-test cycles. */
unsigned long nCycles = nMilliseconds * CYCLES_PER_MS;
while (nCycles--);
}
void SPI_init()
{

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
SSPIE = 1;
PEIE = 1;
GIE = 1;
}

void main(void) 
{
SPI_init();
TRISB = 0x00;
while(1)
{
if (data==0x01)     
{
RB0=1;       //if received data = 0x01, turn on LED
}
else if (data==0x02)
{
RB0=0;  //if received data = 0x02, turn off LED
}
}
return;
}
