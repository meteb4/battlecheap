#include <stdio.h>

int len = 10;
char alphabet[10] = "ABCDEFGHIJ";

int ALPHANUM(char c)
{
    for (int i = 0; i < len; i += 1)
    {
        if (c == alphabet[i])
        {
            return i + 1;
        }
    }
    return -1;
}

int main()
{
    char x;
    scanf("%c", &x);
    printf("%d", ALPHANUM(x));
    return 0;
}
