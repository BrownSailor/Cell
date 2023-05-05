#include <stdio.h>

int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    for (int i = 0; i < 100000000; i++)
    {
        printf("%d\n", i);
    }
    
    return 0;
}
