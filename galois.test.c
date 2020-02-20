#include "./galois.c"
#include "./assert_equal.c"
#include <stdio.h>

int galois_test()
{
    _init_inverse_bits_table();

    assert_equal(_inverse_bits[0x00], 0x00, "0x00 inversed bits");
    assert_equal(_inverse_bits[0b00000001], 0b10000000, "0b00000001 inversed bits");
    assert_equal(_inverse_bits[0xFF], 0xFF, "0xFF inversed bits");
    assert_equal(_inverse_bits[0b11111110], 0b01111111, "0b11111110 inversed bits");

    assert_equal(poly, _inverse_bits[poly_inversed], "Inversed bits for poly are same");

    assert_equal(
        poly_multiple_inversed(
            _inverse_bits[0b001100],
            _inverse_bits[0b1100001]),
        _inverse_bits[0b10111010],
        "Some random poly");

    return 0;
};
