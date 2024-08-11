#ifndef ACDC_H
#define ACDC_H

#include "huffman.h"
#include "bitstream.h"



int magnitude(int valeur);
int index_huffman(int valeur);
int codage(int valeur, HuffmanTable *huffman_symbols, uint8_t *symbols, int num_symbols, uint8_t *num_bits);
void encode_dc(int MCU[8][8], HuffmanTable *huffman_dc, uint8_t *symbols_dc, int num_symbols, BitWriter *writer, int *last_dc);
int encode_ac(int MCU[8][8], HuffmanTable *huffman_ac, uint8_t *symbols_ac, int num_symbols, BitWriter *writer);

#endif /* ACDC_H */
