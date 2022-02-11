#include "digenerator.h"

#define BUFSIZE 32
#define LIMIT   ((BUFSIZE >> !!OPTA) - 1)
#define STEP    (1 + INGATE1)

const uint8_t sine[BUFSIZE] = {
    127, 103,  79,  56,  37,  21,   9,   2,   0,   2,   9,  21,  37,  56,  79, 103,
    127, 152, 176, 199, 218, 234, 246, 253, 255, 253, 246, 234, 218, 199, 176, 152 
};
const uint8_t square[BUFSIZE] = {
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,   0,   0,   0,   0
};
const uint8_t saw[BUFSIZE] = {
      0,   8,  16,  24,  32,  41,  49,  57,  65,  74,  82,  90,  98, 106, 115, 123,
    131, 139, 148, 156, 164, 172, 180, 189, 197, 205, 213, 222, 230, 238, 246, 255
};

volatile uint8_t var[BUFSIZE] = {0};
volatile uint8_t count = 0, direction = 0, resync = 1;

VCO_INTERRUPT(ISR_NOBLOCK)
{
    count = (direction ? (count - STEP) : (count + STEP)) & LIMIT; 
    if (!INGATE2) {
        uint8_t cmp = (uint8_t) random();
        if (cmp > 0xFB) cmp = 0xFB;
        var[count] = (cmp >= CV) ? square[count]
                   : (OPTB) ? saw[count]
                   : sine[count];
    }
    if ((CV >> 1) >= var[count]) OUTGATE_ON;
    else OUTGATE_OFF;
    dac(var[count] << ((OPTA) ? 1 : 0));
}

GATE_INTERRUPT()
{
    if (INGATE3 && resync) {
        count  = 0; 
        resync = 0;
    } else if (INGATE4 && resync) {
        direction ^= 1;
        resync = 0;
    }
}

int main()
{
    setup();
    for(;;) {
        if (!(INGATES & 0x0C)) resync = 1;
    }
}
