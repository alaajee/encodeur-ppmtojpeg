#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <stdint.h>

typedef struct {    
    uint8_t code_length;
    uint32_t huffman_code;
} HuffmanTable;

void buildCanonicalHuffman(HuffmanTable *symbols, uint8_t *num_symbols_per_length);
int findHuffmanEncoding(uint8_t value, HuffmanTable *huffman_table, uint8_t *tables_symbols, int num_symbols, uint8_t *num_bits);

#endif /* HUFFMAN_H */
