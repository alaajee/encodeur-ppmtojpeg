#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <htables.h>
#include <include/huffman.h>

int main() {
    int sample_type = 0; // 0 pour DC et 1 pour AC
    int color_component = 0; // 0 pour Y, 1 pour Cb et 2 pour Cr

    uint8_t *symbols = htables_symbols[sample_type][color_component];
    uint8_t *num_symbols_per_length = htables_nb_symb_per_lengths[sample_type][color_component];
    int num_symbols = htables_nb_symbols[sample_type][color_component];

    HuffmanTable huffman_symbols[num_symbols];


    buildCanonicalHuffman(huffman_symbols, num_symbols_per_length, num_symbols);

    // Exemple : Recherche de l'encodage Huffman de la première valeur dans le tableau
    int encoding = findHuffmanEncoding(5, huffman_symbols, symbols, num_symbols);
    printf("Encodage Huffman : %d\n", encoding);

    printf("Codage de Huffman :\n");
    for (int i = 0; i < num_symbols; i++) {
        printf("Path: ");
        for (int j = 0; j < huffman_symbols[i].code_length; j++) {
            printf("%d", (huffman_symbols[i].huffman_code >> (huffman_symbols[i].code_length - 1 - j)) & 1);
        }
        printf(" Symbol: %x\n", *symbols);
        symbols++;
    }

    

    return 0;
}


