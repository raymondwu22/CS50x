#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

bool has_repeats(string text);

int main(int argc, string argv[])
{
    string substitution = "";
    string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    string plain_text = "";
    bool repeated = false;
    int length = 0;
    int index = 0;
    char *e;

    if (argc == 2)
    {
        substitution = argv[1];
        int substitution_length = strlen(substitution);

        if (substitution_length != 26)
        {
            printf("Key must contain 26 characters.\n");
            return 1;
        }
        else
        {
            repeated = has_repeats(substitution);
            if (repeated == true)
            {
                printf("repeated");
                return 1;
            }
            else
            {
                for (int i = 0; i < substitution_length; i++)
                {
                    if (!isalpha(substitution[i]))
                    {
                        printf("Usage: ./substitution key\n");
                        return 1;
                    }
                }
            }
        }
    }
    else
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }

    do
    {
        plain_text = get_string("plaintext: ");
        length = strlen(plain_text);
    }
    while (length == 0);

    printf("ciphertext: ");
    for (int j = 0; j < length; j++)
    {
        if (isalpha(plain_text[j]))
        {
            e = strchr(alphabet, toupper(plain_text[j]));
            index = (int)(e - alphabet);
            if (isupper(plain_text[j]))
            {
                printf("%c", (char) toupper(substitution[index]));
            }
            else
            {
                printf("%c", (char) tolower(substitution[index]));
            }
        }
        else
        {
            printf("%c", (char) plain_text[j]);
        }
    }
    printf("\n");
    return 0;
}

bool has_repeats(string text)
{
    int length = strlen(text);
    if (length == 0)
    {
        return false;
    }
    char current = text[0];
    for (int i = 0; i < length; i++)
    {
        current = text[i];
        for (int j = 0; j < length; j++)
        {
            if (i != j && current == text[j])
            {
                return true;
            }
        }
    }
    return false;
}