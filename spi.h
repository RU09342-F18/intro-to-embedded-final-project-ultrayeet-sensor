/*
 * spi.h
 *
 *  Created on: Oct 29, 2016
 *      Author: Nicholas DeCicco <nsd.cicco@gmail.com>
 *                               <deciccon0@students.rowan.edu>
 */

#ifndef SPI_H
#define SPI_H

#define DECLARE_SPI_PROTOTYPES \
void SPI_FUNC(init)(); \
void SPI_FUNC(blockingSendSeq)(uint8_t const* data, const int len); \
void SPI_FUNC(blockingSendByte)(const uint8_t data); \
void SPI_FUNC(blockingReadSeq)(uint8_t *reply, const int len); \
void SPI_FUNC(assert)(); \
void SPI_FUNC(deassert)();

#define SPI_FUNC(x) spiB0_ ## x
DECLARE_SPI_PROTOTYPES

//#undef SPI_FUNC
//#define SPI_FUNC(x) spiB1_ ## x
//DECLARE_SPI_PROTOTYPES

typedef struct{
#if 1
    void (*init)();
    void (*blockingSendSeq)(uint8_t const* data, const int len);
    void (*blockingSendByte)(const uint8_t data);
    void (*blockingReadSeq)(uint8_t *reply, const int len);
    void (*assert)();
    void (*deassert)();
#else
#undef SPI_FUNC
#define SPI_FUNC(x) (*x)
DECLARE_SPI_PROTOTYPES
#endif
} SPI ;

#endif /* SPI_H */
