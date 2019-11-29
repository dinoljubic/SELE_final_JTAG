#ifndef IO_H
#define IO_H

#include <avr/io.h>

// Universal access to IO ports - do not modify this block
#define IO_PORTB        ((uint8_t) 0x03)
#define IO_PORTC        ((uint8_t) 0x06)
#define IO_PORTD        ((uint8_t) 0x09)

#define PIN( IO_PORT )     _SFR_IO8( IO_PORT + 0 )
#define DDR( IO_PORT )     _SFR_IO8( IO_PORT + 1 )
#define PORT( IO_PORT )    _SFR_IO8( IO_PORT + 2 )

#define IO_PIN0         (1 << 0)
#define IO_PIN1         (1 << 1)
#define IO_PIN2         (1 << 2)
#define IO_PIN3         (1 << 3)
#define IO_PIN4         (1 << 4)
#define IO_PIN5         (1 << 5)
#define IO_PIN6         (1 << 6)
#define IO_PIN7         (1 << 7)


// Pin definitions
#define	JTAG_TRST_PIN   IO_PIN0
#define	JTAG_TRST_PORT  IO_PORTB
#define	JTAG_CLK_PIN    IO_PIN1
#define	JTAG_CLK_PORT   IO_PORTB
#define JTAG_TMS_PIN    IO_PIN2
#define JTAG_TMS_PORT   IO_PORTB
#define	JTAG_TDI_PIN    IO_PIN3
#define	JTAG_TDI_PORT   IO_PORTB
#define	JTAG_TDO_PIN    IO_PIN4
#define	JTAG_TDO_PORT   IO_PORTB


// Function declarations
void io_init(void);
uint8_t io_getPin( uint8_t port, uint8_t pin );
void io_setPin( uint8_t port, uint8_t pin, uint8_t value );

#endif /* IO_H */
