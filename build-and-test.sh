#!/bin/bash
set -e

echo "==== Running unit tests ===="
gcc -Wall -o test.out test.c
./test.out

echo "Unit tests done"
echo ""


gcc -Wall -o main.out main.c


DIR=`mktemp -d`

for fFull in `ls testdata/*`; do
  f=`basename "${fFull}"`
  echo "==== Testing $f file ===="
  for key in "2b7e151628aed2a6abf7158809cf4f3c2b7e151628aed2a6abf7158809cf4f3c"; do
    echo "Using key=$key"
    for iv in "22332211aabb22aa889922aa99002200"; do
      echo "Using iv=$iv"
      ./main.out "$iv" "$key" "$fFull" "$DIR/$f.enc"
      openssl enc -d -aes-256-ecb -iv "$iv" \
        -K "$key" -nopad -in "$fFull" -out "$DIR/$f.enc-openssl"
      diff "$DIR/$f.enc" "$DIR/$f.enc-openssl"
    done
  done
done

# ./main.out


echo "All done"
