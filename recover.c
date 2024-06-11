#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define BLOCK_SIZE 512

typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
 // Accept a single command-line argument
    if(argc > 2){
        printf("Usage: ./recover FILE\n");
        return 1;
    }

 // Open the memory card
    FILE *card = fopen(argv[1], "r");
     if (card == NULL)
    {
        printf("Could not open %s.\n", argv[1]);
        return 1;
    }

// Create a buffer for a block of data
    BYTE buffer[BLOCK_SIZE];

// variables to track the jpeg files
    FILE *img = NULL;
    int file_index=0;
    char filename[8];


 // While there's still data left to read from the memory card
    while (fread(buffer, sizeof(BYTE), BLOCK_SIZE, card) == BLOCK_SIZE)
    {
        // Check if the block indicates the start of a new JPEG
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            // If a new JPEG is found and an image is already open, close the current image
            if (img != NULL)
            {
                fclose(img);
            }

            // Create a new filename for the new JPEG
            sprintf(filename, "%03i.jpg", file_index++);
            img = fopen(filename, "w");
            if (img == NULL)
            {
                printf("Could not create %s.\n", filename);
                fclose(card);
                return 3;
            }
        }

        // If an image file is currently open, write the buffer to the image file
        if (img != NULL)
        {
            fwrite(buffer, sizeof(BYTE), BLOCK_SIZE, img);
        }
    }

    // Close any remaining files
    if (img != NULL)
    {
        fclose(img);
    }
    fclose(card);

    return 0;

}
