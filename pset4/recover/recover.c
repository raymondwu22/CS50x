#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <cs50.h>

typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    // Check usage
    if (argc != 2)
    {
        printf("Usage: ./recover image\n");
        return 1;
    }

    // Open file
    FILE *file = fopen(argv[1], "r");
    if (!file)
    {
        return 1;
    }

    BYTE buffer[512];
    int jpeg_count = 0;
    bool found = false;
    char filename[8];
    FILE *img = NULL;

    // Read 512 bytes into a buffer: fread (data, size, number, inptr);
    // Repeat until end of card
    while (fread(buffer, 512, 1, file) == 1)
    {
        // Check first four bytes for start of JPEG
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            found = true;
            // if first JPEG
            if (jpeg_count == 0)
            {
                // Create a new JPEG file
                sprintf(filename, "%03i.jpg", jpeg_count);
                img = fopen(filename, "w");
                fwrite(buffer, 512, 1, img);
                jpeg_count++;
            }
            else
            {
                // Close file we already wrote to
                fclose(img);
                // Open new file to write to
                sprintf(filename, "%03i.jpg", jpeg_count);
                img = fopen(filename, "w");
                fwrite(buffer, 512, 1, img);
                jpeg_count++;
            }
        }

        else // If already found JPEG
        {
            if (found)
            {
                // Keep writing to the current JPEG
                // fread (data, size, number, outptr);
                fwrite(buffer, 512, 1, img);
            }
        }
    }


    // close any remaining files
    fclose(file);
}
