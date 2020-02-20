#include <stdint.h>

#ifndef __GALOIS_INCLUDED__
#define __GALOIS_INCLUDED__

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

#include <stdio.h>
void print_bits(uint8_t a)
{
       for (uint8_t i = 0; i < 8; i++)
       {
              printf((a >> (7 - i)) & 1 ? "1" : "0");
       };
};

uint8_t poly_divide(uint8_t a, uint8_t b, uint8_t a_have_highest_bit, uint8_t *q, uint8_t *r)
{
       // http://www.ee.unb.ca/cgi-bin/tervo/calc.pl
       printf("\nDivide ");
       printf(a_have_highest_bit ? "1." : "0.");
       print_bits(a);
       printf(" by ");
       print_bits(b);
       printf("\n");

       uint8_t local_q;
       uint8_t local_r;

       uint8_t next_q;
       uint8_t next_r;

       if (!a_have_highest_bit)
       {

              for (int8_t i = 7; i >= 0; i--)
              {
                     if ((a >> i) & 1)
                     {
                            for (int8_t ii = i; ii >= 0; ii--)
                            {
                                   if ((b >> ii) & 1)
                                   {
                                          printf("Found div i=%i ii=%ii\n", i, ii);
                                          local_q = 1 << (i - ii);
                                          local_r = a ^ (b << (i - ii));

                                          printf("Local_q = ");
                                          print_bits(local_q);
                                          printf("  local_r = ");
                                          print_bits(local_r);
                                          printf("\n");

                                          poly_divide(local_r, b, 0, &next_q, &next_r);
                                          *q = local_q ^ next_q;
                                          *r = next_r;
                                          printf("Return q = ");
                                          print_bits(*q);
                                          printf("  r = ");
                                          print_bits(*r);
                                          printf("\n");
                                          return 0;
                                   };
                            };
                            printf("Error: b is zero\n");
                            *q = 0;
                            *r = 0;
                            return 1;
                     };

                     if ((b >> i) & 1)
                     {
                            printf("Found non-zero at b at %i pos, b is greater than a. Finished\n", i);
                            // We found a non-zero value at b, but still not value at a
                            // This means search is over, deg(b) > deg(a)
                            *q = 0;
                            *r = a;
                            return 0;
                     };
              }

              printf("Lol: a is zero\n");
              *q = 0;
              *r = 0;
              return 0;
       }
       else
       {
              // TODO
              return 1;
       };
};

#endif