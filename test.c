#include "./galois.test.c"
#include "./aes.test.c"

#include "./test_utils.c"

/*
gcc -o /tmp/test test.c && /tmp/test
*/

int main()
{
    check(galois_test());
    check(galois_test_2());
    check(galois_test_3());

    check(aes_test());

    printf("All tests done\n");
}
