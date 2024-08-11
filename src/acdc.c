#include <stdio.h>
#include <math.h>
#include "huffman.c"
#include "bitstream.c"
#include <acdc.h>
#include "htables.c"


int magnitude(int valeur){
    int m = 0;
    while (pow(2,m) <=abs(valeur)){
        m++;
    }
    return m;
}

int index_huffman(int valeur){
    if (valeur >=0){
        return valeur;
    }
    int m=magnitude(valeur);
    valeur+=pow(2,m)-1;
    return valeur;
}

int codage(int valeur , HuffmanTable *huffman_symbols, uint8_t *symbols, int num_symbols, uint8_t *num_bits){
    int index = index_huffman(valeur);
    int code = findHuffmanEncoding(index, huffman_symbols, symbols, num_symbols, num_bits);
    return code;

}

void encode_dc(int MCU[8][8], HuffmanTable  *huffman_dc, uint8_t *symbols_dc,int num_symbols, BitWriter* writer, int *last_dc){ 
    int prem_valeur = MCU[0][0]-*last_dc;
    *last_dc = MCU[0][0];
    int m = magnitude(prem_valeur);
    int idx = index_huffman(prem_valeur);
    uint8_t num_bits;
    int code = codage(m, huffman_dc, symbols_dc, num_symbols, &num_bits);
    writeBits(writer, code, num_bits);
    writeBits(writer, idx, m);
    // printf("%d \n", MCU[0][0]);
    // printf("%d \n", prem_valeur);
    // printf("%d \n", code);
    // printf("%d \n", num_bits);
    // printf("%d \n", idx);
    // printf("%d \n", m);
    // printf("\n");
}

int encode_ac(int MCU[8][8], HuffmanTable *huffman_ac, uint8_t *symbols_ac, int num_symbols, BitWriter *writer) {
    int zeros = 0;
    int i = 1;    
    while (i < 64) {
        int ligne = i / 8;
        int colonne = i % 8;
        
        // Comptage des zéros
        while (i < 64 && MCU[ligne][colonne] == 0) {
            zeros += 1;
            i += 1;
            ligne = i / 8;
            colonne = i % 8;
        }
        
        // Si on a atteint la fin du bloc avec des zéros restants
        if (i == 64) {
            uint8_t num_bits =0;
            int code = codage(0x00, huffman_ac, symbols_ac, num_symbols, &num_bits);
            writeBits(writer, code, num_bits);
            return 1;
        }
        while (zeros>=16){
            uint8_t num_bits =0;
            int code = codage(0xF0, huffman_ac, symbols_ac, num_symbols, &num_bits);
            writeBits(writer, code, num_bits);
            zeros-=16;
        }
        // Encode les coefficients non-nuls
        int m = magnitude(MCU[ligne][colonne]);
        int valeur = zeros * 16 + m;
        uint8_t num_bits;
        int code = codage(valeur, huffman_ac, symbols_ac, num_symbols, &num_bits);
        writeBits(writer, code, num_bits);
        int idx = index_huffman(MCU[ligne][colonne]);
        writeBits(writer, idx, m);       
        // Pour le debug
        // if ( (y == 0 && x == 32) | (y == 0 && x == 24) ){
        //     if (i == 2) {
        //         printf("%d \n", code);
        //         printf("%d \n", num_bits);
        //         printf("%d \n", idx);
        //         printf("%d \n", m);
        //     }
        //     printf("\n");
        // }
        i++;
        zeros = 0;
    }
    
    return 1;
}



/*int main(){
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
    
     encode_ac(matrixx,huffman_ac,symbols_ac,num_symbols_ac,writer,0,0); }*/

//     int MCU1[8][8]={
//     {-156, -1, -5, 0, 0x0000, 0, 0x0000, 0},
//     {0, 0, 0x0001, 0, 0x0000, 0x0000, 0, 0x0000},
//     {0x0000, -1, 0x0000, 0, 0x0000, 0x0000, 0, 0x0000},
//     {0x0000, 1, 0x0000, 0, 0x0000, 0x0000, 0, 0x0000},
//     {0x0000, 0, 0x0000, 0, 0x0000, 0x0000, 0, 0x0000},
//     {0x0000, 0, 0x0000, 0, 0x0000, 0x0000, 0, 0x0000},
//     {0x0000, 0, 0x0000, 0, 0x0000, 0x0000, 0, 0x0000}
//     };

//     encode_dc(MCU1,huffman_dc,symbols,num_symbols,writer,&last_dc);
    
//     encode_ac(MCU1,huffman_ac,symbols_ac,num_symbols_ac,writer);

//     int MCU2[8][8]= {
//         {-160, 2, -3, 0, 0, 0, 0, 0},
//         {0, 0, 1, 0, 0, 0, 0, 0},
//         {-1, 0, 0, 0, 0, 0, 0, 0},
//         {0, 1, 0, 0, 0, 0, 0, 0},
//         {0, 0, 0, 0, 0, 0, 0, 0},
//         {0, 0, 0, 0, 0, 0, 0, 0},
//         {0, 0, 0, 0, 0, 0, 0, 0},
//         {0, 0, 0, 0, 0, 0, 0, 0}
//     };

//     encode_dc(MCU2,huffman_dc,symbols,num_symbols,writer,&last_dc);
    
//     encode_ac(MCU2,huffman_ac,symbols_ac,num_symbols_ac,writer);

//     int MCU3[8][8]=  {
//     {-163, 4, -2, 0, 0, 0, 0, 0},
//     {-1, 0, 1, 0, 0, 0, 0, 0},
//     {0, 0, 0, 0, 0, 0, 0, 0},
//     {0, 0, 0, 0, 0, 0, 0, 0},
//     {0, 0, 0, 0, 0, 0, 0, 0},
//     {0, 0, 0, 0, 0, 0, 0, 0},
//     {0, 0, 0, 0, 0, 0, 0, 0},
//     {0, 0, 0, 0, 0, 0, 0, 0}
// };

//     encode_dc(MCU3,huffman_dc,symbols,num_symbols,writer,&last_dc);
    
//     encode_ac(MCU3,huffman_ac,symbols_ac,num_symbols_ac,writer);
//     for (size_t i = 0; i < writer->size; ++i) {
//         char bit_a_ecrire = (writer->data[i / BYTE_SIZE] >> (BYTE_SIZE - 1 - i % BYTE_SIZE)) & 1;
//         fprintf(fp,"%d",bit_a_ecrire);
//         byte_count++;
    

//     int MCU4[8][8]= {
//     {-164, 0, -2, 0, 0, 0, 0, 0},
//     {0, 0, 0, 0, 0, 0, 0, 0},
//     {0, 0, 0, 0, 0, 0, 0, 0},
//     {1, 0, 0, 0, 0, 0, 0, 0},
//     {0, 0, 0, 0, 0, 0, 0, 0},
//     {0, 0, 0, 0, 0, 0, 0, 0},
//     {0, 0, 0, 0, 0, 0, 0, 0},
//     {0, 0, 0, 0, 0, 0, 0, 0}
// };

//     encode_dc(MCU4,huffman_dc,symbols,num_symbols,writer,&last_dc);
    
//     encode_ac(MCU4,huffman_ac,symbols_ac,num_symbols_ac,writer);

//     int byte_count =0;
//     for (size_t i = 0; i < writer->size; ++i) {
//         char bit_a_ecrire = (writer->data[i / BYTE_SIZE] >> (BYTE_SIZE - 1 - i % BYTE_SIZE)) & 1;
//         fprintf(fp,"%d",bit_a_ecrire);
//         byte_count++;
//     }


//     freeBitWriter(writer);
//     return 0;
 

