# Implementing AES encryption in CBC mode

## Building and testing

![ci_test](https://github.com/roginvs/test_crypto/workflows/ci_test/badge.svg)

```bash
./build-and-test.sh
```

## Using in command-line mode

Only AES-256-CBC with PKCS5 padding is supported

```bash
./main.out <mode> <iv> <key> <input_file_name> <output_file_name>

# mode = aes-256-cbc | aes-256-cbc-d
# iv = initialization vector
```

### Encryption example

```bash
./main.out \
  aes-256-cbc \
  22332211aabb22aa889922aa99002200 \
  2b7e151628aed2a6abf7158809cf4f3c2b7e151628aed2a6abf7158809cf4f3c \
  in.txt \
  in.txt.encrypted

# The same, but using openssl
openssl enc -aes-256-cbc \
  -iv 22332211aabb22aa889922aa99002200 \
  -K 2b7e151628aed2a6abf7158809cf4f3c2b7e151628aed2a6abf7158809cf4f3c \
  -in in.txt \
  -out in.txt.encrypted

```

### Decryption example

```bash
./main.out \
  aes-256-cbc-d \
  22332211aabb22aa889922aa99002200 \
  2b7e151628aed2a6abf7158809cf4f3c2b7e151628aed2a6abf7158809cf4f3c \
  in.txt.encrypted \
  in.txt.decrypted

# The same, but using openssl
openssl enc -aes-256-cbc \
  -d \
  -iv 22332211aabb22aa889922aa99002200 \
  -K 2b7e151628aed2a6abf7158809cf4f3c2b7e151628aed2a6abf7158809cf4f3c \
  -in in.txt.encrypted \
  -out in.txt.decrypted

```

## Using as library

```C

#include "./aes.c"

// First of all we need to initialize tables
init_tables();

// Block to encrypt
uint8_t block[BLOCK_SIZE] = {
    0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
    0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff};

// Key. In this example it is 256 bits length
uint8_t cipher_key_256[AES_256_KEY_SIZE] = {
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
    0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
    0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
    0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f};

// We have to perform expansion for the key according to standard
uint8_t expanded_key_256[AES_256_EXPANDED_KEY_SIZE];
fill_key_expansion(cipher_key_256, AES_256_KEY_SIZE, expanded_key_256);

// And thenn to encrypt the block
aes_encrypt_block(block, expanded_key_256, AES_256_KEY_SIZE);

// Now block contains encrypted data
```

## What can be improved

- Try to use x86/x64 operations for perfomance
- Chech that file is opened using buffered write - no need to perform fsync after each block
