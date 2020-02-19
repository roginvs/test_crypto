#include "./poly8.c"

#include <stdio.h>
#define assertEqual(a, b, msg)                                 \
                                                               \
    if ((a) != (b))                                            \
    {                                                          \
        printf("\n");                                          \
        printf("Asserted=0x%02x Obtained=0x%02x\n", (b), (a)); \
        printf("Error with: ");                                \
        printf(msg);                                           \
        printf("\n");                                          \
        return 100;                                            \
    }
;

#define assertMixColumn(d)                                                   \
    assertEqual(mix_column_0(d[0], d[1], d[2], d[3]), d[4], "mix column 0"); \
    assertEqual(mix_column_1(d[0], d[1], d[2], d[3]), d[5], "mix column 1"); \
    assertEqual(mix_column_2(d[0], d[1], d[2], d[3]), d[6], "mix column 2"); \
    assertEqual(mix_column_3(d[0], d[1], d[2], d[3]), d[7], "mix column 3"); \
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

    uint8_t test1[] = {0xdb, 0x13, 0x53, 0x45, 0x8e, 0x4d, 0xa1, 0xbc};
    assertMixColumn(test1);

    uint8_t test2[] = {0xd4, 0xbf, 0x5d, 0x30, 0x04, 0x66, 0x81, 0xe5};
    assertMixColumn(test2);

    return 0;
};
