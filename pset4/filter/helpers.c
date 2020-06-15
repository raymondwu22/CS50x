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


// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    int gx_red = 0;
    int gy_red = 0;
    int total_red = 0;
    int gx_green = 0;
    int gy_green = 0;
    int total_green = 0;
    int gx_blue = 0;
    int gy_blue = 0;
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
            gx_red = 0;
            gy_red = 0;
            total_red = 0;
            gx_green = 0;
            gy_green = 0;
            total_green = 0;
            gx_blue = 0;
            gy_blue = 0;
            total_blue = 0;

            gx_red += temp[i][j].rgbtRed * 0;
            gy_red += temp[i][j].rgbtRed * 0;
            gx_green += temp[i][j].rgbtGreen * 0;
            gy_green += temp[i][j].rgbtGreen * 0;
            gx_blue += temp[i][j].rgbtBlue * 0;
            gy_blue += temp[i][j].rgbtBlue * 0;

            // column to the left
            if (j - 1 >= 0)
            {
                gx_red += temp[i][j - 1].rgbtRed * -2;
                gy_red += temp[i][j - 1].rgbtRed * 0;
                gx_green += temp[i][j - 1].rgbtGreen * -2;
                gy_green += temp[i][j - 1].rgbtGreen * 0;
                gx_blue += temp[i][j - 1].rgbtBlue * -2;
                gy_blue += temp[i][j - 1].rgbtBlue * 0;
            }
            // treat the image as if there was a 1 pixel solid black border around the edge of the image
            else
            {
                gx_red += 0 * -2;
                gy_red += 0 * 0;
                gx_green += 0 * -2;
                gy_green += 0 * 0;
                gx_blue += 0 * -2;
                gy_blue += 0 * 0;
            }
            // column to the right
            if (j + 1 < width)
            {
                gx_red += temp[i][j + 1].rgbtRed * 2;
                gy_red += temp[i][j + 1].rgbtRed * 0;
                gx_green += temp[i][j + 1].rgbtGreen * 2;
                gy_green += temp[i][j + 1].rgbtGreen * 0;
                gx_blue += temp[i][j + 1].rgbtBlue * 2;
                gy_blue += temp[i][j + 1].rgbtBlue * 0;
            }
            else
            {
                gx_red += 0 * 2;
                gy_red += 0 * 0;
                gx_green += 0 * 2;
                gy_green += 0 * 0;
                gx_blue += 0 * 2;
                gy_blue += 0 * 0;
            }
            // row above
            if (i - 1 >= 0)
            {
                gx_red += temp[i - 1][j].rgbtRed * 0;
                gy_red += temp[i - 1][j].rgbtRed * -2;
                gx_green += temp[i - 1][j].rgbtGreen * 0;
                gy_green += temp[i - 1][j].rgbtGreen * -2;
                gx_blue += temp[i - 1][j].rgbtBlue * 0;
                gy_blue += temp[i - 1][j].rgbtBlue * -2;
                // column to the left
                if (j - 1 >= 0)
                {
                    gx_red += temp[i - 1][j - 1].rgbtRed * -1;
                    gy_red += temp[i - 1][j - 1].rgbtRed * -1;
                    gx_green += temp[i - 1][j - 1].rgbtGreen * -1;
                    gy_green += temp[i - 1][j - 1].rgbtGreen * -1;
                    gx_blue += temp[i - 1][j - 1].rgbtBlue * -1;
                    gy_blue += temp[i - 1][j - 1].rgbtBlue * -1;
                }
                else
                {
                    gx_red += 0 * -1;
                    gy_red += 0 * -1;
                    gx_green += 0 * -1;
                    gy_green += 0 * -1;
                    gx_blue += 0 * -1;
                    gy_blue += 0 * -1;
                }
                // column to the right
                if (j + 1 < width)
                {
                    gx_red += temp[i - 1][j + 1].rgbtRed * 1;
                    gy_red += temp[i - 1][j + 1].rgbtRed * -1;
                    gx_green += temp[i - 1][j + 1].rgbtGreen * 1;
                    gy_green += temp[i - 1][j + 1].rgbtGreen * -1;
                    gx_blue += temp[i - 1][j + 1].rgbtBlue * 1;
                    gy_blue += temp[i - 1][j + 1].rgbtBlue * -1;
                }
                else
                {
                    gx_red += 0 * 2;
                    gy_red += 0 * 0;
                    gx_green += 0 * 2;
                    gy_green += 0 * 0;
                    gx_blue += 0 * 2;
                    gy_blue += 0 * 0;
                }
            }
            else
            {
                gx_red += 0 * 0;
                gy_red += 0 * -2;
                gx_green += 0 * 0;
                gy_green += 0 * -2;
                gx_blue += 0 * 0;
                gy_blue += 0 * -2;
            }
            // row below
            if (i + 1 < height)
            {
                gx_red += temp[i + 1][j].rgbtRed * 0;
                gy_red += temp[i + 1][j].rgbtRed * 2;
                gx_green += temp[i + 1][j].rgbtGreen * 0;
                gy_green += temp[i + 1][j].rgbtGreen * 2;
                gx_blue += temp[i + 1][j].rgbtBlue * 0;
                gy_blue += temp[i + 1][j].rgbtBlue * 2;
                // column to the left
                if (j - 1 >= 0)
                {
                    gx_red += temp[i + 1][j - 1].rgbtRed * -1;
                    gy_red += temp[i + 1][j - 1].rgbtRed * 1;
                    gx_green += temp[i + 1][j - 1].rgbtGreen * -1;
                    gy_green += temp[i + 1][j - 1].rgbtGreen * 1;
                    gx_blue += temp[i + 1][j - 1].rgbtBlue * -1;
                    gy_blue += temp[i + 1][j - 1].rgbtBlue * 1;
                }
                else
                {
                    gx_red += 0 * -1;
                    gy_red += 0 * 1;
                    gx_green += 0 * -1;
                    gy_green += 0 * 1;
                    gx_blue += 0 * -1;
                    gy_blue += 0 * 1;
                }
                // column to the right
                if (j + 1 < width)
                {
                    gx_red += temp[i + 1][j + 1].rgbtRed * 1;
                    gy_red += temp[i + 1][j + 1].rgbtRed * 1;
                    gx_green += temp[i + 1][j + 1].rgbtGreen * 1;
                    gy_green += temp[i + 1][j + 1].rgbtGreen * 1;
                    gx_blue += temp[i + 1][j + 1].rgbtBlue * 1;
                    gy_blue += temp[i + 1][j + 1].rgbtBlue * 1;
                }
                else
                {
                    gx_red += 0 * 1;
                    gy_red += 0 * 1;
                    gx_green += 0 * 1;
                    gy_green += 0 * 1;
                    gx_blue += 0 * 1;
                    gy_blue += 0 * 1;
                }
            }
            else
            {
                gx_red += 0 * 0;
                gy_red += 0 * 2;
                gx_green += 0 * 0;
                gy_green += 0 * 2;
                gx_blue += 0 * 0;
                gy_blue += 0 * 2;
            }

            total_red = round(sqrt(pow(gx_red, 2) + pow(gy_red, 2)));
            total_blue = round(sqrt(pow(gx_blue, 2) + pow(gy_blue, 2)));
            total_green = round(sqrt(pow(gx_green, 2) + pow(gy_green, 2)));

            if (total_red > 255)
            {
                total_red = 255;
            }
            if (total_blue > 255)
            {
                total_blue = 255;
            }
            if (total_green > 255)
            {
                total_green = 255;
            }

            image[i][j].rgbtRed = total_red;
            image[i][j].rgbtGreen = total_green;
            image[i][j].rgbtBlue = total_blue;
        }
    }
}