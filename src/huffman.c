#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <htables.h>
#include <huffman.h>

void buildCanonicalHuffman(HuffmanTable *symbols, uint8_t *num_symbols_per_length) {
    uint32_t huffman_code = 0;
    uint32_t symbol_index = 0;

    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < num_symbols_per_length[i]; j++) {
            symbols[symbol_index].code_length = i + 1;
            symbols[symbol_index].huffman_code = huffman_code;
            huffman_code++;
            symbol_index++;
        }
        huffman_code <<= 1;
    }
}



int findHuffmanEncoding(uint8_t value, HuffmanTable *huffman_table, uint8_t *tables_symbols, int num_symbols, uint8_t *num_bits) {
    for (int i = 0; i < num_symbols; i++) {
        if (*tables_symbols == value) {
            *num_bits = huffman_table[i].code_length;
            return huffman_table[i].huffman_code;
        }
        tables_symbols++;
    }
    // Si la valeur n'est pas trouvée, retourner -1
    *num_bits = 0;
    return -1;
}







