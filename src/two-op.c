#include "digenerator.h"

#define BUFSIZE 64
#define RESET   ((OPTA) ? (BUFSIZE >> 1) : BUFSIZE)

const uint8_t sine[BUFSIZE] = {
    128, 140, 152, 165, 176, 188, 199, 209, 218, 226, 234, 240, 246, 250, 253, 255,
    255, 255, 253, 250, 246, 240, 234, 226, 218, 209, 199, 188, 176, 165, 152, 140,
    128, 115, 103,  90,  79,  67,  56,  46,  37,  29,  21,  15,   9,   5,   2,   0,
      0,   0,   2,   5,   9,  15,  21,  29,  37,  46,  56,  67,  79,  90, 103, 115
};

volatile uint8_t count = 0, mcount = 0,
                 mode = 1, modeSwitch = 1,
                 sub = 0, subSwitch = 0;

// opt a - half-wave rectifier
// opt b - xor / pm
// gate 1 - suboctave
// gates 2-4 - mod osc octave shift (-1/0/1/2)
// cv - mod index

VCO_INTERRUPT()
{
    if (++count == RESET) {
        count = 0;
        mode = modeSwitch;
    }
    sub = (subSwitch) ? (sub ^ 1) : 0;
    if ((mcount += (sub * mode)) >= BUFSIZE) mcount = 0;

    dac((OPTB) ? sine[count + max(sine[mcount], CV)]
               : sine[count] ^ max(sine[mcount], CV));
}

GATE_INTERRUPT()
{
    subSwitch = INGATE1;
    uint8_t gates = INGATES >> 1;
    if (gates) modeSwitch = gates;
}

int main() { setup(); for (;;) {} }
