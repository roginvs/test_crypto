/*

gcc -o lfsr-galois.out lfsr-galois.c && ./lfsr-galois.out

*/

#include <stdio.h>
#include <stdint.h>
#include <limits.h>

void test_lfsr()
{
    typedef uint16_t POLYNOM;

    POLYNOM start_state = 0x1; /* Any nonzero start state will work. */
    POLYNOM lfsr = start_state;

    // This is a "x^n + x^1 + 1" polynom
    // For 16 bits it is equal to 0b1100000000000000
    // (Lowest degree is highest bit)
    POLYNOM const polynom_mask = (POLYNOM)0b11 << ((sizeof(POLYNOM) * CHAR_BIT) - 2);

    // This is an example from wikipedia, deg=16
    // x^16 + x^5 + x^3 + x^2 + 1
    // POLYNOM const polynom_mask = 0b1011010000000000;

    // An irreducible polynomial from
    // https://www.partow.net/programming/polynomials/index.html#deg16
    // x^16 + x^12 + x^3 + x^1 + 1
    // POLYNOM const polynom_mask = 0b1101000000001000;
    // x^16 + x^15 + x^11 + x^10 + x^9 + x^8 + x^6 + x^4 + x^2 + x^1 + 1
    // POLYNOM const polynom_mask = 0b1110101011110001;

    unsigned long period = 0;

    do
    {
        lfsr = (lfsr >> 1) ^ (-(lfsr & 1u) & polynom_mask);

        ++period;
    } while (lfsr != start_state);

    unsigned long maximum_possible_period = (1l << (sizeof(POLYNOM) * CHAR_BIT)) - 1;
    printf("Maximum possible period = %lu\n", maximum_possible_period);
    printf("Observed period = %lu\n", period);
}

int main()
{
    test_lfsr();

    return 0;
}