#ifndef JTAG_H
#define JTAG_H

#include "jtag_ll.h"

#define JTAG_INSTR_L            5
#define JTAG_INSTR_IDCODE       0b00001
#define JTAG_INSTR_EXTEST       0b00110
#define JTAG_INSTR_BYPASS       0b11111
#define JTAG_INSTR_SANDPR       0b00010

// Width of boundary scan registers
#define JTAG_DATA_BDSCAN_L      148
#define JTAG_DATA_IDCODE_L      32

// PIC32 pin defines
#define PIC32_LED_CELL           19 // output cell on LED
#define PIC32_BUTTON_CELL        3 // input cell on btn

void jtag_setPin( uint8_t PinNumber, uint8_t value);
uint8_t jtag_getPin( uint8_t PinNumber );
uint8_t jtag_getIDCode( uint8_t *data );
void jtag_setAll( void );

#endif /* JTAG_H */
