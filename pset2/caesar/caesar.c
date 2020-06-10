#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <stdlib.h>

int main(int argc, string argv[])
{
    string plain_text = "";
    string input = "";
    int key = 0;
    int length = 0;

    if (argc == 2)
    {
        input = argv[1];
        for (int i = 0, input_len = strlen(input); i < input_len; i++)
        {
            if (!isdigit(input[i]))
            {
                printf("Usage: ./caesar key");
                return 1;
            }
        }
        key = atoi(argv[1]);
    }
    else
    {
        printf("Usage: ./caesar key");
        return 1;
    }

    do
    {
        plain_text = get_string("plaintext: ");
        length = strlen(plain_text);
    }
    while (length == 0);

    printf("ciphertext: ");
    for (int i = 0; i < length; i++)
    {
        if (isalpha(plain_text[i]))
        {
            // A = 065, Z = 90
            if (isupper(plain_text[i]))
            {
                printf("%c", 65 + ((plain_text[i] % 65) + key) % 26);
            }
            // a = 097, z = 122
            else if (islower(plain_text[i]))
            {
                printf("%c", 97 + ((plain_text[i] % 97) + key) % 26);
            }
        }
        else
        {
            printf("%c", plain_text[i]);
        }
    }
    printf("\n");
}