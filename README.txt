# Реализация шифрования AES в режиме CBC

## Сборка и тест

```bash
./build-and-test.sh
```

## Использование в режиме командой строки

Поддерживается только AES-256-CBC с автопаддингом в режиме openssl

```bash
./main.out <iv> <key> <input_file_name> <output_file_name>

# Пример

./main.out \
  22332211aabb22aa889922aa99002200 \
  2b7e151628aed2a6abf7158809cf4f3c2b7e151628aed2a6abf7158809cf4f3c \
  in.txt \
  in.txt.encrypted

```

## Использование как библиотеки

```C

#include "./aes.c"

// Инициализация таблиц
init_rcon();
init_sbox();

// Блок для шифрования
uint8_t block[BLOCK_SIZE] = {
    0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
    0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff};

// Ключ. В данном примере 256 бит
uint8_t cipher_key_256[AES_256_KEY_SIZE] = {
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
    0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
    0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
    0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f};

// Для ключа нужно сделать расширение
uint8_t expanded_key_256[AES_256_EXPANDED_KEY_SIZE];
fill_key_expansion(cipher_key_256, AES_256_KEY_SIZE, expanded_key_256);

// И затем зашифровать блок
aes_encrypt_block(block, expanded_key_256, AES_256_KEY_SIZE);

// Теперь block содержит зашифрованные данные
```