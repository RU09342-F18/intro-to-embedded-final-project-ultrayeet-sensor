
/*
 * max7221.c
 *
 *  Created on: Jan 23, 2017
 *      Author: Nicholas DeCicco <nsd.cicco@gmail.com>
 *                               <deciccon0@students.rowan.edu>
 */

#include <msp430.h>
#include <stdint.h>
#include "spi.h"
#include "max7221.h"

#define MAX_CHAIN_LENGTH 3

/**
 *
 * @param chainLength The number of MAX7221's chained together.
 */
void max7221_init(SPI const*const spi, uint8_t chainLength,
                  const uint8_t brightness)
{
	max7221_setBrightness(spi, chainLength, brightness);
}

/**
 * Sets the display brightness.
 *
 * @param spi
 * @param chainLength
 * @param brightness
 */
void max7221_setBrightness(SPI const*const spi, uint8_t chainLength,
                           const uint8_t brightness)
{
	max7221_sendChainCommand(spi, chainLength, REG_INTENSITY, brightness);
}

/**
 * Sets the decode mode to use the Code B font.
 *
 * @param spi
 * @param chainLength
 */
void max7221_useCodeBFont(SPI const*const spi, uint8_t chainLength)
{
	max7221_sendChainCommand(spi, chainLength, REG_DECODE, 0xFF);
}

/**
 *
 *
 * @param spi
 * @param len
 */
void max7221_write(SPI const*const spi, const uint8_t str[], const uint8_t len)
{
	spi->assert();
	spi->blockingSendSeq(str, len);
	spi->deassert();
}

/**
 *
 */
void max7221_shutdown(SPI const*const spi, uint8_t chainLength)
{
	max7221_sendChainCommand(spi, chainLength, REG_SHUTDOWN, 0x00);
}

/**
 *
 */
void max7221_powerOn(SPI const*const spi, uint8_t chainLength)
{
	max7221_sendChainCommand(spi, chainLength, REG_SHUTDOWN, 0x01);
}

/**
 * @param spi
 * @param chainLength
 * @param reg
 * @param value
 */
void max7221_sendChainCommand(SPI const*const spi, uint8_t chainLength,
                              const uint8_t reg, const uint8_t value)
{
	uint8_t commandString[MAX_CHAIN_LENGTH*2];
	spi->assert();
	while (chainLength-- > 0) {
		commandString[chainLength*2+1] = value;
		commandString[chainLength*2+0] = reg;
	}
	spi->blockingSendSeq(commandString, MAX_CHAIN_LENGTH*2);
	spi->deassert();
}
