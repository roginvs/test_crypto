#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "./aes.cbc.c"

void writer(uint8_t *buf, uint32_t len, void *user_data)
{
    fwrite(buf, len, 1, (FILE *)user_data);
}

int main(int argc, char *argv[])
{
    char *command = argv[1];

    uint8_t is_encrypting;
    if (strcmp(command, "aes-256-cbc") == 0)
    {
        is_encrypting = 1;
    }
    else if (strcmp(command, "aes-256-cbc-d") == 0)
    {
        is_encrypting = 0;
    }
    else
    {
        printf("Wrong usage\n");
        return 120;
    };

    char *iv_char = argv[2];
    char *key_char = argv[3];

    char *input_file_name = argv[4];
    char *output_file_name = argv[5];

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

    aes_256_cbc_encrypt(input_buf, input_buf_size, iv, key,
                        &writer, p_output_file);

    printf("Closing output file %s\n", output_file_name);
    fclose(p_output_file);

    if (input_buf_size != 0)
    {
        if (munmap(input_buf, input_buf_size) == -1)
        {
            perror("Error un-mmapping input file");
        }
    }
    printf("Closing input file %s\n", input_file_name);
    close(fd);
}