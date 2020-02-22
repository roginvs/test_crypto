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

uint8_t _calc_sbox(uint8_t x)
{
    uint8_t b = x != 0 ? get_inverse_element(x) : 0;
    uint8_t s = b ^ byte_cyclic_left_shift(b, 1) ^
                byte_cyclic_left_shift(b, 2) ^
                byte_cyclic_left_shift(b, 3) ^
                byte_cyclic_left_shift(b, 4) ^ 0x63;
    return s;
};

uint8_t sbox[0x100];
void init_sbox()
{
    sbox[0x00] = _calc_sbox(0x00);
    for (uint8_t i = 0xFF; i != 0; i--)
    {
        sbox[i] = _calc_sbox(i);
    };
}

typedef uint8_t *Block;
uint8_t BLOCK_SIZE = 16;

void SubBytes(Block b)
{
    for (uint8_t i = 0; i < BLOCK_SIZE; i++)
    {
        b[i] = sbox[b[i]];
    }
};

void ShiftRows(Block b)
{
    uint8_t c;

    c = b[13];
    b[13] = b[1];
    b[1] = b[5];
    b[5] = b[9];
    b[9] = c;

    c = b[14];
    b[14] = b[6];
    b[6] = c;

    c = b[10];
    b[10] = b[2];
    b[2] = c;

    c = b[15];
    b[15] = b[11];
    b[11] = b[7];
    b[7] = b[3];
    b[3] = c;
};

void MixColumns(Block b)
{
    uint8_t a0;
    uint8_t a1;
    uint8_t a2;
    uint8_t a3;
    for (uint8_t i = 0; i < 4; i++)
    {
        a0 = b[i * 4 + 0];
        a1 = b[i * 4 + 1];
        a2 = b[i * 4 + 2];
        a3 = b[i * 4 + 3];
        b[i * 4 + 0] = mix_column_0(a0, a1, a2, a3);
        b[i * 4 + 1] = mix_column_1(a0, a1, a2, a3);
        b[i * 4 + 2] = mix_column_2(a0, a1, a2, a3);
        b[i * 4 + 3] = mix_column_3(a0, a1, a2, a3);
    }
}

typedef uint8_t *Word;

void AddRoundKey(Block b){
    // TODO
};

#endif