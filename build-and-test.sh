#!/bin/bash
set -e

echo "==== Running unit tests ===="
gcc -Wall -o test.out test.c
./test.out

echo ""
echo ""

## TODO: Build main executable and test it

echo "All done"
