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
typedef uint8_t *Word;
#define WORD_SIZE 4
#define WORDS_IN_BLOCK 4
#define BLOCK_SIZE (WORD_SIZE * WORDS_IN_BLOCK)

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

const uint8_t MAX_RCON = 10;

uint8_t _rcon[WORD_SIZE * MAX_RCON];
void init_rcon()
{
    Poly xi = 0b1;
    for (uint8_t i = 0; i < 10; i++)
    {
        _rcon[i * WORD_SIZE + 0] = xi;
        _rcon[i * WORD_SIZE + 1] = 0;
        _rcon[i * WORD_SIZE + 2] = 0;
        _rcon[i * WORD_SIZE + 3] = 0;

        xi = poly_multiple(xi, 0b10);
    }
};

uint8_t *get_rcon_at(uint8_t i)
{
    return (_rcon + (i - 1) * WORD_SIZE);
};

uint8_t get_rounds_number(uint8_t key_size_in_words)
{
    switch (key_size_in_words)
    {
    case 4:
        return 10;
    case 6:
        return 12;
    case 8:
        return 14;

    default:
        return 0;
    }
}

// Maximum is 240 bytes
uint8_t get_size_of_key_expansion_buffer(uint8_t key_size_in_words)
{
    return WORD_SIZE * WORDS_IN_BLOCK * (get_rounds_number(key_size_in_words) + 1);
};

void SubWord(Word w)
{
    for (uint8_t i = 0; i < 4; i++)
    {
        w[i] = sbox[w[i]];
    };
};

void RotWord(Word w)
{
    uint8_t c;
    c = w[0];
    w[0] = w[1];
    w[1] = w[2];
    w[2] = w[3];
    w[3] = c;
};

void AddWords(Word a, Word b)
{
    a[0] = a[0] ^ b[0];
    a[1] = a[1] ^ b[1];
    a[2] = a[2] ^ b[2];
    a[3] = a[3] ^ b[3];
}

void fill_key_expansion(uint8_t key[], uint8_t key_size_in_words, uint8_t *buf)
{

    for (uint8_t i = 0; i < 4 * (get_rounds_number(key_size_in_words) + 1); i++)
    {
        if (i < key_size_in_words)
        {
            for (uint8_t ii = 0; ii < WORD_SIZE; ii++)
            {
                buf[i * WORD_SIZE + ii] = key[i * WORD_SIZE + ii];
            };
        }
        else
        {
            for (uint8_t ii = 0; ii < WORD_SIZE; ii++)
            {
                buf[i * WORD_SIZE + ii] = buf[(i - 1) * WORD_SIZE + ii];
            }

            if (i % key_size_in_words == 0)
            {
                RotWord(buf + i * WORD_SIZE);
                SubWord(buf + i * WORD_SIZE);
                AddWords(buf + i * WORD_SIZE, get_rcon_at(i / key_size_in_words));
            };
            if (key_size_in_words == 8 && i % key_size_in_words == 4)
            {
                SubWord(buf + i * WORD_SIZE);
            };
            AddWords(buf + i * WORD_SIZE, buf + (i - key_size_in_words) * WORD_SIZE);
        }
    }
};

void AddRoundKey(Block b, uint8_t round, uint8_t *expanded_key)
{
    for (uint8_t i = 0; i < BLOCK_SIZE; i++)
    {
        b[i] = b[i] ^ expanded_key[round * BLOCK_SIZE + i];
    };
};

#endif