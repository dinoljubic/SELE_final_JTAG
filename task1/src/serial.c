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

    serial_transmit( "JTAG driver for PIC32MX320F128H.", sizeof("JTAG driver for PIC32MX320F128H.") );
    serial_transmit("Controls:", sizeof("Controls:"));
    serial_transmit("0 - Turn on LED", sizeof("0 - Turn on LED"));
    serial_transmit("1 - Turn on LED", sizeof("1 - Turn on LED"));
    serial_transmit("d - display uC's IDCODE", sizeof("d - display uC's IDCODE"));
    serial_transmit("b - read button state", sizeof("b - read button state"));
}
    

void serial_transmit( char *data, uint8_t size ){
    for (uint8_t i=0;i<size;i++){
        usart_transmit( data[i] );
    }
    usart_transmit('\r');
    usart_transmit('\n');
}

void serial_transmitHex( uint8_t* data, uint8_t size ){
    usart_transmit('0');
    usart_transmit('x');
    uint8_t ch, nib;
    for (int8_t i=size-1;i>=0;i--){
        // First nibble
        nib = (data[i] & 0xf0) >> 4;
        if (nib<10)
            ch=nib+48;
        else
            ch=nib+55;
        usart_transmit(ch);

        // Second nibble
        nib = data[i] & 0x0f;
        if (nib<10)
            ch=nib+48;
        else
            ch=nib+55;
        usart_transmit(ch);
    }
    usart_transmit('\r');
    usart_transmit('\n');
}

static void serial_data_parser( uint8_t data ){
    uint32_t read;
    switch(data){
    case 'd':
        jtag_getIDCode((uint8_t*)&read);
        //serial_transmit(sizeof(read), (uint8_t*)&read);
        break;
    case '0':
        jtag_setPin(PIC32_LED_CELL, 0);
        break;
    case '1':
        jtag_setPin(PIC32_LED_CELL, 1);
        break;
    case 'b':
        data = jtag_getPin(PIC32_BUTTON_CELL);
        if (data==1)
            serial_transmit( " OFF", (uint8_t)4 );
        else if (data==0)
            serial_transmit( " ON", (uint8_t)3 );
        break;
    }
}

ISR (USART_RX_vect) 
{
    uint8_t data;
    data = usart_receive();
    
    serial_data_parser(data);
}