#include <stdio.h>
#include <math.h>
#include "huffman.c"
#include "bitstream.c"
#include "../src/acdc.c"
#include "htables.c"

int main(){
    const char *hex_values[8][8] = {
        {"ff4e", "ffff", "0000", "0000", "0000", "0000", "0000", "0000"},
        {"0000", "0000", "0000", "0000", "0000", "0000", "0000", "0000"},
        {"0000", "0000", "0000", "0000", "0000", "0000", "0000", "0000"},
        {"ffff", "0001", "0000", "0000", "0000", "0000", "0000", "0000"},
        {"0000", "0000", "0000", "0000", "0000", "0000", "0000", "0000"},
        {"0000", "0000", "0000", "0000", "0000", "0000", "0000", "0000"},
        {"0000", "0000", "0000", "0000", "0000", "0000", "0000", "0000"},
        {"0000", "0000", "0000", "0000", "0000", "0000", "0000", "0000"}
    };

    int32_t matrixx[8][8];

    // Conversion des valeurs hexadécimales en int32_t
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            sscanf(hex_values[i][j], "%x", &matrixx[i][j]);
        }
    }
     FILE *fp;
     fp = fopen("output.txt", "w");
     BitWriter* writer = initBitWriter(10);

     int sample_type = 0; // 0 pour DC et 1 pour AC
     int color_component = 0; // 0 pour Y, 1 pour Cb et 2 pour Cr
     int last_dc =65358;

     uint8_t *symbols = htables_symbols[sample_type][color_component];
     uint8_t *num_symbols_per_length = htables_nb_symb_per_lengths[sample_type][color_component];
     int num_symbols = htables_nb_symbols[sample_type][color_component];

     HuffmanTable huffman_dc[num_symbols];
     buildCanonicalHuffman(huffman_dc, num_symbols_per_length, num_symbols);

     sample_type = 1; // 0 pour DC et 1 pour AC
     color_component = 0; // 0 pour Y, 1 pour Cb et 2 pour Cr

     uint8_t *symbols_ac = htables_symbols[sample_type][color_component];
     uint8_t *num_symbols_per_length_ac = htables_nb_symb_per_lengths[sample_type][color_component];
     int num_symbols_ac = htables_nb_symbols[sample_type][color_component];

     HuffmanTable huffman_ac[num_symbols_ac];
     buildCanonicalHuffman(huffman_ac, num_symbols_per_length_ac, num_symbols_ac);


     encode_dc(matrixx,huffman_dc,symbols,num_symbols,writer,&last_dc,0,0);
    
     encode_ac(matrixx,huffman_ac,symbols_ac,num_symbols_ac,writer,0,0); 
     int byte_count =0;
     for (size_t i = 0; i < writer->size; ++i) {
        char bit_a_ecrire = (writer->data[i / BYTE_SIZE] >> (BYTE_SIZE - 1 - i % BYTE_SIZE)) & 1;
        fprintf(fp,"%d",bit_a_ecrire);
        byte_count++;
     }
}
