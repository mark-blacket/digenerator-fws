#include "digenerator.h"

#define BUFSIZE 1024

volatile uint8_t  buffer[BUFSIZE] = {0};
volatile uint8_t  lofi = 0, direction = 0, resync = 0;
volatile uint16_t count = 0;

INLINE(uint16_t) updateCount(uint16_t x, uint16_t limit)
{
    return (direction ? (x + 1) : (x - 1)) & limit;
}
INLINE(uint8_t) readNib(uint16_t n)
{
    return (n & 1) ? buffer[n >> 1] << 4
                   : buffer[n >> 1] & 0xF0;
}
INLINE(void) writeNib(uint16_t n, uint8_t x)
{
    buffer[n >> 1] = (n & 1) ? (buffer[n >> 1] & 0xF0) + (x >> 4)
                             : (buffer[n >> 1] & 0x0F) + (x & 0xF0);
}

VCO_INTERRUPT(ISR_NOBLOCK)
{
    if (INGATE1) {}
    else if (OPTA) {
        if (!lofi) {
            lofi = 1;
            count <<= 1;
        }
        if (!INGATE2)
            writeNib(count, OPTB ? xfade(CV, readNib(count), 3) : CV);
        count = updateCount(count, (BUFSIZE << 1) - 1);
        if (count >= (BUFSIZE << 1) - 2 || count < 2) OUTGATE_ON;
        else OUTGATE_OFF;
        dac(readNib(count));
    } else {
        if (lofi) {
            lofi = 0;
            count >>= 1;
        }
        if (!INGATE2)
            buffer[count] = OPTB ? xfade(CV, buffer[count], 3) : CV;
        count = updateCount(count, BUFSIZE - 1);
        if (count >= (BUFSIZE - 2) || count < 2) OUTGATE_ON;
        else OUTGATE_OFF;
        dac(buffer[count]);
    }
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
