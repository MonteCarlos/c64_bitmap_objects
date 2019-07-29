#include <stdio.h>
#include <stdint.h>

typedef struct{
    union{
        uint8_t bytes[64];
        uint8_t rows[21][3];
    };
} spriteMatrix_t;

unsigned char loBitsTable[] = {
    0x00, 0x07, 0x38, 0x3f, 0xc0, 0xc7, 0xf8, 0xff,
    0x00, 0x07, 0x38, 0x3f, 0xc0, 0xc7, 0xf8, 0xff
};

unsigned char hiBitsTable[] = {
    0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01,
    0x0e, 0x0e, 0x0e, 0x0e, 0x0f, 0x0f, 0x0f, 0x0f
};

void PrintBits (unsigned long int l) {
    if (l) {
        PrintBits (l >> 1);
        putchar (l & 1 ? '1' : '0');
    }
}

unsigned long int mkBitTriples (unsigned char num) {
    unsigned long int result =
        (unsigned long int) loBitsTable[num & 15] |
        (unsigned long int)hiBitsTable[num & 15] << 8 |
        (unsigned long int)loBitsTable[num >> 4] << 12 |
        (unsigned long int)hiBitsTable[num >> 4] << 20;
    return result;
}

int main (void) {
    unsigned char number = 0xaa;
    unsigned long int result;
    int i;

    spriteMatrix_t *spritematrix = (void*)0x2000;

    PrintBits (number);
    putchar ('\n');
    result = mkBitTriples (number);
    PrintBits (result);
    putchar ('\n');
    for (i = 0; i<21; ++i){
        PrintBits ((uint32_t)spritematrix->rows[i][0]<<16|(uint32_t)spritematrix->rows[i][1]<<8|spritematrix->rows[i][2]);
        putchar ('\n');
    }
    return 0;
}
