#ifndef assert_equal
#define assert_equal(a, b, msg)                                      \
    printf(msg);                                                     \
    printf("... ");                                                  \
    if ((a) != (b))                                                  \
    {                                                                \
        printf("FAIL: Asserted=0x%02x Obtained=0x%02x\n", (b), (a)); \
        return 100;                                                  \
    }                                                                \
    else                                                             \
    {                                                                \
        printf("ok\n");                                              \
    };

#endif