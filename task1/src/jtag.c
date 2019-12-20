#include "jtag.h"
#include "serial.h"

static void jtag_clearBitStream( void );

uint8_t bitStream[JTAG_LL_MAX_BITSTREAM_LEN/8];

void jtag_setPin( uint8_t PinNumber, uint8_t value){
    jtag_clearBitStream();
    //jtag_ll_resetAndIdle();
    uint8_t d = JTAG_INSTR_EXTEST;
    jtag_ll_setIR(JTAG_INSTR_L, &d);
    jtag_ll_getDR(JTAG_DATA_BDSCAN_L, bitStream);
    //serial_transmit(JTAG_DATA_BDSCAN_L/8, bitStream);
    bitStream[(PinNumber+1)/8] |= (1<<((PinNumber+1)%8));
    if (value > 0)
        bitStream[PinNumber/8] |= (1<<(PinNumber%8));
    else
        bitStream[PinNumber/8] &= ~(1<<(PinNumber%8));
    jtag_ll_setDR(JTAG_DATA_BDSCAN_L, bitStream);
    jtag_clearBitStream();
}

uint8_t jtag_getPin( uint8_t PinNumber ){
    jtag_clearBitStream();
    uint8_t d = JTAG_INSTR_EXTEST;
    //jtag_ll_resetAndIdle();
    jtag_ll_setIR(JTAG_INSTR_L, &d);
    jtag_ll_getDR(JTAG_DATA_BDSCAN_L, bitStream);
    uint8_t value = (uint8_t)((bitStream[PinNumber/8] & (1<<(PinNumber%8))) != 0);
    jtag_clearBitStream();
    return value;
}

uint8_t jtag_getIDCode( uint8_t *data ){
    uint8_t d = JTAG_INSTR_IDCODE;
    jtag_clearBitStream();
    //jtag_ll_resetAndIdle();
    jtag_ll_setIR(JTAG_INSTR_L, &d);
    jtag_ll_getDR(JTAG_DATA_IDCODE_L, bitStream);
    serial_transmitHex( bitStream, 5 );
    jtag_clearBitStream();
    return JTAG_DATA_IDCODE_L;
}

void jtag_setAll( void ){
    for (uint8_t i=0;i<7;i++)
        bitStream[i]=0xff;
    uint8_t d = JTAG_INSTR_EXTEST;

    //jtag_ll_resetAndIdle();
    jtag_ll_setIR(JTAG_INSTR_L, &d);
    jtag_ll_setDR(JTAG_DATA_BDSCAN_L, bitStream);
}

// Static functions
static void jtag_clearBitStream( void ){
    for (uint8_t i=0;i<JTAG_LL_MAX_BITSTREAM_LEN/8;i++)
        bitStream[i]=0;
}
