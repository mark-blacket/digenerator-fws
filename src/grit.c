#include "digenerator.h"

volatile uint8_t frames[8][32] = {
    {128, 152, 176, 198, 217, 233, 245, 252, 255, 252, 245, 233, 217, 198, 176, 152, 128, 103, 79, 57, 38, 22, 10, 3, 1, 3, 10, 22, 38, 57, 79, 103},
    {64, 84, 104, 123, 140, 156, 170, 182, 191, 198, 202, 204, 204, 203, 200, 196, 191, 171, 151, 132, 115, 99, 85, 73, 64, 57, 53, 51, 51, 52, 55, 59},
    {0, 16, 32, 48, 64, 80, 96, 112, 128, 144, 160, 176, 192, 208, 224, 240, 255, 240, 224, 208, 192, 176, 160, 144, 128, 112, 96, 80, 64, 48, 32, 16},
    {0, 12, 24, 36, 48, 60, 72, 84, 96, 108, 120, 132, 144, 156, 168, 180, 255, 243, 231, 219, 207, 195, 183, 171, 159, 147, 135, 123, 111, 99, 87, 75},
    {0, 8, 16, 24, 32, 40, 48, 56, 64, 72, 80, 88, 96, 104, 112, 120, 255, 247, 239, 231, 223, 215, 207, 199, 191, 183, 175, 167, 159, 151, 143, 135},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255},
    {0, 4, 8, 12, 16, 20, 24, 28, 32, 36, 40, 44, 48, 52, 56, 60, 191, 195, 199, 203, 207, 211, 215, 219, 223, 227, 231, 235, 239, 243, 247, 251},
    {0, 8, 16, 24, 32, 40, 48, 56, 64, 72, 80, 88, 96, 104, 112, 120, 128, 136, 144, 152, 160, 168, 176, 184, 192, 200, 208, 216, 224, 232, 240, 248}
};

volatile uint8_t wave = 0, fx = 0, counter = 0, canMutate = 1, canResync = 1;

// opt a - cut
// opt b - distortion type
// gate 1 - sync
// gate 2 - wave hold
// gate 3 - fx hold
// gate 4 - mutate
// cv - character
// gate out - sub

VCO_INTERRUPT()
{ 
    if (++counter == 32) {
        if (!INGATE2) wave = CV >> 5;
        if (!INGATE3) fx = CV;
        counter = 0;
    }

    if (counter == 0) OUTGATE_FLIP;

    uint8_t frame = frames[wave][counter];
    if (OPTB) {
        frame = (frame << 1)
              + (clipSub(frames[wave][(counter << 2) % 32], ~fx) << 1)
              + (frames[wave][(counter << 1) % 32] << 1)
              ^ (fx + 69);
    } else {
        frame ^= fx;
    }
    if (OPTA) dac(frame);
}

GATE_INTERRUPT()
{
    if (INGATE1 && canResync) {
        canResync = 0;
        counter = 0; 
    }
    if (INGATE4 && canMutate) {
        canMutate = 0;
        for (uint8_t x = 0; x < 8; x++) {
            uint8_t temp = random() >> 8;
            for (uint8_t y = 0; y < 32; y++) {
               if (temp > 200)
                   frames[x][y] = frames[x][y] + frames[(x + 4) % 8][y] + temp;
               else if (temp > 180)
                   frames[x][y] = (frames[x][y] >> 1) 
                                + (frames[(x + 5) % 8][y] >> 1);
               else
                   frames[x][y] += temp;
            } 
        }
    }
} 

int main()
{
    setup();
    for (;;) {
        if (!INGATE1) canResync = 1;
        if (!INGATE4) canMutate = 1;
    }
}
