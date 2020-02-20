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

void print_bits(uint8_t a)
{
    for (uint8_t i = 0; i < 8; i++)
    {
        printf((a >> (7 - i)) & 1 ? "1" : "0");
    };
};

#define assert_divide(a, b, a_have_highest_bit, qq, rr, errr) \
    printf("Divide 0b");                                      \
    print_bits(a);                                            \
    printf(" div 0b");                                        \
    print_bits(b);                                            \
    printf(" == q=0b");                                       \
    print_bits(qq);                                           \
    printf(" r=0b");                                          \
    print_bits(rr);                                           \
    printf(" err=%i\n", errr);                                \
    err = poly_divide(a, b, 0, &q, &r);                       \
    printf("  ");                                             \
    assert_equal(q, qq, "Check q");                           \
    printf("  ");                                             \
    assert_equal(r, rr, "Check r");                           \
    printf("  ");                                             \
    assert_equal(err, errr, "Return code");

int galois_test_2()
{
    uint8_t q;
    uint8_t r;
    uint8_t err;

    assert_divide(0b01111110, 0b11011, 0, 0b101, 0b1001, 0);

    err = poly_divide(0b11111110, 0b11011, 0, &q, &r);
    assert_equal(q, 0b1011, "q");
    assert_equal(r, 0b1011, "q");
    assert_equal(err, 0, "no error ");

    err = poly_divide(0b11111110, 0b0, 0, &q, &r);
    assert_equal(err, 1, "error");

    err = poly_divide(0b0, 0b0, 0, &q, &r);
    assert_equal(err, 0, "no error");

    err = poly_divide(0b0, 0b11011, 0, &q, &r);
    assert_equal(q, 0b0, "q");
    assert_equal(r, 0b0, "q");
    assert_equal(err, 0, "no error");

    err = poly_divide(0b10101010, 0b10101010, 0, &q, &r);
    assert_equal(q, 0b1, "q");
    assert_equal(r, 0b0, "q");
    assert_equal(err, 0, "no error");

    err = poly_divide(0b10101010, 0b1101, 0, &q, &r);
    assert_equal(q, 0b11011, "q");
    assert_equal(r, 0b101, "q");
    assert_equal(err, 0, "no error");

    err = poly_divide(0b10101010, 0b111, 0, &q, &r);
    assert_equal(q, 0b111000, "q");
    assert_equal(r, 0b10, "q");
    assert_equal(err, 0, "no error");

    err = poly_divide(0b10101010, 0b111, 1, &q, &r);
    assert_equal(q, 0b1010101, "q");
    assert_equal(r, 0b1, "q");
    assert_equal(err, 0, "no error");

    err = poly_divide(0b0, 0b111, 1, &q, &r);
    assert_equal(q, 0b1101101, "q");
    assert_equal(r, 0b11, "q");
    assert_equal(err, 0, "no error");

    err = poly_divide(0b0, 0b11111111, 1, &q, &r);
    assert_equal(q, 0b11, "q");
    assert_equal(r, 0b1, "q");
    assert_equal(err, 0, "no error");

    err = poly_divide(0b0, 0b11110101, 1, &q, &r);
    assert_equal(q, 0b11, "q");
    assert_equal(r, 0b11111, "q");
    assert_equal(err, 0, "no error");

    err = poly_divide(0b01011011, 0b111101, 1, &q, &r);
    assert_equal(q, 0b1111, "q");
    assert_equal(r, 0b0, "q");
    assert_equal(err, 0, "no error");

    err = poly_divide(0b01011011, 0b1111, 1, &q, &r);
    assert_equal(q, 0b111101, "q");
    assert_equal(r, 0b0, "q");
    assert_equal(err, 0, "no error");
    return 0;
};