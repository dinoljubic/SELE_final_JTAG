#ifndef JTAG_LL_H
#define JTAG_LL_H

#include "io.h"

#define JTAG_LL_MAX_BITSTREAM_LEN  64

void jtag_ll_resetAndIdle( void );
void jtag_ll_setIR( uint8_t BitStreamSize, uint8_t *data );
void jtag_ll_getIR( uint8_t BitStreamSize, uint8_t *data );
void jtag_ll_setDR( uint8_t BitStreamSize, uint8_t *data );
void jtag_ll_getDR( uint8_t BitStreamSize, uint8_t *data );

#endif /* JTAG_LL_H */
