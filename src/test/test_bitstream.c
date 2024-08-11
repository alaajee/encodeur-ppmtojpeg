#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "../src/bitstream.c"
#define BYTE_SIZE 8 // Taille d'un octet

int main() {
    BitWriter* writer = initBitWriter(10);
    if (writer == NULL) {
        fprintf(stderr, "Erreur lors de l'initialisation du BitWriter.\n");
        return 1;
    }

    writeBits(writer,6,3);

    printf("Bits écrits : ");
    for (size_t i = 0; i < writer->size; ++i) {
        printf("%d", (writer->data[i / BYTE_SIZE] >> (BYTE_SIZE - 1 - i % BYTE_SIZE)) & 1);
    }
    printf("\n");

    FILE *fp;
    fp = fopen("output.txt", "w");
    for (size_t i = 0; i < writer->size; ++i) {
        char bit_a_ecrire = (writer->data[i / BYTE_SIZE] >> (BYTE_SIZE - 1 - i % BYTE_SIZE)) & 1;
        fprintf(fp,"%d",bit_a_ecrire);
    }

    freeBitWriter(writer);

    return 0;
}

