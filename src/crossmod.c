#include "digenerator.h"

#define BUFSIZE 64

const int8_t sine[BUFSIZE] = {
      0,  12,  25,  37,  49,  61,  72,  82,  91, 100, 107, 113, 119, 123, 126, 127,
    127, 127, 124, 121, 116, 110, 103,  95,  87,  77,  66,  55,  43,  31,  19,   6,
     -7, -20, -32, -44, -56, -67, -78, -88, -96,-104,-111,-117,-122,-125,-128,-128,
   -128,-127,-124,-120,-114,-108,-101, -92, -83, -73, -62, -50, -38, -26, -13,  -1
};

volatile uint8_t count = 0, resync = 1;

VCO_INTERRUPT()
{
    count = (count + 1 + !OPTB * 3) & (BUFSIZE - 1);
    uint8_t glitch = (INGATES + reverse(INGATES)) & 0xE7;
    int8_t  sample = sine[count] ^ glitch;
    int16_t modulator = (int16_t) CV - 128;
    if (modulator > -2 && modulator < 2) modulator = 0;
    if (sample > modulator) OUTGATE_ON;
    else OUTGATE_OFF;
    int16_t result = (OPTA) ? ifoldAdd(modulator, sample)
                            : (modulator * sample) >> 8;
    dac((uint8_t) result + 128);
}

GATE_INTERRUPT()
{
    if (INGATE1 && resync) {
        count  = 0; 
        resync = 0;
    }
}

int main()
{
    setup();
    for(;;) {
        if (!INGATE1) resync = 1;
    }
}
