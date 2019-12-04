#ifndef SERIAL_H
#define SERIAL_H

#include "io.h"
//#include <avr/io.h>
#include <avr/interrupt.h>

#define	BAUD 57600                   // Desired baud rate
//#define BAUDGEN ((16000000/(16*BAUD))-1)  // Calculated divider (may under/overflow for some cases)
// Pre-calculated BAUDGEN for 9600 baud
#define BAUDGEN 103

void serial_init( void );
void serial_transmit( uint8_t size, uint8_t *data );

ISR (USART_RX_vect);

#endif /* SERIAL_H */
