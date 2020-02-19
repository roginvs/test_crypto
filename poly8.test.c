#include "./poly8.c"

#include <stdio.h>
#define assertEqual(a, b, msg)                                       \
    printf(msg);                                                     \
    printf("... ");                                                  \
    if ((a) != (b))                                                  \
    {                                                                \
        printf("FAIL: Asserted=0x%02x Obtained=0x%02x\n", (b), (a)); \
        return 100;                                                  \
    }                                                                \
    else                                                             \
    {                                                                \
        printf("ok\n");                                              \
    };

#define assertMixColumn(d, msg)                                   \
    printf("mix_column_0 ");                                      \
    assertEqual(mix_column_0(d[0], d[1], d[2], d[3]), d[4], msg); \
    printf("mix_column_1 ");                                      \
    assertEqual(mix_column_1(d[0], d[1], d[2], d[3]), d[5], msg); \
    printf("mix_column_2 ");                                      \
    assertEqual(mix_column_2(d[0], d[1], d[2], d[3]), d[6], msg); \
    printf("mix_column_3 ");                                      \
    assertEqual(mix_column_3(d[0], d[1], d[2], d[3]), d[7], msg); \
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
    assertMixColumn(test1, "from wikipedia");

    uint8_t test2[] = {0xd4, 0xbf, 0x5d, 0x30, 0x04, 0x66, 0x81, 0xe5};
    assertMixColumn(test2, "from nist 1");

    return 0;
};
