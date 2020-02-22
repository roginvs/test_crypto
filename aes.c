#ifndef __AES_INCLUDED__
#define __AES_INCLUDED__

#include "./galois.c"

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

uint8_t byte_cyclic_left_shift(uint8_t x, uint8_t i)
{
    return (x << i) | (x >> (8 - i));
};

uint8_t calc_sbox(uint8_t x)
{
    uint8_t b = x != 0 ? get_inverse_element(x) : 0;
    uint8_t s = b ^ byte_cyclic_left_shift(b, 1) ^
                byte_cyclic_left_shift(b, 2) ^
                byte_cyclic_left_shift(b, 3) ^
                byte_cyclic_left_shift(b, 4) ^ 0x63;
    return s;
};

#endif