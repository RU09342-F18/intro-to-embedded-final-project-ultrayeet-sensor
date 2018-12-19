///*
// * spi_generic.c
// *
// *  Created on: December 5, 2016
// *      Author: Nicholas DeCicco <nsd.cicco@gmail.com>
// *                               <deciccon0@students.rowan.edu>
// */
//
///**
// * Initializes the USCI peripheral for SPI communications.
// */
//void SPI_FUNC(init)()
//{
//    /* Enable UCxxCLK */
//    UCxxCLK_SEL |=  UCxxCLK_BIT;
//
//    /* Enable UCxxSOMI */
//    UCxxSOMI_SEL |=  UCxxSOMI_BIT;
////
//    /* Enable UCxxSIMO */
//    UCxxSIMO_SEL |=  UCxxSIMO_BIT;
//
//    /* Configure UCxxSTE as output */
//    UCxxSTE_DIR |= UCxxSTE_BIT;
//    UCxxSTE_OUT |= UCxxSTE_BIT; // Default to unasserted state
//
//    /* Reset USCIxx registers and disable USCIxx by setting the UCSWRST
//     * bit of UCxxCTLW0.
//     */
//    UCxxCTL1 |= UCSWRST;
//
//    UCxxCTL0 |= UCMSB    | /* (Optional) Configure the USCI to send MSB first. */
//                UCMST    | /* Select master mode. */
//                UCSYNC   | /* Enable synchronous mode. */
//                //UCCKPL   | /* Clock polarity: the inactive state is high. */
//                UCCKPH   | /* Clock phase */
//                UCMODE_0;  /* 3-pin SPI mode */
//
//    /* Set clock source to SMCLK. (If we don't set a clock source,
//     * UCSSEL will default to 0, which is "reserved"; i.e., not tied to any
//     * clock source. (Behavior undefined afaik.)
//     */
//    UCxxCTL1 |= UCSSEL_2;
//    UCxxBR0  = 0x02; /* divide by 2; this gives us ~0.5 MHz operation */
//    UCxxBR1  = 0;
//
//    /* Clear UCSWRST to take the USCI out of the reset condition. */
//    UCxxCTL1 &= ~UCSWRST;
//}
//
//
///**
// * Sends an individual byte of data over SPI.
// */
//void SPI_FUNC(blockingSendByte)(const uint8_t data)
//{
//    UCxxTXBUF = data;
//
//    /* Note: The MSP430x5xx and MSP430x6xx Family Guide, Rev. O (slau208o)
//     * erronously refers to UCAxSTAT as 'UCAxSTATW'. There is no 'W' suffix
//     * in the preprocessor macros found in the MSP430 development headers.
//     */
//    while (UCxxSTAT & UCBUSY) {} /* Wait for the send to complete. */
//}
//
///**
// * Sends a sequence of data over SPI.
// */
//void SPI_FUNC(blockingSendSeq)(uint8_t const* data, const int len)
//{
//    int i;
//
//    for (i = 0; i < len; i++) {
//        SPI_FUNC(blockingSendByte)(*data++);
//    }
//}
//
//void SPI_FUNC(blockingReadSeq)(uint8_t *reply, const int len)
//{
//    int i;
//
//    for (i = 0; i < len; i++) {
//        SPI_FUNC(blockingSendByte)(0xFF);
//        *reply++ = UCxxRXBUF;
//    }
//}
//
//void SPI_FUNC(assert)()
//{
//    UCxxSTE_OUT &= ~UCxxSTE_BIT;
//}
//
//void SPI_FUNC(deassert)()
//{
//    UCxxSTE_OUT |= UCxxSTE_BIT;
//}
