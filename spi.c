
/*
 * spi.c
 *
 *  Created on: Oct 29, 2016
 *      Author: Nicholas DeCicco <nsd.cicco@gmail.com>
 *                               <deciccon0@students.rowan.edu>
 */

#include <msp430.h>
#include <stdint.h>
#include "spi.h"

/*
 * SPI B0
 */

// UCB0CLK is on P1.5
#define UCxxCLK_SEL  P1SEL
#define UCxxCLK_BIT  BIT5

// UCB0SOMI is on P1.6
#define UCxxSOMI_SEL P1SEL
#define UCxxSOMI_BIT BIT6

// UCB0SIMO is on P1.7
#define UCxxSIMO_SEL P1SEL
#define UCxxSIMO_BIT BIT7

// UCB0STE is on P2.7
#define UCxxSTE_SEL P2SEL
#define UCxxSTE_BIT BIT7
#define UCxxSTE_OUT P2OUT
#define UCxxSTE_DIR P2DIR

#define UCxxCTL0  UCB0CTL0
#define UCxxCTL1  UCB0CTL1
#define UCxxBR0   UCB0BR0
#define UCxxBR1   UCB0BR1
#define UCxxSTAT  UCB0STAT
#define UCxxTXBUF UCB0TXBUF
#define UCxxRXBUF UCB0RXBUF


//#define SPI_FUNC(x) spi_B0 ## x
const SPI spiB0 = {
	spiB0_init,
	spiB0_blockingSendSeq,
	spiB0_blockingSendByte,
	spiB0_blockingReadSeq,
	spiB0_assert,
	spiB0_deassert
};

void SPI_FUNC(init)()
{
    /* Enable UCxxCLK */
    UCxxCLK_SEL |=  UCxxCLK_BIT;

    /* Enable UCxxSOMI */
    UCxxSOMI_SEL |=  UCxxSOMI_BIT;

    /* Enable UCxxSIMO */
    UCxxSIMO_SEL |=  UCxxSIMO_BIT;

    /* Configure UCxxSTE as output */
    UCxxSTE_DIR |= UCxxSTE_BIT;
    UCxxSTE_OUT |= UCxxSTE_BIT; // Default to unasserted state

    /* Reset USCIxx registers and disable USCIxx by setting the UCSWRST
     * bit of UCxxCTLW0.
     */
    UCxxCTL1 |= UCSWRST;

    UCxxCTL0 |= UCMSB    | /* (Optional) Configure the USCI to send MSB first. */
                UCMST    | /* Select master mode. */
                UCSYNC   | /* Enable synchronous mode. */
                //UCCKPL   | /* Clock polarity: the inactive state is high. */
                UCCKPH   | /* Clock phase */
                UCMODE_0;  /* 3-pin SPI mode */

    /* Set clock source to SMCLK. (If we don't set a clock source,
     * UCSSEL will default to 0, which is "reserved"; i.e., not tied to any
     * clock source. (Behavior undefined afaik.)
     */
    UCxxCTL1 |= UCSSEL_2;
    UCxxBR0  = 0x02; /* divide by 2; this gives us ~0.5 MHz operation */
    UCxxBR1  = 0;

    /* Clear UCSWRST to take the USCI out of the reset condition. */
    UCxxCTL1 &= ~UCSWRST;
}


/**
 * Sends an individual byte of data over SPI.
 */
void SPI_FUNC(blockingSendByte)(const uint8_t data)
{
    UCxxTXBUF = data;

    /* Note: The MSP430x5xx and MSP430x6xx Family Guide, Rev. O (slau208o)
     * erronously refers to UCAxSTAT as 'UCAxSTATW'. There is no 'W' suffix
     * in the preprocessor macros found in the MSP430 development headers.
     */
    while (UCxxSTAT & UCBUSY) {} /* Wait for the send to complete. */
}

/**
 * Sends a sequence of data over SPI.
 */
void SPI_FUNC(blockingSendSeq)(uint8_t const* data, const int len)
{
    int i;

    for (i = 0; i < len; i++) {
        SPI_FUNC(blockingSendByte)(*data++);
    }
}

void SPI_FUNC(blockingReadSeq)(uint8_t *reply, const int len)
{
    int i;

    for (i = 0; i < len; i++) {
        SPI_FUNC(blockingSendByte)(0xFF);
        *reply++ = UCxxRXBUF;
    }
}

void SPI_FUNC(assert)()
{
    UCxxSTE_OUT &= ~UCxxSTE_BIT;
}

void SPI_FUNC(deassert)()
{
    UCxxSTE_OUT |= UCxxSTE_BIT;
}
///*
// * SPI B1
// */
//
//#undef UCxxCLK_SEL
//#undef UCxxCLK_BIT
//#undef UCxxSOMI_SEL
//#undef UCxxSOMI_BIT
//#undef UCxxSIMO_SEL
//#undef UCxxSIMO_BIT
//#undef UCxxSTE_SEL
//#undef UCxxSTE_BIT
//#undef UCxxSTE_OUT
//#undef UCxxSTE_DIR
//#undef UCxxCTL0
//#undef UCxxCTL1
//#undef UCxxBR0
//#undef UCxxBR1
//#undef UCxxSTAT
//#undef UCxxTXBUF
//#undef UCxxRXBUF
//
//// UCB1CLK is on P4.3
//#define UCxxCLK_SEL P4SEL
//#define UCxxCLK_BIT BIT3
//
//// UCB1SOMI is on P4.2
//#define UCxxSOMI_SEL P4SEL
//#define UCxxSOMI_BIT BIT2
//
//// UCB1SIMO is on P4.1
//#define UCxxSIMO_SEL P4SEL
//#define UCxxSIMO_BIT BIT1
//
//// UCB1STE is on P4.0
//#define UCxxSTE_SEL P4SEL
//#define UCxxSTE_BIT BIT0
//#define UCxxSTE_OUT P4OUT
//#define UCxxSTE_DIR P4DIR
//
//#define UCxxCTL0  UCB1CTL0
//#define UCxxCTL1  UCB1CTL1
//#define UCxxBR0   UCB1BR0
//#define UCxxBR1   UCB1BR1
//#define UCxxSTAT  UCB1STAT
//#define UCxxTXBUF UCB1TXBUF
//#define UCxxRXBUF UCB1RXBUF
//
////#undef SPI_FUNC
////#define SPI_FUNC(x) spiB1_ ## x
//
//#include <spi_generic.c>

//#define SPI_FUNC(x) spi_B1 ## x
//const SPI spiB1 = {
//	spiB1_init,
//	spiB1_blockingSendSeq,
//	spiB1_blockingSendByte,
//	spiB1_blockingReadSeq,
//	spiB1_assert,
//	spiB1_deassert
//};

#undef SPI_FUNC
