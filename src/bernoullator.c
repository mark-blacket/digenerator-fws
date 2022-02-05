#include "digenerator.h"

#define BUFSIZE 64
#define LIMIT   ((BUFSIZE >> !OPTA) - 1)
#define STEP    (1 + INGATE1)

const uint8_t sine[BUFSIZE] = {
    128, 140, 152, 165, 176, 188, 199, 209, 218, 226, 234, 240, 246, 250, 253, 255,
    255, 255, 253, 250, 246, 240, 234, 226, 218, 209, 199, 188, 176, 165, 152, 140,
    128, 115, 103,  90,  79,  67,  56,  46,  37,  29,  21,  15,   9,   5,   2,   0,
      0,   0,   2,   5,   9,  15,  21,  29,  37,  46,  56,  67,  79,  90, 103, 115
};
const uint8_t square[BUFSIZE] = {
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0
};
const uint8_t saw[BUFSIZE] = {
      0,   4,   8,  12,  16,  20,  24,  28,  32,  36,  40,  44,  48,  52,  56,  60,
     65,  69,  73,  77,  81,  85,  89,  93,  97, 101, 105, 109, 113, 117, 121, 125,
    130, 134, 138, 142, 146, 150, 154, 158, 162, 166, 170, 174, 178, 182, 186, 190,
    195, 199, 203, 207, 211, 215, 219, 223, 227, 231, 235, 239, 243, 247, 251, 255
};

volatile uint8_t var[BUFSIZE] = {0};
volatile uint8_t count = 0, direction = 0;

// opt a - full-wave rectifier
// opt b - sine-square -> saw-square
// gate 1 - octave up
// gate 2 - hold waveform
// gate 3 - sync
// gate 4 - softsync
// cv - probability

VCO_INTERRUPT()
{
    count = ((direction) ? (count - STEP) : (count + STEP)) & LIMIT; 
    if (!INGATE2)
        var[count] = ((uint8_t) random() >= CV) ? square[count]
                   : (OPTB) ? saw[count]
                   : sine[count];
    dac(var[count]);
}

GATE_INTERRUPT()
{
    if (INGATE3) count = 0;
    else if (INGATE4) direction ^= 1;
}

int main() { setup(); for (;;) {} }
