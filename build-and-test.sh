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
  for key in "000102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f"\
   "2b7e151628aed2a6abf7158809cf4f3c2b7e151628aed2a6abf7158809cf4f3c"; do
    echo "Using key=$key"
    for iv in "00000000000000000000000000000000" "22332211aabb22aa889922aa99002200"; do
      echo "Using iv=$iv"
      echo "Run ./main..."
      ./main.out aes-256-cbc "$iv" "$key" "$fFull" "$DIR/$f.enc"
      echo "Run openssl.."
      openssl enc -aes-256-cbc -iv "$iv" \
        -K "$key" -in "$fFull" -out "$DIR/$f.enc-openssl"
     
      # Debug
      if false; then 
        echo "We encrypted"
        cat "$DIR/$f.enc" | xxd
        echo ""
  
        echo "Openssk encrypted"
        cat "$DIR/$f.enc-openssl" | xxd
        echo ""
        
        openssl enc -d -aes-256-cbc -iv "$iv" \
          -K "$key" -in "$DIR/$f.enc-openssl" -nopad -out "$DIR/$f.enc-openssl-dec"
        echo "Openssk decrypted with no pad"
        cat "$DIR/$f.enc-openssl-dec" | xxd
        echo ""
      fi

      echo "Checking difference..."
      diff "$DIR/$f.enc" "$DIR/$f.enc-openssl"
      echo "Good, encrypted files are same as openssl does"

      echo "Decrypting ./main..."
      ./main.out aes-256-cbc-d "$iv" "$key" "$DIR/$f.enc" "$DIR/$f.decrypted"

      echo "Checking difference"
      diff "$fFull" "$DIR/$f.decrypted"
      echo "Good, decrypted file is same as initial"

      echo ""
    done
  done
done

# ./main.out


echo "All done"
