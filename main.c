#include <msp430.h>
#include <stdint.h>

#define test 0

#if test
#include "spi.h"
#include "max7221.h"
#endif

int miliseconds;
int distance;
long sensor;
int target = 20;

#if test
extern SPI spiB0;



int hundreds;
int tenths;
int integer;
int temp;

uint8_t * send;

#endif

#define ECHO BIT3
#define TRIGGER BIT4
#define LED BIT6


void main(void)
{
#if test
    spiB0_init();
    max7221_powerOn(&spiB0, 3);
    max7221_init(&spiB0, 3, 15);
#endif

  BCSCTL1 = CALBC1_1MHZ;
  DCOCTL = CALDCO_1MHZ;                     // SMCLK 1MHz
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT

  TA0CCTL0 = CCIE;                             // CCR0 interrupt enabled
  TA0CCR0 = 10000;                  // 1ms at 1mhz
  TA0CTL = TASSEL_2 + MC_1;                  // SMCLK, upmode

  P2IFG  = 0x00;                //clear all interrupt flags
  P1DIR |= LED;                            // P1.6 as output for LED
  P1OUT &= ~LED;                           // turn LED off
  P2DIR |= TRIGGER;          // trigger pin as output

#if !test
      DCOCTL = 0;                               // Select lowest DCOx and MODx settings
    BCSCTL1 = CALBC1_1MHZ;                    // Set DCO
    DCOCTL = CALDCO_1MHZ;
    P1SEL = BIT1 + BIT2 ;                     // P1.1 = RXD, P1.2=TXD
    P1SEL2 = BIT1 + BIT2 ;                    // P1.1 = RXD, P1.2=TXD
    UCA0CTL1 |= UCSSEL_2;                     // SMCLK
    UCA0BR0 = 104;                            // 1MHz 9600
    UCA0BR1 = 0;                              // 1MHz 9600
    UCA0MCTL = UCBRS0;                        // Modulation UCBRSx = 1
    UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
    IE2 |= UCA0RXIE;                          // Enable USCI_A0 RX interrupt

#endif

  _BIS_SR(GIE);                         // global interrupt enable

 while(1){
    P2IE &= ~0x01;          // disable interrupt
    P2OUT |= TRIGGER;          // generate pulse
    __delay_cycles(10);             // for 10us
    P2OUT &= ~TRIGGER;                 // stop pulse
    P2DIR &= ~ECHO;         // make pin P1.2 input (ECHO)
    P2IFG = 0x00;                   // clear flag just in case anything happened before
    P2IE |= ECHO;           // enable interrupt on ECHO pin
    P2IES &= ~ECHO;         // rising edge on ECHO pin
        __delay_cycles(30000);          // delay for 30ms (after this time echo times out if there is no object detected)
        distance = sensor/58;
#if !test
        while (!(IFG2&UCA0TXIFG));                // USCI_A0 TX buffer ready?
         UCA0TXBUF = distance;                    // TX -> RXed character// converting ECHO length into cm
#endif

#if test
         hundreds = distance/100;
         send[2] = hundreds;
        // max7221_write(spiB0, hundreds, 3);
         temp = distance - hundreds;
         tenths = temp/10;
         //max7221_write(spiB0, tenths, 3);
         send[1] = tenths;
         temp = distance - tenths;
         integer = temp;
        // max7221_write(spiB0, integer, 3);
         send[0] = integer;

         max7221_write(&spiB0, send, 3);

#endif

        if((distance < target) && (distance != 0))
            P1OUT |= LED;  //turning LED on if distance is less than 20cm and if distance isn't 0.
        else
            P1OUT &= ~LED;




 }
}

#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
}

#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void)
{
    if(P2IFG&ECHO)  //is there interrupt pending?
        {
          if(!(P2IES&ECHO)) // is this the rising edge?
          {
            TACTL|=TACLR;   // clears timer A
            miliseconds = 0;
            P2IES |= ECHO;  //falling edge
          }
          else
          {
            sensor = (long)miliseconds*1000 + (long)TAR;    //calculating ECHO length

          }
    P2IFG &= ~ECHO;             //clear flag
    }
}

#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A (void)
{
  miliseconds++;
}

//#if test
//// USCIAB0RX interrupt service routine
//#pragma vector = USCIAB0RX_VECTOR
//__interrupt void USCIAB0RX (void)
//{
//}
//
//
//// USCIAB0TX interrupt service routine
//#pragma vector = USCIAB0TX_VECTOR
//__interrupt void USCIAB0TX (void)
//{
//}
//#endif


#if !test
//  Echo back RXed character, confirm TX buffer is ready first
#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void)

{
    while (!(IFG2&UCA0TXIFG));                // USCI_A0 TX buffer ready?
     target = UCA0RXBUF;
}

#endif
