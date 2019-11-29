#include "serial.h"

void usart_init( void ){
    UBRR0 = BAUDGEN;
    // enable reciver, enable recive interrupt, 8 bit mode
    UCSR0B = (1 << RXEN0) | (1 << RXCIE0);
    // 1 stop bit, 8 bit mode
    UCSR0C = (1 << UCSZ00) | (1 << UCSZ01);

    sei();
}

void usart_transmit( uint8_t data ){
    /* Wait for empty transmit buffer */
    while (!(UCSR0A & (1<<UDRE0)));
    /* Put data into buffer, sends the data */
    UDR0 = data;
}

uint8_t usart_receive( void ){
    /* Wait for data to be received */
    while (!(UCSR0A & (1<<RXC0)));
    /* Get and return received data from buffer */
    return UDR0;
}

ISR (USART_RX_vect) 
{
    uint8_t data;
    data = usart_receive();
    // TODO: Store data in a buffer to be read by the parser, or disable interrupts and usart and call parser.
}