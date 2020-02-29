#ifndef __AES_INCLUDED__
#define __AES_INCLUDED__

#include "./galois.c"
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

uint8_t _sbox[0x100];
void _init_sbox()
{
    _sbox[0x00] = _calc_sbox(0x00);
    for (uint8_t i = 0xFF; i != 0; i--)
    {
        _sbox[i] = _calc_sbox(i);
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
        b[i] = _sbox[b[i]];
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

uint8_t _poly_multiple_02[0x100];
uint8_t _poly_multiple_03[0x100];

void _init_poly_multiplication_table()
{
    _poly_multiple_02[0x00] = 0x00;
    _poly_multiple_03[0x00] = 0x00;

    for (uint8_t i = 0xFF; i != 0; i--)
    {
        _poly_multiple_02[i] = poly_multiple(i, 0x2);
        _poly_multiple_03[i] = poly_multiple(i, 0x3);
    };
};

uint8_t _mix_column_0(uint8_t b0, uint8_t b1, uint8_t b2, uint8_t b3)
{
    return _poly_multiple_02[b0] ^
           _poly_multiple_03[b1] ^
           b2 ^
           b3;
};

uint8_t _mix_column_1(uint8_t b0, uint8_t b1, uint8_t b2, uint8_t b3)
{
    return b0 ^
           _poly_multiple_02[b1] ^
           _poly_multiple_03[b2] ^
           b3;
};

uint8_t _mix_column_2(uint8_t b0, uint8_t b1, uint8_t b2, uint8_t b3)
{
    return b0 ^ b1 ^
           _poly_multiple_02[b2] ^
           _poly_multiple_03[b3];
};

uint8_t _mix_column_3(uint8_t b0, uint8_t b1, uint8_t b2, uint8_t b3)
{
    return _poly_multiple_03[b0] ^
           b1 ^ b2 ^
           _poly_multiple_02[b3];
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
        b[i * 4 + 0] = _mix_column_0(a0, a1, a2, a3);
        b[i * 4 + 1] = _mix_column_1(a0, a1, a2, a3);
        b[i * 4 + 2] = _mix_column_2(a0, a1, a2, a3);
        b[i * 4 + 3] = _mix_column_3(a0, a1, a2, a3);
    }
}

#define MAX_RCON 10

uint8_t _rcon[WORD_SIZE * MAX_RCON];
void _init_rcon()
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

void init_tables()
{
    _init_sbox();
    _init_rcon();
    _init_poly_multiplication_table();
};

uint8_t *get_rcon_at(uint8_t i)
{
    return (_rcon + (i - 1) * WORD_SIZE);
};

#define AES_128_KEY_SIZE (4 * WORD_SIZE)
#define AES_192_KEY_SIZE (6 * WORD_SIZE)
#define AES_256_KEY_SIZE (8 * WORD_SIZE)

#define _AES_128_ROUNDS_COUNT 10
#define _AES_192_ROUNDS_COUNT 12
#define _AES_256_ROUNDS_COUNT 14

uint8_t _get_rounds_count(uint8_t key_size)
{
    switch (key_size)
    {
    case AES_128_KEY_SIZE:
        return _AES_128_ROUNDS_COUNT;
    case AES_192_KEY_SIZE:
        return _AES_192_ROUNDS_COUNT;
    case AES_256_KEY_SIZE:
        return _AES_256_ROUNDS_COUNT;

    default:
        return 0;
    }
}

#define AES_128_EXPANDED_KEY_SIZE (WORD_SIZE * WORDS_IN_BLOCK * (_AES_128_ROUNDS_COUNT + 1))
#define AES_192_EXPANDED_KEY_SIZE (WORD_SIZE * WORDS_IN_BLOCK * (_AES_192_ROUNDS_COUNT + 1))
#define AES_256_EXPANDED_KEY_SIZE (WORD_SIZE * WORDS_IN_BLOCK * (_AES_256_ROUNDS_COUNT + 1))

// Maximum is 240 bytes
uint8_t get_size_of_key_expansion_buffer(uint8_t key_size)
{
    return WORD_SIZE * WORDS_IN_BLOCK * (_get_rounds_count(key_size) + 1);
};

void SubWord(Word w)
{
    for (uint8_t i = 0; i < 4; i++)
    {
        w[i] = _sbox[w[i]];
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

void fill_key_expansion(uint8_t key[], uint8_t key_size, uint8_t *buf)
{

    uint8_t key_size_in_words = key_size / WORD_SIZE;

    for (uint8_t i = 0; i < 4 * (_get_rounds_count(key_size) + 1); i++)
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

void aes_encrypt_block(Block b, uint8_t *expanded_key, uint8_t key_size)
{
    AddRoundKey(b, 0, expanded_key);
    uint8_t rounds_count = _get_rounds_count(key_size);
    for (uint8_t round = 1; round < rounds_count; round++)
    {
        SubBytes(b);
        ShiftRows(b);
        MixColumns(b);
        AddRoundKey(b, round, expanded_key);
    };
    SubBytes(b);
    ShiftRows(b);
    AddRoundKey(b, rounds_count, expanded_key);
};

#endif