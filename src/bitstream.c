#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "../include/bitstream.h"

#define BYTE_SIZE 8 // Taille d'un octet


BitWriter* initBitWriter(size_t capacity) {
    BitWriter* writer = (BitWriter*)malloc(sizeof(BitWriter));
    if (writer == NULL) {
        return NULL;
    }
    writer->capacity = capacity;
    writer->size = 0;
    writer->data = (uint8_t*)malloc(capacity * sizeof(uint8_t));
    if (writer->data == NULL) {
        free(writer);
        return NULL;
    }
    return writer;
}

void freeBitWriter(BitWriter* writer) {
    if (writer != NULL) {
        free(writer->data);
        free(writer);
    }
}

void writeBit(BitWriter* writer, uint8_t bit) {
    if (writer->size >= writer->capacity * BYTE_SIZE) {
        size_t newCapacity = writer->capacity * 2;
        uint8_t* newData = (uint8_t*)malloc(newCapacity * sizeof(uint8_t));
        if (newData == NULL) {
            return;
        }
        for (size_t i = 0; i < writer->size / BYTE_SIZE; ++i) {
            newData[i] = writer->data[i];
        }
        free(writer->data);
        writer->data = newData;
        writer->capacity = newCapacity;
    }
    writer->data[writer->size / BYTE_SIZE] |= (bit & 1) << (BYTE_SIZE - 1 - writer->size % BYTE_SIZE);
    writer->size+=1;
}

void writeBits(BitWriter* writer, uint32_t bits, uint8_t length) {
    for (uint8_t i = 0; i < length; ++i) {
        writeBit(writer, (bits >> (length - 1 - i)) & 1);
    }
}

