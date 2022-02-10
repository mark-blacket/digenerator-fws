#include "digenerator.h"

#define BUFSIZE 1024

volatile uint8_t  buffer[BUFSIZE] = {0};
volatile uint8_t  lofi = 0, direction = 0;
volatile uint16_t count = 0;

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
    buffer[n >> 1] = (n & 1) ? (buffer[n >> 1] & 0xF0) + (x & 0x0F)
                             : (buffer[n >> 1] & 0x0F) + (x << 4);
}

VCO_INTERRUPT()
{
    int16_t tmp = (int16_t) CV - 128;
    uint8_t input = (uint8_t) (tmp < 0 ? -tmp : tmp);
    input = clipAdd(input, input);
    if (input <= 3) input = 0;

    if (INGATE1) {}
    else if (OPTA) {
        if (!lofi) {
            lofi = 1;
            count >>= 1;
        }
        if (!INGATE2)
            writeNib(count, clipAdd(input, readNib(count) << (OPTB ? 3 : 1)) >> 4);
        count = updateCount(count, (BUFSIZE << 1) - 1);
        if (count >= (BUFSIZE << 1) - 4 || count < 4) OUTGATE_ON;
        else OUTGATE_OFF;
        dac(readNib(count) << 4);
    } else {
        if (lofi) {
            lofi = 0;
            count <<= 1;
        }
        if (!INGATE2)
            buffer[count] = clipAdd(input, buffer[count] >> (OPTB ? 1 : 2));
        count = updateCount(count, BUFSIZE - 1);
        if (count >= BUFSIZE - 4 || count < 4) OUTGATE_ON;
        else OUTGATE_OFF;
        dac(buffer[count]);
    }
}

GATE_INTERRUPT()
{
    if (INGATE3) count = 0;
    if (INGATE4) direction ^= 1;
}

int main() { setup(); for(;;) {} }
