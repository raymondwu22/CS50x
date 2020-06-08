#include <stdio.h>
#include <cs50.h>
#include <math.h>
#include <stdlib.h>

bool luhns(long credit_card);
string card_type(long credit_card_number);

int main(void)
{
    long credit_card_num = 0;

    do
    {
        credit_card_num = get_long("Number: ");
    }
    while (credit_card_num == 0);
    bool is_credit = luhns(credit_card_num);
    if (is_credit)
    {
        printf("%s\n", card_type(credit_card_num));
    }
    else
    {
        printf("INVALID\n");
    }
}

bool luhns(long credit_card)
{
    long credit_card_copy = credit_card;
    long doubled = 0;
    long not_doubled = 0;

    while (credit_card_copy > 0)
    {
        not_doubled += credit_card_copy % 10;
        doubled = (doubled * 10) + (round((credit_card_copy % 100) / 10) * 2);
        credit_card_copy = credit_card_copy / 100;
    }

    long product_digit_sum = 0;
    while (doubled > 0)
    {
        product_digit_sum += doubled % 10;
        doubled = doubled / 10;
    }
    product_digit_sum += not_doubled;
    long last_digit_summed = product_digit_sum % 10;
    // printf("%ld\n", last_digit_summed);
    return (last_digit_summed == 0);
}

string card_type(long credit_card_number)
{
    int nDigits = floor(log10(labs(credit_card_number))) + 1;
    string card = "";
    long prefix = 0;
    if (nDigits == 13)
    {
        prefix = round(credit_card_number / 1000000000000);
        if (prefix == 4)
        {
            card = "VISA";
        }
        else
        {
            card = "INVALID";
        }
    }
    else if (nDigits == 15)
    {
        // get the first two digits
        prefix = round(credit_card_number / 10000000000000);
        if (prefix == 34 || prefix == 37)
        {
            card = "AMEX";
        }
        else
        {
            card = "INVALID";
        }
    }
    else if (nDigits == 16)
    {
        // get the first digit
        prefix = round(credit_card_number / 1000000000000000);
        if (prefix == 5)
        {
            prefix = round(credit_card_number / 100000000000000);
            if (prefix == 51 || prefix == 52 || prefix == 53 || prefix == 54 || prefix == 55)
            {
                card = "MASTERCARD";
            }
            else
            {
                card = "INVALID";
            }
        }
        else if (prefix == 4)
        {
            card = "VISA";
        }
    }
    else
    {
        card = "INVALID";
    }

    return card;
}