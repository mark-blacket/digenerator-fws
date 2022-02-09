#include "digenerator.h"

#define BUFSIZE 1024

volatile uint8_t  buffer[BUFSIZE] = {0};
volatile uint8_t  lofi = 0, direction = 0;
volatile uint16_t count = 0;

// cv - input
// opt a - 8bit / 4bit
// opt b - short / long fb
// gate 1 - stop
// gate 2 - freeze buffer
// gate 3 - retrigger
// gate 4 - reverse
// gate out - buffer start

INLINE(uint16_t) updateCount(uint16_t x, uint16_t limit)
{
    return (direction ? (x + 1) : (x - 1)) & limit;
}
INLINE(uint8_t) readNib(uint16_t n)
{
    return (n & 1) ? buffer[n >> 1] & 0x0F
                   : buffer[n >> 1] >> 4;
}
INLINE(void) writeNib(uint16_t n, uint8_t x)
{
    buffer[n >> 1] |= (n & 1) ? x & 0x0F
                              : x << 4;
}

VCO_INTERRUPT()
{
    if (INGATE1) reti();
    if (OPTA) {
        if (!lofi) {
            lofi = 1;
            count >>= 1;
        }
        if (!INGATE2)
            writeNib(count, clipAdd(CV >> 4, readNib(count) >> (OPTB ? 1 : 2)));
        count = updateCount(count, (BUFSIZE << 1) - 1);
        dac(readNib(count) << 4);
    } else {
        if (lofi) {
            lofi = 0;
            count <<= 1;
        }
        if (!INGATE2)
            buffer[count] = clipAdd(CV, buffer[count] >> (OPTB ? 1 : 2));
        count = updateCount(count, BUFSIZE - 1);
        dac(buffer[count]);
    }
}

GATE_INTERRUPT()
{
    if (INGATE3) count = 0;
    if (INGATE4) direction ^= 1;
}

int main() { setup(); for(;;) {} }
