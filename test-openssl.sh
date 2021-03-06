#!/bin/bash

echo -e -n '\x32\x43\xf6\xa8\x88\x5a\x30\x8d\x31\x31\x98\xa2\xe0\x37\x07\x34' > tmp1.tmp

key='2b7e151628aed2a6abf7158809cf4f3c'
    
openssl enc -aes-128-ecb -iv '0000000000000000' -K "$key" -in tmp1.tmp -nopad -out tmp2.tmp

echo -e -n '\x39\x25\x84\x1d\x02\xdc\x09\xfb\xdc\x11\x85\x97\x19\x6a\x0b\x32' > tmp2-expected.tmp

diff tmp2.tmp tmp2-expected.tmp

# 0x09 padding, or 0x01, or 0x0F - how many bytes are padded

# echo -e -n '\x32\x43\xf6\xa8\x88\x5a\x30\x8d\x31\x31\x98\xa2\xe0\x37' > tmpA.tmp
echo -e -n '\x32' > tmpA.tmp
openssl enc -aes-128-ecb -iv '0000000000000000' -K "$key" -in tmpA.tmp -out tmpA-1.tmp
#cat tmpA-1.tmp | xxd

openssl enc -d -aes-128-ecb -iv '0000000000000000' -K "$key" -nopad -out tmpA-2.tmp -in tmpA-1.tmp

cat tmpA-2.tmp  | xxd

# 0x10 padding for whole block

echo -e -n '\x32\x43\xf6\xa8\x88\x5a\x30\x8d\x31\x31\x98\xa2\xe0\x37\x07\x34' > tmpB.tmp
openssl enc -aes-128-ecb -iv '0000000000000000' -K "$key" -in tmpB.tmp -out tmpB-1.tmp
cat tmpB-1.tmp | xxd

openssl enc -d -aes-128-ecb -iv '0000000000000000' -K "$key" -nopad -in tmpB-1.tmp -out tmpB-2.tmp

cat tmpB-2.tmp  | xxd