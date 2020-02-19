#include "./poly8.c"

#include <stdio.h>
#define assertEqual(a, b, msg)                                 \
                                                               \
    if ((a) != (b))                                            \
    {                                                          \
        printf("\n");                                          \
        printf("Asserted=0x%08x Obtained=0x%08x\n", (b), (a)); \
        printf("Error with: ");                                \
        printf(msg);                                           \
        printf("\n");                                          \
        return 100;                                            \
    }
;

int poly8_test()
{
    assertEqual(poly, _inverse_bits[poly_inversed], "Inversed bits");

    assertEqual(
        poly_multiple_inversed(
            _inverse_bits[0b001100],
            _inverse_bits[0b1100001]),
        _inverse_bits[0b10111010],
        "Some random poly");

    assertEqual(mix_column_0(0xdb, 0x13, 0x53, 0x45), 0x8e, "mix_column_0");

    return 0;
};
