#include "helpers.h"
#include <math.h>
#include <stdlib.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            unsigned int grey = round((image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / 3.0);
            image[i][j].rgbtBlue = grey;
            image[i][j].rgbtGreen = grey;
            image[i][j].rgbtRed = grey;
        }
    }
    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // For each pixel, the sepia color values should be calculated based on the original color values per the below.
            unsigned int sepiaRed = round(.393 * image[i][j].rgbtRed + .769 * image[i][j].rgbtGreen + .189 * image[i][j].rgbtBlue);
            unsigned int sepiaGreen = round(.349 * image[i][j].rgbtRed + .686 * image[i][j].rgbtGreen + .168 * image[i][j].rgbtBlue);
            unsigned int sepiaBlue = round(.272 * image[i][j].rgbtRed + .534 * image[i][j].rgbtGreen + .131 * image[i][j].rgbtBlue);
            if (sepiaRed > 255)
            {
                sepiaRed = 255;
            }
            if (sepiaGreen > 255)
            {
                sepiaGreen = 255;
            }
            if (sepiaBlue > 255)
            {
                sepiaBlue = 255;
            }

            image[i][j].rgbtBlue = sepiaBlue;
            image[i][j].rgbtGreen = sepiaGreen;
            image[i][j].rgbtRed = sepiaRed;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < round(width/2); j++)
        {
            int *tempBlue = malloc(sizeof (int));
            int *tempGreen = malloc(sizeof (int));
            int *tempRed = malloc(sizeof (int));
            *tempBlue = image[i][j].rgbtBlue;
            *tempGreen = image[i][j].rgbtGreen;
            *tempRed = image[i][j].rgbtRed;
            image[i][j].rgbtBlue = image[i][width - 1 - j].rgbtBlue;
            image[i][j].rgbtGreen = image[i][width - 1 - j].rgbtGreen;
            image[i][j].rgbtRed = image[i][width - 1 - j].rgbtRed;
            image[i][width - 1 - j].rgbtBlue = *tempBlue;
            image[i][width - 1 - j].rgbtGreen = *tempGreen;
            image[i][width - 1 - j].rgbtRed = *tempRed;
        }
    }
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    float count = 0.0;
    int total_red = 0;
    int total_green = 0;
    int total_blue = 0;

    //create copy since we are altering the image in place
    RGBTRIPLE temp[height][width];

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            temp[i][j].rgbtRed = image[i][j].rgbtRed;
            temp[i][j].rgbtGreen = image[i][j].rgbtGreen;
            temp[i][j].rgbtBlue = image[i][j].rgbtBlue;
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // reset variables to 0 for start of loop
            count = 0.0;
            total_red = 0;
            total_green = 0;
            total_blue = 0;

            total_red += temp[i][j].rgbtRed;
            total_green += temp[i][j].rgbtGreen;
            total_blue += temp[i][j].rgbtBlue;
            count += 1;

            // column to the left
            if (j - 1 >= 0)
            {
                total_red += temp[i][j - 1].rgbtRed;
                total_green += temp[i][j - 1].rgbtGreen;
                total_blue += temp[i][j - 1].rgbtBlue;
                count += 1;
            }
            // column to the right
            if (j + 1 < width)
            {
                total_red += temp[i][j + 1].rgbtRed;
                total_green += temp[i][j + 1].rgbtGreen;
                total_blue += temp[i][j + 1].rgbtBlue;
                count += 1;
            }

            // row above
            if (i - 1 >= 0)
            {
                total_red += temp[i - 1][j].rgbtRed;
                total_green += temp[i - 1][j].rgbtGreen;
                total_blue += temp[i - 1][j].rgbtBlue;
                count += 1;
                // column to the left
                if (j - 1 >= 0)
                {
                    total_red += temp[i - 1][j - 1].rgbtRed;
                    total_green += temp[i - 1][j - 1].rgbtGreen;
                    total_blue += temp[i - 1][j - 1].rgbtBlue;
                    count += 1;
                }
                // column to the right
                if (j + 1 < width)
                {
                    total_red += temp[i - 1][j + 1].rgbtRed;
                    total_green += temp[i - 1][j + 1].rgbtGreen;
                    total_blue += temp[i - 1][j + 1].rgbtBlue;
                    count += 1;
                }
            }
            // row below
            if (i + 1 < height)
            {
                total_red += temp[i + 1][j].rgbtRed;
                total_green += temp[i + 1][j].rgbtGreen;
                total_blue += temp[i + 1][j].rgbtBlue;
                count += 1;
                // column to the left
                if (j - 1 >= 0)
                {
                    total_red += temp[i + 1][j - 1].rgbtRed;
                    total_green += temp[i + 1][j - 1].rgbtGreen;
                    total_blue += temp[i + 1][j - 1].rgbtBlue;
                    count += 1;
                }
                // column to the right
                if (j + 1 < width)
                {
                    total_red += temp[i + 1][j + 1].rgbtRed;
                    total_green += temp[i + 1][j + 1].rgbtGreen;
                    total_blue += temp[i + 1][j + 1].rgbtBlue;
                    count += 1;
                }
            }

            image[i][j].rgbtRed = round(total_red * 1.0 / count);
            image[i][j].rgbtGreen = round(total_green * 1.0 / count);
            image[i][j].rgbtBlue = round(total_blue * 1.0 / count);
        }
    }
}
