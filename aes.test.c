#include <stdio.h>

#include "./aes.c"

#define assert_mix_columns(d, msg)                                                                            \
    printf(msg);                                                                                              \
    printf(": %02x %02x %02x %02x -> %02x %02x %02x %02x\n", d[0], d[1], d[2], d[3], d[4], d[5], d[6], d[7]); \
    assert_equal(mix_column_0(d[0], d[1], d[2], d[3]), d[4], "  mix_column_0");                               \
    assert_equal(mix_column_1(d[0], d[1], d[2], d[3]), d[5], "  mix_column_1");                               \
    assert_equal(mix_column_2(d[0], d[1], d[2], d[3]), d[6], "  mix_column_2");                               \
    assert_equal(mix_column_3(d[0], d[1], d[2], d[3]), d[7], "  mix_column_3");                               \
    ;

int aes_test()
{
    uint8_t test1[] = {0xdb, 0x13, 0x53, 0x45, 0x8e, 0x4d, 0xa1, 0xbc};
    assert_mix_columns(test1, "From wikipedia");

    uint8_t test2[] = {0xd4, 0xbf, 0x5d, 0x30, 0x04, 0x66, 0x81, 0xe5};
    assert_mix_columns(test2, "Nist appendix B, round 1, column 0");

    uint8_t test3[] = {0xe0, 0xb4, 0x52, 0xae, 0xe0, 0xcb, 0x19, 0x9a};
    assert_mix_columns(test3, "Nist appendix B, round 1, column 1");

    uint8_t test4[] = {0xb8, 0x41, 0x11, 0xf1, 0x48, 0xf8, 0xd3, 0x7a};
    assert_mix_columns(test4, "Nist appendix B, round 1, column 2");

    uint8_t test5[] = {0x1e, 0x27, 0x98, 0xe5, 0x28, 0x06, 0x26, 0x4c};
    assert_mix_columns(test5, "Nist appendix B, round 1, column 3");

    assert_equal(byte_cyclic_left_shift(0b00000001, 0), 0b1, "Left shift 1");
    assert_equal(byte_cyclic_left_shift(0b10000000, 1), 0b1, "Left shift 2");
    assert_equal(byte_cyclic_left_shift(0b1, 1), 0b10, "Left shift 2A");
    assert_equal(byte_cyclic_left_shift(0b10000001, 1), 0b11, "Left shift 3");
    assert_equal(byte_cyclic_left_shift(0b10011010, 4), 0b10101001, "Left shift 4");

    init_sbox();

    assert_equal(sbox[0x0], 0x63, "S-box 0x00");
    assert_equal(sbox[0x01], 0x7c, "S-box 0x01");
    assert_equal(sbox[0x02], 0x77, "S-box 0x02");
    assert_equal(sbox[0x60], 0xd0, "S-box 0x60");
    assert_equal(sbox[0x66], 0x33, "S-box 0x66");
    assert_equal(sbox[0xE9], 0x1E, "S-box 0xE9");
    assert_equal(sbox[0xF1], 0xA1, "S-box 0xF1");
    assert_equal(sbox[0xFF], 0x16, "S-box 0xFF");
    return 0;
}