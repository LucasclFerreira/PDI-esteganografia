/*---------------------------------------------------------
 * Image Processing -
 * -----------------------
 * Steganography
 * By Luiz Eduardo da Silva.
 *---------------------------------------------------------*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "imagelib.h"

#define DEBUG(x)

long int fileSize(FILE *fp)
{
    fseek(fp, 0L, SEEK_END);
    long int res = ftell(fp);
    fseek(fp, 0L, SEEK_SET);
    return res;
}

void codeByte(image In, unsigned char ch, int *i, int *band)
{
    unsigned char r, g, b, r0, g0, b0;
    unsigned char mask = 0x80, bit;
    DEBUG(printf(" %c(%X) ", ch, ch);)
    while (mask)
    {
        r = (In->px[*i] >> 16) & 0xFF;
        g = (In->px[*i] >> 8) & 0xFF;
        b = In->px[*i] & 0xFF;
        if (ch & mask)
            bit = 1;
        else
            bit = 0;
        DEBUG(printf("%d", bit);)
        if (*band == 0)
        {
            r0 = r;
            r = bit ? r | 0x01 : r & 0xFE;
            DEBUG(printf(" r=%x>%x ", r0, r);)
        }
        else if (*band == 1)
        {
            g0 = g;
            g = bit ? g | 0x01 : g & 0xFE;
            DEBUG(printf(" g=%x>%x ", g0, g);)
        }
        else if (*band == 2)
        {
            b0 = b;
            b = bit ? b | 0x01 : b & 0xFE;
            DEBUG(printf(" b=%x>%x ", b0, b);)
        }
        mask = mask >> 1;
        *band = (*band + 1) % 3;
        In->px[*i] = (r << 16) + (g << 8) + b;
        (*i)++;
    }
}

void code(image In, char *name)
{
    FILE *arq;
    unsigned char ch;
    int i, j, band, fsize, qtdPixels, nl = In->nr, nc = In->nc;
    arq = fopen(name, "rb");
    if (!arq)
    {
        printf("Cannot open file: %s\n\n", name);
        exit(10);
    }
    fsize = fileSize(arq);
    qtdPixels = 8 * (strlen(name) + 5 + fsize);
    printf("%s is %ld bytes and fills %d pixels\n", name, fileSize(arq), qtdPixels);
    if (qtdPixels > nl * nc)
    {
        printf("Cannot hidde file %s in image\n\n", name);
        exit(20);
    }
    band = 0;
    i = 0;
    // encode the file name
    j = 0;
    do
    {
        ch = name[j];
        codeByte(In, ch, &i, &band);
        j++;
    } while (name[j]);
    // encode end of string
    codeByte(In, '\0', &i, &band);
    // encode file size
    codeByte(In, (fsize >> 24) & 0xFF, &i, &band);
    codeByte(In, (fsize >> 16) & 0xFF, &i, &band);
    codeByte(In, (fsize >> 8) & 0xFF, &i, &band);
    codeByte(In, fsize & 0xFF, &i, &band);
    // encode the file
    ch = fgetc(arq);
    while (!feof(arq))
    {
        codeByte(In, ch, &i, &band);
        ch = fgetc(arq);
    }
}

void msg(char *s)
{
    printf("\nSteganography - encode");
    printf("\n-------------------------------");
    printf("\nUSO.:  %s  <IMG.PPM> <ARQ.EXT>", s);
    printf("\nONDE:\n");
    printf("    <IMG.PPM>  Image file in PPM ASCII format\n");
    printf("    <ARQ.EXT>  Hidden file\n\n");
    exit(20);
}

/*---------------------------------
  Main program
  ---------------------------------*/
int main(int argc, char *argv[])
{
    char nameIn[100], nameOut[100], cmd[110];
    image In;
    if (argc < 3)
        msg(argv[0]);

    img_name(argv[1], nameIn, nameOut, COLOR, COLOR);

    //-- read image
    In = img_get(nameIn, COLOR);
    //-- transformation
    code(In, argv[2]);
    img_put(In, nameOut, COLOR);

    sprintf(cmd, "%s %s &", VIEW, nameOut);
    system(cmd);
    img_free(In);
    return 0;
}
