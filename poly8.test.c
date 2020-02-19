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
    _init_inverse_bits_table();

    assertEqual(_inverse_bits[0x00], 0x00, "0x00 inversed bits");
    assertEqual(_inverse_bits[0b00000001], 0b10000000, "0b00000001 inversed bits");
    assertEqual(_inverse_bits[0xFF], 0xFF, "0xFF inversed bits");
    assertEqual(_inverse_bits[0b11111110], 0b01111111, "0b11111110 inversed bits");

    assertEqual(poly, _inverse_bits[poly_inversed], "Inversed bits for poly are same");

    assertEqual(
        poly_multiple_inversed(
            _inverse_bits[0b001100],
            _inverse_bits[0b1100001]),
        _inverse_bits[0b10111010],
        "Some random poly");

    uint8_t test1[] = {0xdb, 0x13, 0x53, 0x45, 0x8e, 0x4d, 0xa1, 0xbc};
    assertMixColumn(test1, "from wikipedia");

    uint8_t test2[] = {0xd4, 0xbf, 0x5d, 0x30, 0x04, 0x66, 0x81, 0xe5};
    assertMixColumn(test2, "nist appendix B, round 1, column 0");

    uint8_t test3[] = {0xe0, 0xb4, 0x52, 0xae, 0xe0, 0xcb, 0x19, 0x9a};
    assertMixColumn(test3, "nist appendix B, round 1, column 1");

    uint8_t test4[] = {0xb8, 0x41, 0x11, 0xf1, 0x48, 0xf8, 0xd3, 0x7a};
    assertMixColumn(test4, "nist appendix B, round 1, column 2");

    uint8_t test5[] = {0x1e, 0x27, 0x98, 0xe5, 0x28, 0x06, 0x26, 0x4c};
    assertMixColumn(test5, "nist appendix B, round 1, column 3");

    return 0;
};
