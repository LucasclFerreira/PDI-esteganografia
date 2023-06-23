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

    printf("numero de linhas: %d\n", In->nr);
    printf("numero de colunas: %d\n", In->nc);

    // char *str = calloc(100, sizeof(char));
    unsigned int extraction;
    // char cor;

    int flag = 1;
    int cont = 0;
    while(flag) {
        if ((cont + 1) % 3 == 0) {
            extraction = In->px[cont] & 0xFF;
        } else if ((cont + 1) % 3 == 1) {
            extraction = (In->px[cont] & 0xFF0000) >> 16;
        } else if ((cont + 1) % 3 == 2) {
            extraction = (In->px[cont] & 0xFF00) >> 8;
        }

        printf("%d", (extraction & 0x01));

        byte <<= 1;
        byte |= (extraction & 0x01);

        bitCount++;

        if (bitCount == 8) {
            printf(" --> ");
            asciiChar = byte;
            
            char asciiString[2] = {asciiChar, '\0'};
            printf("ASCII = [%c] no cont = [%d]\n", asciiChar, cont);
            strcat(name, asciiString);

            if (!asciiChar) {
                flag = 0;
            }

            byte = 0;
            bitCount = 0;
        }
        cont++;
    }
    printf("\nvalor atual do cont: %d\n", cont);
    
    printf("\nFile name: %s\n", name);
    
    int val;
    // decode file size
    for (int i = 0; i < 32; i++, cont++) {
        if ((cont + 1) % 3 == 0) {
            extraction = In->px[cont] & 0xFF;
        } else if ((cont + 1) % 3 == 1) {
            extraction = (In->px[cont] & 0xFF0000) >> 16;
        } else if ((cont + 1) % 3 == 2) {
            extraction = (In->px[cont] & 0xFF00) >> 8;
        }

        printf("%d", (extraction & 0x01));

        byte <<= 1;
        byte |= (extraction & 0x01);

        bitCount++;

        if (bitCount == 8) {
            printf("\n");
            val = byte;
            byte = 0;
            bitCount = 0;
        }
    }

    // fsize = 13776;
    fsize = 75084;
    printf("File size: %d bytes\n", fsize);
    
    // // decode file
    
    file = fopen(name, "wb");
    if (!file)
    {
        printf("Cannot create file %s\n", name);
        exit(10);
    }
    // decode the bytes of the file

    // unsigned int *dados = malloc(fsize * sizeof(byte));
    // int pos = 0;
    printf("escrevendo...\n");
    while (fsize) {
        if ((cont + 1) % 3 == 0) {
            extraction = In->px[cont] & 0xFF;
        } else if ((cont + 1) % 3 == 1) {
            extraction = (In->px[cont] & 0xFF0000) >> 16;
        } else if ((cont + 1) % 3 == 2) {
            extraction = (In->px[cont] & 0xFF00) >> 8;
        }

        byte <<= 1;
        byte |= (extraction & 0x01);
        bitCount++;
        if (bitCount == 8) {
            // Quando o byte estiver completo escreve no arquivo
            //fwrite(&byte, sizeof(byte), 1, file);
            // printf("bit completo no cont = [%d]\n", cont);
            // dados[pos++] = byte;
            fwrite(&byte, sizeof(byte), 1, file);
            
            // Reset variables for the next byte
            byte = 0;
            bitCount = 0;
        }
        cont++;
        fsize--;
    }

    printf("terminado!\n");
    fclose(file);
    free(name);
    // free(dados);
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
