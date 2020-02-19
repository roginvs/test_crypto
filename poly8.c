#include <stdint.h>

/** Table of bytes with bits in inverse order */
uint8_t _inverse_bits[0x100];

uint8_t _inverse_bits_for_byte(uint8_t a)
{
       uint8_t r = 0;
       for (uint8_t i = 0; i < 8; i++)
       {
              if ((a >> i) & 1)
              {
                     r = r + (1 << (7 - i));
              }
       }
       return r;
};

void _init_inverse_bits_table()
{
       _inverse_bits[0x00] = _inverse_bits_for_byte(0x00);
       for (uint8_t i = 0xFF; i > 0; i--)
       {
              _inverse_bits[i] = _inverse_bits_for_byte(i);
       };
};

uint8_t poly = 0b00011011;
uint8_t poly_inversed = 0b11011000;

/** Multiplication of two polynoms in GF2, bits are inversed */
uint8_t poly_multiple_inversed(uint8_t a, uint8_t b)
{

       uint8_t window = 0;

       // Go through highest degree to lowest
       for (uint8_t i = 0; i < 8; i++)
       {
              uint8_t bit_value = (b >> i) & 1;

              // printf("Bit as pos %i value=%i\n", i, bit_value);
              if (bit_value == 1)
              {
                     // Bit is set. This means that we should add
                     //  (a * x^(31-i)) % poly
                     // into result

                     // Our window is shifted left now for 31-i items
                     // This means it is already multiplied by x^(31-i)
                     // So, we just adding "a" into window

                     // printf("  Adding 'a' to window. Before=0x%08x after=0x%08x\n", window, window ^ a);
                     window = window ^ a;
              }

              // If it is not the last cycle, then we need to shift window
              // TODO: We can optimize this by altering loop, i.e.
              //  we can have a loop with 31 iteraction but do copy-pasted things before the loop.
              //  This might bring some performance.
              if (i != 7)
              {
                     // Now prepare for shift
                     uint8_t window_highest_degree_bit = window & 1;
                     // printf("  window=0x%08x shiftedWindow=0x%08x shiftedBit=%i\n", window, window >> 1, window_highest_degree_bit);
                     window = window >> 1;
                     if (window_highest_degree_bit == 1)
                     {
                            window = window ^ poly_inversed;
                            // printf("  windowAfterPolyXor=0x%08x\n", window);
                     }
              }
       }

       return window;
};

/** Multiplication of two polynoms in GF2 */
uint8_t poly_multiple(uint8_t a, uint8_t b)
{
       // TODO: Implement without inversed bits
       uint8_t m = poly_multiple_inversed(
           _inverse_bits[a],
           _inverse_bits[b]);
       return _inverse_bits[m];
};

uint8_t mix_column_0(uint8_t b0, uint8_t b1, uint8_t b2, uint8_t b3)
{
       return poly_multiple(
                  0x2,
                  b0) ^
              poly_multiple(
                  0x3,
                  b1) ^
              b2 ^
              b3;
};

uint8_t mix_column_1(uint8_t b0, uint8_t b1, uint8_t b2, uint8_t b3)
{
       return b0 ^
              poly_multiple(
                  0x2,
                  b1) ^
              poly_multiple(
                  0x3,
                  b2) ^
              b3;
};

uint8_t mix_column_2(uint8_t b0, uint8_t b1, uint8_t b2, uint8_t b3)
{
       return b0 ^ b1 ^
              poly_multiple(
                  0x2,
                  b2) ^
              poly_multiple(
                  0x3,
                  b3);
};

uint8_t mix_column_3(uint8_t b0, uint8_t b1, uint8_t b2, uint8_t b3)
{
       return poly_multiple(
                  0x3,
                  b0) ^
              b1 ^ b2 ^
              poly_multiple(
                  0x2,
                  b3);
};