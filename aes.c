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

#endif