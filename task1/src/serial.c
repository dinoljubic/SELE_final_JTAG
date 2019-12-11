#include "serial.h"
#include "jtag.h"

static void usart_init( void ){
    UBRR0 = BAUDGEN;
    // enable reciver, enable recive interrupt, 8 bit mode
    UCSR0B = (1 << RXEN0) | (1 << RXCIE0) | (1 << TXEN0);
    // 1 stop bit, 8 bit mode
    UCSR0C = (1 << UCSZ00) | (1 << UCSZ01);
    //UCSR0C = (1 << USBS0) | (3 << UCSZ00);

    sei();
}

static void usart_transmit( uint8_t data ){
    /* Wait for empty transmit buffer */
    while (!(UCSR0A & (1<<UDRE0)));
    /* Put data into buffer, sends the data */
    UDR0 = data;
}

static uint8_t usart_receive( void ){
    /* Wait for data to be received */
    while (!(UCSR0A & (1<<RXC0)));
    /* Get and return received data from buffer */
    uint8_t udr = UDR0;
    usart_transmit(udr);
    return udr;
}

void serial_init( void ){
    usart_init(  );

    serial_transmit(19, "USART initialized.");
}

void serial_transmit( uint8_t size, uint8_t *data ){
    for (uint8_t i=0;i<size;i++){
        usart_transmit( data[i] );
    }
}

static void serial_data_parser( uint8_t data ){
    uint32_t read;
    switch(data){
    case 'd':
        jtag_getIDCode((uint8_t*)&read);
        serial_transmit(sizeof(read), (uint8_t*)&read);
        break;
    case '0':
        jtag_setPin(PIC32_LED_PIN, 0);
        break;
    case '1':
        jtag_setPin(PIC32_LED_PIN, 1);
        break;
    case 'b':
        data = jtag_getPin(PIC32_BUTTON_PIN);
        serial_transmit(1, (uint8_t*)&read);
        break;
    }
}

ISR (USART_RX_vect) 
{
    uint8_t data;
    data = usart_receive();
    
    serial_data_parser(data);
}