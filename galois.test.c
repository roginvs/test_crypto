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
    printf(a_have_highest_bit ? "1." : "0.");                 \
    print_bits(a);                                            \
    printf(" div 0b");                                        \
    print_bits(b);                                            \
    printf(" == q=0b");                                       \
    print_bits(qq);                                           \
    printf(" r=0b");                                          \
    print_bits(rr);                                           \
    printf(" err=%i\n", errr);                                \
    err = poly_divide(a, b, a_have_highest_bit, &q, &r);      \
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

    assert_divide(0b11111110, 0b11011, 0, 0b1011, 0b1011, 0);

    assert_divide(0b11111110, 0b0, 0, 0, 0, 1);

    assert_divide(0b0, 0b0, 0, 0, 0, 0);

    assert_divide(0b0, 0b11011, 0, 0b0, 0b0, 0);

    assert_divide(0b10101010, 0b10101010, 0, 0b1, 0b0, 0);

    assert_divide(0b10101010, 0b1101, 0, 0b11011, 0b101, 0);

    assert_divide(0b10101010, 0b111, 0, 0b111000, 0b10, 0);

    assert_divide(0b10101010, 0b111, 1, 0b1010101, 0b1, 0);

    assert_divide(0b0, 0b111, 1, 0b1101101, 0b11, 0);

    assert_divide(0b0, 0b11111111, 1, 0b11, 0b1, 0);

    assert_divide(0b0, 0b11110101, 1, 0b11, 0b11111, 0);

    assert_divide(0b01011011, 0b111101, 1, 0b1111, 0b0, 0);

    assert_divide(0b01011011, 0b1111, 1, 0b111101, 0b0, 0);

    return 0;
};

#define assert_bezout(a, b, a_have_highest_bit, xx, yy, errr)     \
    printf("Get bezout identity for 0b");                         \
    printf(a_have_highest_bit ? "1." : "0.");                     \
    print_bits(a);                                                \
    printf(" div 0b");                                            \
    print_bits(b);                                                \
    printf(" == x=0b");                                           \
    print_bits(xx);                                               \
    printf(" y=0b");                                              \
    print_bits(yy);                                               \
    printf(" err=%i\n", errr);                                    \
    err = _get_bezout_identity(a, b, a_have_highest_bit, &x, &y); \
    printf("  ");                                                 \
    assert_equal(x, xx, "Check x");                               \
    printf("  ");                                                 \
    assert_equal(y, yy, "Check y");                               \
    printf("  ");                                                 \
    assert_equal(err, errr, "Return code");

int galois_test_3()
{
    uint8_t x;
    uint8_t y;
    uint8_t err;

    assert_bezout(0b1010010, 0b111, 0, 0b1, 0b11101, 0);

    assert_bezout(0b101, 0b10, 0, 0b1, 0b10, 0);

    assert_bezout(0b1010010, 0b101, 0, 0b10, 0b100001, 0);

    // We just testing all 256 possible polynoms because field is small
    // For 2^32 field it will be more reasonable to test some random elements
    //   instead of all elements.
    for (int i = 0xFF; i > 0; i--)
    {
        Poly target = i;
        printf("Checking inverse of ");
        print_bits(target);
        printf(" x ");
        Poly result = get_inverse_element(target);
        print_bits(result);
        printf(" ");
        Poly multiplication = poly_multiple(target, result);
        assert_equal(multiplication, 0b1, "= 1");
    };

    return 0;
};