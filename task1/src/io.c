#include "io.h"

void io_init(void)
{
    // Enable pull-up setting for input pins
    MCUCR &= ~(1<<PUD);

    // TODO: look up setting of IO pins

    // JTAG outputs
 	DDR (JTAG_CLK_PORT) |= JTAG_CLK_PIN;
    DDR (JTAG_TMS_PORT) |= JTAG_TMS_PIN; 
    DDR (JTAG_TDO_PORT) |= JTAG_TDO_PIN; 
    DDR (JTAG_TRST_PORT) |= JTAG_TRST_PIN; 
    // JTAG input
    DDR (JTAG_TDI_PORT) &= ~JTAG_TDI_PIN;
	PORT (JTAG_TDI_PORT) &= ~JTAG_TDI_PIN; // disable pull-up 
	
}

uint8_t io_getPin( uint8_t port, uint8_t pin ){
    return (PIN(port)&pin)>0;
}


void io_setPin( uint8_t port, uint8_t pin, uint8_t value ){
    if (value > 0)
        PORT(port) |= pin;
    else
        PORT(port) &= ~(pin);   
}
