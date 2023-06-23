/*---------------------------------------------------------
 * Image Processing
 * 
 * Steganography - decode
 *---------------------------------------------------------*/

#include <stdio.h>
#include <stdint.h>
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
    unsigned int extraction;

    // decode name
    int flag = 1;
    int count = 0;
    while(flag) {
        if ((count + 1) % 3 == 0) {
            extraction = In->px[count] & 0xFF;
        } else if ((count + 1) % 3 == 1) {
            extraction = (In->px[count] & 0xFF0000) >> 16;
        } else if ((count + 1) % 3 == 2) {
            extraction = (In->px[count] & 0xFF00) >> 8;
        }

        printf("%d", (extraction & 0x01));

        byte <<= 1;
        byte |= (extraction & 0x01);

        bitCount++;

        if (bitCount == 8) {
            printf(" --> ");
            asciiChar = byte;
            
            char asciiString[2] = {asciiChar, '\0'};
            printf("ASCII = [%c]\n", asciiChar);
            strcat(name, asciiString);

            if (!asciiChar) {
                flag = 0;
            }

            byte = 0;
            bitCount = 0;
        }
        count++;
    }
    
    printf("\nFile name: %s\n", name);
    

    // decode size
    uint32_t binarySize = 0;
    for (int i = 0; i < 32; i++, count++) {
        if ((count + 1) % 3 == 0) {
            extraction = In->px[count] & 0xFF;
        } else if ((count + 1) % 3 == 1) {
            extraction = (In->px[count] & 0xFF0000) >> 16;
        } else if ((count + 1) % 3 == 2) {
            extraction = (In->px[count] & 0xFF00) >> 8;
        }

        // printf("%d", (extraction & 0x01));

        binarySize <<= 1;
        binarySize |= (extraction & 0x01);
    }

    fsize = binarySize * 8;
    printf("File size: %d bytes\n", fsize);

    // decode file
    file = fopen(name, "wb");
    if (!file)
    {
        printf("Cannot create file %s\n", name);
        exit(10);
    }

    printf("escrevendo...\n");
    while (fsize) {
        if ((count + 1) % 3 == 0) {
            extraction = In->px[count] & 0xFF;
        } else if ((count + 1) % 3 == 1) {
            extraction = (In->px[count] & 0xFF0000) >> 16;
        } else if ((count + 1) % 3 == 2) {
            extraction = (In->px[count] & 0xFF00) >> 8;
        }

        byte <<= 1;
        byte |= (extraction & 0x01);
        bitCount++;
        if (bitCount == 8) {
            fwrite(&byte, sizeof(byte), 1, file);
            byte = 0;
            bitCount = 0;
        }
        count++;
        fsize--;
    }

    printf("terminado!\n");
    fclose(file);
    free(name);
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
