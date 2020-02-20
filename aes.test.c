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

    return 0;
}