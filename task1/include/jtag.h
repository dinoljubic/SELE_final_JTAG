#ifndef JTAG_H
#define JTAG_H

#include "jtag_ll.h"

// TODO: Not real values
#define JTAG_INSTR_L            8

#define JTAG_INSTR_IDCODE       0b00000000
#define JTAG_INSTR_EXTEST       0b00000000
#define JTAG_INSTR_BYPASS       0b00000001
#define JTAG_INSTR_SANDPR       0b10000001

#define JTAG_DATA_BDSCAN_L       32
#define JTAG_DATA_IDCODE_L       32

void jtag_setPin( uint8_t PinNumber, uint8_t value);
uint8_t jtag_getPin( uint8_t PinNumber );
uint8_t jtag_getIDCode( uint8_t *data );

#endif /* JTAG_H */
