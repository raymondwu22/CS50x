#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

int count_letters(string sentence, int sentence_length);
int count_words(string sentence, int sentence_length);
int count_sentences(string sentence, int sentence_length);

int main(void)
{
    string sentence = "";
    int length = 0;
    do
    {
        sentence = get_string("Text: ");
        length = strlen(sentence);
    }

    while (length == 0);
    int W = count_words(sentence, length);
    float L = count_letters(sentence, length);
    float S = count_sentences(sentence, length);
    // printf("%f letter(s)\n", L);
    // printf("%i word(s)\n", W);
    // printf("%f sentence(s)\n", S);
    // where L is the average number of letters per 100 words in the text
    L = L * 100 / W;
    // S is the average number of sentences per 100 words in the text.
    S = S * 100 / W;
    // printf("%f average L\n", L);
    // printf("%f average S\n", S);
    int index = round((0.0588 * L) - (0.296 * S) - 15.8);
    // printf("Grade: %i\n",index);

    // If the resulting index number is 16 or higher (equivalent to or greater than a senior undergraduate reading level), your program should output "Grade 16+".
    // If the index number is less than 1, your program should output "Before Grade 1".
    if (index < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (index < 16)
    {
        printf("Grade %d\n", index);
    }
    else
    {
        printf("Grade 16+\n");
    }
}

int count_letters(string sentence, int sentence_length)
{
    int count = 0;

    for (int i = 0; i < sentence_length; i++)
    {
        if (isalpha(sentence[i]))
        {
            count += 1;
        }
    }

    return count;
}

int count_words(string sentence, int sentence_length)
{
    int num_words = 1;

    for (int i = 0; i < sentence_length; i++)
    {
        if (sentence[i] == ' ')
        {
            num_words += 1;
        }
    }

    return num_words;
}

int count_sentences(string sentence, int sentence_length)
{
    int num_sentence = 0;

    for (int i = 0; i < sentence_length; i++)
    {
        if (sentence[i] == '.' || sentence[i] == '?' || sentence[i] == '!')
        {
            num_sentence += 1;
        }
    }

    return num_sentence;
}