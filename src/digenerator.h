#ifndef __DGR_H_
#define __DGR_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

#define B(n) (1 << n)
#define INLINE(type) static inline type __attribute__((always_inline))

#define INGATES (PINC & 0x0F)
#define INGATE1 (PINC & 0x01)
#define INGATE2 (PINC & 0x02)
#define INGATE3 (PINC & 0x04)
#define INGATE4 (PINC & 0x08)

#define OUTGATE_ON   (PORTB |=  0x10)
#define OUTGATE_OFF  (PORTB &= ~0x10)
#define OUTGATE_FLIP (PORTB ^=  0x10)

#define OPTS ((PINC & 0x30) >> 4)
#define OPTA  (PINC & 0x10)
#define OPTB  (PINC & 0x20)

#define CV ADCH

#define GATE_INTERRUPT(...) ISR(INT0_vect, ##__VA_ARGS__)
#define VCO_INTERRUPT(...)  ISR(INT1_vect, ##__VA_ARGS__)

uint16_t random();
uint8_t  reverse(uint8_t);
uint8_t  div3(uint8_t);
void     dac(uint8_t);
void     setup();

INLINE(uint8_t) max(uint8_t x, uint8_t y) { return (x > y) ? x : y; }
INLINE(uint8_t) min(uint8_t x, uint8_t y) { return (x < y) ? x : y; }
INLINE(uint8_t) diff(uint8_t x, uint8_t y) { return (y > x) ? (y - x) : (x - y); }
INLINE(uint8_t) clipAdd(uint8_t x, uint8_t y) { return (y >= (255 - x)) ? 255 : (x + y); }
INLINE(uint8_t) clipSub(uint8_t x, uint8_t y) { return (y >= x) ? 0 : (x - y); }
INLINE(uint8_t) foldAdd(uint8_t x, uint8_t y) { return (y > (255 - x)) ? 0 - (x + y) : (x + y); }
INLINE(int16_t) ifoldAdd(int8_t x, int8_t y)
{ 
    int16_t res = (int16_t) x + (int16_t) y;
    return (res >  127) ?  127 - ( res - 127)
         : (res < -128) ? -128 + (-128 - res)
         :  res;
}
INLINE(uint8_t) xfade(uint8_t x, uint8_t y, uint8_t i) { 
    i &= 8;
    return clipAdd((x >> 3) * (8 - i), (y >> 3) * i);
}

#endif // __DGR_H_
