#include <stdio.h>
#include <cs50.h>

int main(void)
{
    int height = 0;
    do {
        height = get_int("Height: ");
    }
    while (height <= 0 || height > 8);

    int block = 1;

    // iterate over each row
    for (int i = 0; i < height; i++) {
        // iterate over each column
        // print spaces
        for (int space = 0; space < height - block; space++) {
            printf(" ");
        }
        // print blocks
        for (int j = 0; j < block; j++) {
            printf("#");
        }
        block++;
        printf("\n");
    }
}