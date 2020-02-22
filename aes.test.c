#include <stdio.h>

#include "./aes.c"

#define assert_mix_columns(d, msg)                                              \
    printf(msg);                                                                \
    printf(": %02x %02x %02x %02x -> %02x %02x %02x %02x\n",                    \
           d[0], d[1], d[2], d[3], d[4], d[5], d[6], d[7]);                     \
    assert_equal(mix_column_0(d[0], d[1], d[2], d[3]), d[4], "  mix_column_0"); \
    assert_equal(mix_column_1(d[0], d[1], d[2], d[3]), d[5], "  mix_column_1"); \
    assert_equal(mix_column_2(d[0], d[1], d[2], d[3]), d[6], "  mix_column_2"); \
    assert_equal(mix_column_3(d[0], d[1], d[2], d[3]), d[7], "  mix_column_3"); \
    ;

#define assert_block(a, b, msg)                                                 \
    printf("Block test ");                                                      \
    printf(msg);                                                                \
    for (uint8_t i = 0; i < BLOCK_SIZE; i++)                                    \
    {                                                                           \
        if (a[i] != b[i])                                                       \
        {                                                                       \
            printf(" FAIL at pos = %i, a[i]=%02x, b[i]=%02x\n", i, a[i], b[i]); \
            return 1;                                                           \
        };                                                                      \
    };                                                                          \
    printf(" ok\n");

#define assert_word(a, b, msg)                                                      \
    printf("Word test ");                                                           \
    printf(msg);                                                                    \
    for (uint8_t i = 0; i < 4; i++)                                                 \
    {                                                                               \
        if ((a)[i] != (b)[i])                                                       \
        {                                                                           \
            printf(" FAIL at pos = %i, a[i]=%02x, b[i]=%02x\n", i, (a)[i], (b)[i]); \
            return 1;                                                               \
        };                                                                          \
    };                                                                              \
    printf(" ok\n");

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

    uint8_t block[] = {
        0x19,
        0x3d,
        0xe3,
        0xbe,
        0xa0,
        0xf4,
        0xe2,
        0x2b,
        0x9a,
        0xc6,
        0x8d,
        0x2a,
        0xe9,
        0xf8,
        0x48,
        0x08};
    uint8_t after_sub_bytes[] = {
        0xd4, 0x27, 0x11, 0xae, 0xe0, 0xbf, 0x98, 0xf1, 0xb8, 0xb4, 0x5d, 0xe5, 0x1e, 0x41, 0x52, 0x30};
    SubBytes(block);
    assert_block(block, after_sub_bytes, "SubBytes");

    uint8_t after_shift_rows[] = {
        0xd4,
        0xbf,
        0x5d,
        0x30,
        0xe0,
        0xb4,
        0x52,
        0xae,
        0xb8,
        0x41,
        0x11,
        0xf1,
        0x1e,
        0x27,
        0x98,
        0xe5,
    };

    ShiftRows(block);
    assert_block(block, after_shift_rows, "ShiftRows");

    uint8_t after_mix_columns[] = {
        0x04, 0x66, 0x81, 0xe5,
        0xe0, 0xcb, 0x19, 0x9a,
        0x48, 0xf8, 0xd3, 0x7a,
        0x28, 0x06, 0x26, 0x4c};
    MixColumns(block);
    assert_block(block, after_mix_columns, "MixColumns");

    init_rcon();
    Poly rcon1[] = {0x1, 0x00, 0x00, 0x00};
    assert_word(get_rcon_at(1), rcon1, "rcon1");

    Poly rcon2[] = {0x02, 0x00, 0x00, 0x00};
    assert_word(get_rcon_at(2), rcon2, "rcon2");

    Poly rcon3[] = {0x04, 0x00, 0x00, 0x00};
    assert_word(get_rcon_at(3), rcon3, "rcon3");

    Poly rcon10[] = {0x36, 0x00, 0x00, 0x00};
    assert_word(get_rcon_at(10), rcon10, "rcon10");

    Poly word[] = {0x1, 0x2, 0x3, 0x4};
    Poly word_after_rotate[] = {0x2, 0x3, 0x4, 0x1};
    RotWord(word);
    assert_word(word, word_after_rotate, "Work rotated");

    Poly word_after_sub[] = {0x77, 0x7b, 0xf2, 0x7c};
    SubWord(word);
    assert_word(word, word_after_sub, "Word sub");

    // Key expansion for AES128
    assert_equal(get_size_of_key_expansion_buffer(4), 44 * 4, "Size of key expansion");
    uint8_t cipher_key[] = {
        0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c};
    uint8_t expanded_key[44 * 4];
    fill_key_expansion(cipher_key, 4, expanded_key);
    assert_word(expanded_key, cipher_key, "Start 1");
    assert_word(expanded_key + 4, cipher_key + 4, "Start 2");
    assert_word(expanded_key + 8, cipher_key + 8, "Start 3");
    assert_word(expanded_key + 12, cipher_key + 12, "Start 4");

    uint8_t i4[] = {0xa0, 0xfa, 0xfe, 0x17};
    assert_word(expanded_key + 4 * 4, i4, "i4");

    uint8_t i5[] = {0x88, 0x54, 0x2c, 0xb1};
    assert_word(expanded_key + 4 * 5, i5, "i5");

    uint8_t i6[] = {0x23, 0xa3, 0x39, 0x39};
    assert_word(expanded_key + 4 * 6, i6, "i6");

    uint8_t i7[] = {0x2a, 0x6c, 0x76, 0x05};
    assert_word(expanded_key + 4 * 7, i7, "i7");

    uint8_t i8[] = {0xf2, 0xc2, 0x95, 0xf2};
    assert_word(expanded_key + 4 * 8, i8, "i8");

    uint8_t i42[] = {0xe1, 0x3f, 0x0c, 0xc8};
    assert_word(expanded_key + 4 * 42, i42, "i42");

    uint8_t i43[] = {0xb6, 0x63, 0x0c, 0xa6};
    assert_word(expanded_key + 4 * 43, i43, "i43");

    return 0;
}