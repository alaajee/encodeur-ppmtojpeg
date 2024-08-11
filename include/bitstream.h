#ifndef BITWRITER_H
#define BITWRITER_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define BYTE_SIZE 8 // Taille d'un octet

typedef struct {
    uint8_t *data;
    size_t capacity;
    size_t size;
} BitWriter;

BitWriter* initBitWriter(size_t capacity);
void freeBitWriter(BitWriter* writer);
void writeBit(BitWriter* writer, uint8_t bit);
void writeBits(BitWriter* writer, uint32_t bits, uint8_t length);

#endif /* BITWRITER_H */
