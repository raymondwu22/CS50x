#include <stdio.h>
#include <cs50.h>
#include <math.h>

int main(void)
{
    float dollars = 0;

    do
    {
        dollars = get_float("Change owed: ");
    }
    while (dollars <= 0);

    int cents = round(dollars * 100);
    int coins = 0;
    int QUARTERS = 25;
    int DIMES = 10;
    int NICKELS = 5;
    // int PENNIES = 1;

    if (cents > 0.25)
    {
        coins = coins + round(cents / QUARTERS);
        cents = cents % QUARTERS;
    }

    if (cents > 0.10)
    {
        coins = coins + round(cents / DIMES);
        cents = cents % DIMES;
    }

    if (cents > 0.05)
    {
        coins = coins + round(cents / NICKELS);
        cents = cents % NICKELS;
    }

    coins = coins + round(cents);
    printf("%i\n", coins);
}