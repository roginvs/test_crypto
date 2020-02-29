#include <stdint.h>
#include "./aes.c"

/** Тип колбека на запись */
typedef void (*write_handler)(
    /** Указатель на данные которые нужно записать */
    uint8_t *buf,
    /** Длинна данных */
    uint32_t len,
    /** Произвольный указатель */
    void *user_data);

void aes_256_cbc_encrypt(uint8_t *in, int32_t size, Block iv, uint8_t *key,
                         write_handler write,
                         void *write_user_data)
{
    init_tables();

    uint8_t key_expanded[AES_256_EXPANDED_KEY_SIZE];
    fill_key_expansion(key, AES_256_KEY_SIZE, key_expanded);

    uint8_t b[BLOCK_SIZE] = {0x00};

    memcpy(b, iv, BLOCK_SIZE);

    int32_t blocks_count = size / BLOCK_SIZE;
    uint8_t tail_bytes_count = size % BLOCK_SIZE;
    if (tail_bytes_count != 0)
    {
        blocks_count++;
    };

    for (int32_t block_num = 0; block_num < blocks_count; block_num++)
    {
        uint8_t is_block_last = block_num == blocks_count - 1;

        uint8_t bytes_to_copy = !is_block_last ? BLOCK_SIZE : tail_bytes_count == 0 ? BLOCK_SIZE : tail_bytes_count;

        // printf("Block num = %i\n", block_num);
        // printf("is_block_last = %i\n", is_block_last);
        // printf("tail_bytes_count= %i\n", tail_bytes_count);
        // printf("bytes_to_copy= %i\n", bytes_to_copy);
        for (uint8_t i = 0; i < bytes_to_copy; i++)
        {
            b[i] = b[i] ^ in[block_num * BLOCK_SIZE + i];
        };

        // printf("=== %i %i %i\n", blocks_count, block_num, is_block_last);
        if (is_block_last)
        {
            uint8_t padding_count = BLOCK_SIZE - bytes_to_copy;
            // printf("Padding count is %i\n", padding_count);
            for (uint8_t i = BLOCK_SIZE - padding_count; i < BLOCK_SIZE; i++)
            {
                b[i] = b[i] ^ padding_count;
            };
        };

        aes_encrypt_block(b, key_expanded, AES_256_KEY_SIZE);
        write(b, BLOCK_SIZE, write_user_data);
    };

    if (tail_bytes_count == 0)
    {
        for (uint8_t i = 0; i < BLOCK_SIZE; i++)
        {
            b[i] = b[i] ^ BLOCK_SIZE;
        };
        aes_encrypt_block(b, key_expanded, AES_256_KEY_SIZE);
        write(b, BLOCK_SIZE, write_user_data);
    };
}
