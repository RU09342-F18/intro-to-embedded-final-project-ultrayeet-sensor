
/*
 * max7221.h
 *
 *  Created on: Jan 23, 2017
 *      Author: Nicholas DeCicco <nsd.cicco@gmail.com>
 *                               <deciccon0@students.rowan.edu>
 */

#ifndef MAX7221_H
#define MAX7221_H

#define REG_NOP        0x00
#define REG_D0         0x01
#define REG_D1         0x02
#define REG_D2         0x03
#define REG_D3         0x04
#define REG_D4         0x05
#define REG_D5         0x06
#define REG_D6         0x07
#define REG_D7         0x08
#define REG_DECODE     0x09
#define REG_INTENSITY  0x0A
#define REG_SCAN_LIMIT 0x0B
#define REG_SHUTDOWN   0x0C
#define REG_DISP_TEST  0x0F

#define DECIMAL_POINT 0x80

void max7221_(SPI const*const spi);
void max7221_init(SPI const*const spi, uint8_t chainLength,
                  const uint8_t brightness);
void max7221_setBrightness(SPI const*const spi, uint8_t chainLength,
                           const uint8_t brightness);
void max7221_useCodeBFont(SPI const*const spi, uint8_t chainLength);
void max7221_write(SPI const*const spi, const uint8_t str[], const uint8_t len);
void max7221_sendChainCommand(SPI const*const spi, uint8_t chainLength,
                              const uint8_t reg, const uint8_t value);
void max7221_shutdown(SPI const*const spi, uint8_t chainLength);
void max7221_powerOn(SPI const*const spi, uint8_t chainLength);

#endif
