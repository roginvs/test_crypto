#include "./poly8.c"

#define assertEqual(a, b, msg)                                 \
                                                               \
    if ((a) != (b))                                            \
    {                                                          \
        printf("\n");                                          \
        printf("Asserted=0x%08x Obtained=0x%08x\n", (b), (a)); \
        printf("Error with: ");                                \
        printf(msg);                                           \
        printf("\n");                                          \
        return 100;                                            \
    }
;

int poly8_test()
{
    assertEqual(poly, _reverse_bits[poly_inversed], "Inversed bits");
    return 0;
};
