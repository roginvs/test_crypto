#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#include "./aes.c"

void add_iv(Block b, Block iv)
{
    for (uint8_t i = 0; i < BLOCK_SIZE; i++)
    {
        b[i] = b[i] ^ iv[i];
    };
};

void aes_256_encrypt(uint8_t *in, int32_t size, Block iv, uint8_t *key, FILE *outfile)
{
    init_rcon();
    init_sbox();

    uint8_t key_expanded[AES_256_EXPANDED_KEY_SIZE];
    fill_key_expansion(key, AES_256_KEY_SIZE, key_expanded);

    uint8_t b[BLOCK_SIZE] = {0x00};
    memcpy(b, in, BLOCK_SIZE);
    add_iv(b, iv);
    aes_encrypt_block(b, key_expanded, AES_256_KEY_SIZE);
    fwrite(b, BLOCK_SIZE, 1, outfile);
}

int main(int argc, char *argv[])
{
    char *iv_char = argv[1];
    char *key_char = argv[2];

    char *input_file_name = argv[3];
    char *output_file_name = argv[4];

    uint8_t iv[BLOCK_SIZE];
    for (uint8_t i = 0; i < BLOCK_SIZE; i++)
    {
        sscanf(&iv_char[i * 2], "%2hhx", &iv[i]);
    };

    uint8_t key[AES_256_KEY_SIZE];
    for (uint8_t i = 0; i < AES_256_KEY_SIZE; i++)
    {
        sscanf(&key_char[i * 2], "%2hhx", &key[i]);
    };

    printf("Opening input file %s\n", input_file_name);
    int fd = open(input_file_name, O_RDONLY);
    if (fd == -1)
    {
        perror("Error opening input file");
        exit(1);
    }
    struct stat sb;
    fstat(fd, &sb);
    int32_t input_buf_size = sb.st_size;
    printf("Size: %i\n", input_buf_size);

    unsigned char *input_buf = NULL;
    if (input_buf_size != 0)
    {
        input_buf = mmap(NULL, input_buf_size, PROT_READ, MAP_PRIVATE, fd, 0);
        if (input_buf == MAP_FAILED)
        {
            perror("Error with mmap");
            exit(1);
        }
    };

    printf("Opening output file %s\n", output_file_name);
    FILE *p_output_file = fopen(output_file_name, "wb");

    aes_256_encrypt(input_buf, input_buf_size, iv, key, p_output_file);

    fclose(p_output_file);

    if (input_buf_size != 0)
    {
        if (munmap(input_buf, input_buf_size) == -1)
        {
            perror("Error un-mmapping input file");
        }
    }
    close(fd);
}