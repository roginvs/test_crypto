#include "./galois.test.c"
#include "./aes.test.c"

/*
gcc -o /tmp/test test.c && /tmp/test
*/

#define check(x)  \
    if (x)        \
    {             \
        return 1; \
    };

int main()
{
    // check(galois_test());
    // check(aes_test());
    check(galois_test_2());

    printf("All tests done\n");
}
