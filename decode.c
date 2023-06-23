/*---------------------------------------------------------
 * Image Processing
 * 
 * Steganography - decode
 *---------------------------------------------------------*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "imagelib.h"
#define DEBUG(x) x


void decode(image In)
{
    FILE *file;
    char *name = calloc(100, sizeof(char));
    int fsize;
    unsigned char byte;
    int bitCount = 0;
    char asciiChar = '\0';

    // char *str = calloc(100, sizeof(char));
    unsigned int extraction;
    char cor;
    for (int i = 0; i < 128; i++) {
        if ((i + 1) % 3 == 0) {
            cor = 'b';
            extraction = In->px[i] & 0xFF;
        } else if ((i + 1) % 3 == 1) {
            cor = 'r';
            extraction = (In->px[i] & 0xFF0000) >> 16;
        } else if ((i + 1) % 3 == 2) {
            cor = 'g';
            extraction = (In->px[i] & 0xFF00) >> 8;
        }

        byte <<= 1;
        byte |= (extraction & 0x01);

        bitCount++;

        if (bitCount == 8) {
            asciiChar = byte;  // Assign the binary byte to the ASCII character variable
            if (!asciiChar)
                break;

            // Use the ASCII character as needed
            char asciiString[2] = {asciiChar, '\0'};
            printf("ASCII Character: %c\n", asciiChar);
            strcat(name, asciiString);

            // Reset variables for the next byte
            byte = 0;
            bitCount = 0;
        }

        // if (extraction & 0x01) {
        //     printf("Na BANDA[%c] o ultimo bit eh 1\n", cor);
        // } else {
        //     printf("Na BANDA[%c] o ultimo bit eh 0\n", cor);
        // }
    }

    

    // // decode the name
    // strcpy(name, str);
    
    printf("File name: %s\n", name);
    
    // decode file size
    fsize = 293;
    printf("File size: %d bytes\n", fsize);
    
    // decode file
    
    // file = fopen(name, "wb");
    // if (!file)
    // {
    //     printf("Cannot create file %s\n", name);
    //     exit(10);
    // }
    // while (fsize)
    // {
    //     // decode the bytes of the file

    
    //     fsize--;
    // }
    fclose(file);
}

void msg(char *s)
{
    printf("\nSteganography - decode");
    printf("\n-------------------------------");
    printf("\nUSE.:  %s  <IMG.PPM>", s);
    printf("\nWhere:\n");
    printf("    <IMG.PPM>  Image file in PPM ASCII format\n\n");
    exit(20);
}

/*-----------------------------------------
  Main program
------------------------------------------*/
int main(int argc, char *argv[])
{
    char name[100];
    image In;
    char *p;
    if (argc < 2)
        msg(argv[0]);
    strcpy(name, argv[1]);
    p = strstr(name, ".ppm");
    if (p)
        *p = 0;
    strcat(name, ".ppm");
    In = img_get(name, COLOR);
    decode(In);
    img_free(In);
    return 0;
}
