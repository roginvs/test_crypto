#include "./poly8.test.c"

/*
gcc -o /tmp/test test.c && /tmp/test
*/

int main()
{
    int poly8_test_result = poly8_test();
    if (poly8_test_result)
    {
        return poly8_test_result;
    };

    printf("All tests done\n");
}
