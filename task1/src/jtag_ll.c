#include "jtag_ll.h"


static void jtag_ll_fsmTransition( uint8_t stepNum, uint8_t tmsStream );
static void jtag_ll_clk( void );
static void jtag_ll_shiftData( uint8_t BitStreamSize, uint8_t *dataIn, uint8_t *dataOut );


void jtag_ll_resetAndIdle( void ){
    #ifndef JTAG_TRST_PIN
    io_setPin(JTAG_TMS_PORT, JTAG_TMS_PIN, 1);
    for (uint8_t i=0;i<5;i++){
        jtag_ll_clk(  );
    }
    #else
    io_setPin(JTAG_TRST_PORT, JTAG_TRST_PIN, 1);
    jtag_ll_clk();
    #endif
    // Move to Idle
    io_setPin(JTAG_TMS_PORT, JTAG_TMS_PIN, 0);
    jtag_ll_clk();
}


void jtag_ll_setIR( uint8_t BitStreamSize, uint8_t *data ){
    // Idle -> ShiftIR
    jtag_ll_fsmTransition(4, 0b0011);
    jtag_ll_shiftData(BitStreamSize, 0, data);
    // Exit1-IR -> Idle
    jtag_ll_fsmTransition(2, 0b10);
}

void jtag_ll_getIR( uint8_t BitStreamSize, uint8_t *data ){
    // Idle -> ShiftIR
    jtag_ll_fsmTransition(4, 0b0011);
    jtag_ll_shiftData(BitStreamSize, data, 0);
    // Exit1-IR -> Idle
    jtag_ll_fsmTransition(2, 0b10);
}

void jtag_ll_setDR( uint8_t BitStreamSize, uint8_t *data ){
    // Reset -> ShiftDR
    jtag_ll_fsmTransition(3, 0b001);
    jtag_ll_shiftData(BitStreamSize, 0, data);
    // Exit1-DR -> Idle
    jtag_ll_fsmTransition(2, 0b10);
}

void jtag_ll_getDR( uint8_t BitStreamSize, uint8_t *data ){
    // Idle -> ShiftIR
    jtag_ll_fsmTransition(4, 0b0011);
    jtag_ll_shiftData(BitStreamSize, 0, data);
    // Exit1-DR -> Idle
    jtag_ll_fsmTransition(2, 0b10);
}


// Static functions - no need to use them outside this module

static void jtag_ll_fsmTransition( uint8_t stepNum, uint8_t tmsStream ){
    uint8_t bit, value;
    for (bit=0;bit<stepNum;bit++){
        value = tmsStream & (1<<bit);
        io_setPin(JTAG_TMS_PORT, JTAG_TMS_PIN, value);
        jtag_ll_clk();
    }
}

static void jtag_ll_clk( void ){
    io_setPin(JTAG_CLK_PORT, JTAG_CLK_PIN, 1);
    io_setPin(JTAG_CLK_PORT, JTAG_CLK_PIN, 0);
}

static void jtag_ll_shiftData( uint8_t BitStreamSize, uint8_t *dataIn, uint8_t *dataOut ){

    /* This function needs to control the clock impulse for exiting Shift state
     * because the exit transition shifts one more bit and this is necessary for
     * input capture.
     * Clock for entry - TBD (probably not for consistency)
     */

    uint8_t value, readVal, i;

    // TODO: TBD if these are declared as static
    static uint8_t inBuffer[JTAG_LL_MAX_BITSTREAM_LEN/8];
    static uint8_t outBuffer[JTAG_LL_MAX_BITSTREAM_LEN/8];

    if (dataIn == 0)
        dataIn = inBuffer;
    if (dataOut == 0)
        dataOut = outBuffer;

    /* 
    // Skipped, assuming that function only exits state, does not enter into it

    // If BitStreamSize is 0, then skip data shift, go straight to Exit-1
    if (BitStreamSize == 0){
        io_setPin(JTAG_TMS_PORT, JTAG_TMS_PIN, 1);
        jtag_ll_clk(  );
        return;
    }

    // Assert TMS to low and enter Shift
    io_setPin(JTAG_TMS_PORT, JTAG_TMS_PIN, 0);
    jtag_ll_clk();
    */

    uint8_t byte, bit;

    for (i=0;i<BitStreamSize;i++){
        byte = i/8;
        bit = i%8;
 
        // Before asserting last bit to TDO, TMS ust be set to 1 to go to Exit-1
        if(i == BitStreamSize-1){
            io_setPin(JTAG_TMS_PORT, JTAG_TMS_PIN, 1);
        }
      
        value = dataOut[byte] & (1<<bit);
        io_setPin(JTAG_TDO_PORT, JTAG_TDO_PIN, value);

        jtag_ll_clk();

        readVal = io_getPin(JTAG_TDI_PORT, JTAG_TDI_PIN);
        dataIn[byte] |= (readVal<<bit);
    }
}

