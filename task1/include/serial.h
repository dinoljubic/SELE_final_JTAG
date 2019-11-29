#ifndef SERIAL_H
#define SERIAL_H

#include "io.h"
//#include <avr/io.h>
#include <avr/interrupt.h>

#define	BAUD 57600                   // Desired baud rate
#define BAUDGEN ((16000000/(16*BAUD))-1)  // Calculated divider (may under/overflow for some cases)

void usart_init( void );
void usart_transmit(uint8_t data);
uint8_t usart_receive( void );

ISR (USART_RX_vect);

#endif /* SERIAL_H */
