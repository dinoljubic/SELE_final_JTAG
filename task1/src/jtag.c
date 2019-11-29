#include "jtag.h"

static void jtag_clearBitStream( void );

uint8_t bitStream[JTAG_LL_MAX_BITSTREAM_LEN/8];

void jtag_setPin( uint8_t PinNumber, uint8_t value){
    uint8_t pinNumberReversed = JTAG_DATA_BDSCAN_L - PinNumber;
    jtag_ll_resetAndIdle();
    jtag_ll_setIR(JTAG_INSTR_L, JTAG_INSTR_EXTEST);
    jtag_ll_getDR(JTAG_DATA_BDSCAN_L, bitStream);
    if (value > 0)
        bitStream[pinNumberReversed/8] |= (1<<(pinNumberReversed%8));
    else
        bitStream[pinNumberReversed/8] &= ~(1<<(pinNumberReversed%8));
    jtag_ll_setDR(JTAG_DATA_BDSCAN_L, bitStream);
}

uint8_t jtag_getPin( uint8_t PinNumber ){
    uint8_t pinNumberReversed = JTAG_DATA_BDSCAN_L - PinNumber;
    jtag_ll_resetAndIdle();
    jtag_ll_setIR(JTAG_INSTR_L, JTAG_INSTR_EXTEST);
    jtag_ll_getDR(JTAG_DATA_BDSCAN_L, bitStream);

    // return 1 if bit is set, 0 otherwise
    return (bitStream[pinNumberReversed/8] & (1<<(pinNumberReversed%8))) > 0;
}

uint8_t jtag_getIDCode( uint8_t *data ){
    jtag_ll_resetAndIdle();
    jtag_ll_setIR(JTAG_INSTR_L, JTAG_INSTR_IDCODE);
    jtag_ll_getDR(JTAG_DATA_IDCODE_L, bitStream);
    data = bitStream;
    return JTAG_DATA_IDCODE_L;
}


// Static functions
static void jtag_clearBitStream( void ){
    for (uint8_t i=0;i<JTAG_LL_MAX_BITSTREAM_LEN/8;i++)
        bitStream[i]=0;
}
