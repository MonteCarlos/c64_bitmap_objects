#include <stdio.h>
#include <stdint.h>

void generateSprites(void);

typedef struct{
    union{
        uint8_t bytes[64];
        uint8_t rows[21][3];
    };
} spriteMatrix_t;

uint8_t loBitsTable[] = {
    0x00, 0x07, 0x38, 0x3f, 0xc0, 0xc7, 0xf8, 0xff,
    0x00, 0x07, 0x38, 0x3f, 0xc0, 0xc7, 0xf8, 0xff
};

uint8_t hiBitsTable[] = {
    0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01,
    0x0e, 0x0e, 0x0e, 0x0e, 0x0f, 0x0f, 0x0f, 0x0f
};

void PrintBits (uint32_t l) {
    int8_t i;
    uint32_t mask = (uint32_t)1 << (8*sizeof(l)-1);

    for (i = sizeof(l)*8-1; i >= 0; --i){
        putchar (l & mask ? '1' : '0');
        l <<= 1;
    }
}

uint32_t mkBitTriples (uint8_t num) {
    uint32_t result =
        (uint32_t)loBitsTable[num & 15] |
        (uint32_t)hiBitsTable[num & 15] << 8 |
        (uint32_t)loBitsTable[num >> 4] << 12 |
        (uint32_t)hiBitsTable[num >> 4] << 20;
    return result;
}

int main (void) {
    uint8_t number = 0xaa;
    uint32_t result;
    int i;

    spriteMatrix_t *spritematrix = (void*)0x2000;

    PrintBits (number);
    putchar ('\n');
    result = mkBitTriples (number);
    PrintBits (result);
    putchar ('\n');

    generateSprites();
    for (i = 0; i<21; ++i){
        PrintBits (
                   ((uint32_t)spritematrix->rows[i][0]<<16)|
                   ((uint32_t)spritematrix->rows[i][1]<<8)|
                   spritematrix->rows[i][2]
                   );
        putchar ('\n');
    }
    getchar();
    return 0;
}
