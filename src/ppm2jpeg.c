#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>
#define LINE_WIDTH 16
#define MAX_LINE_LENGTH 1024
#define INITIAL_CAPACITY 100
#include <math.h>
#include "downsampling.c"
#define N 8


void options_acceptees(void) {
    printf("Usage: ./ppm2jpeg [options] image.{pgm, ppm} \n");
    printf("Options:\n");
    printf("  --help                  Affiche cette aide.\n");
    printf("  --outfile=sortie.jpg    Définit le nom du fichier de sortie.\n");
    printf("  --sample=h1xv1,h2xv2,h3xv3\n");
    printf("Définit les facteurs d'échantillonnage hxv des trois composantes de couleur.\n");
}

char* nom_fichier(const char* nom_fichier) {
    char* nouveau_nom = malloc(strlen(nom_fichier) + strlen(".jpg") + 1);
    if (nouveau_nom == NULL) {
        fprintf(stderr, "Erreur d'allocation de mémoire.\n");
        exit(EXIT_FAILURE);
    }
    strcpy(nouveau_nom, nom_fichier);
    char* point = strrchr(nouveau_nom, '.');
    if (point) {
        *point = '\0';
    }
    strcat(nouveau_nom, ".jpg");
    return nouveau_nom;
}


int main(int argc, char *argv[]) {
    if (argc < 2) {
        options_acceptees();
        exit(EXIT_FAILURE);
    }
    char* image = NULL;
    char* fichier_sortie = NULL; 
    int h1 = 1, v1 = 1, h2 = 1, v2 = 1, h3 = 1, v3 = 1;
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--help") == 0) {
            options_acceptees();
            exit(EXIT_SUCCESS);
        } 
        else if (strncmp(argv[i], "--outfile=", 10) == 0) {
            fichier_sortie = argv[i] + 10;
        } 
        else if (strncmp(argv[i], "--sample=", 9) == 0) {
            if (sscanf(argv[i] + 9, "%dx%d,%dx%d,%dx%d", &h1, &v1, &h2, &v2, &h3, &v3) != 6) {
                fprintf(stderr, "Erreur de format pour l'option --sample. Utilisation: --sample=h1xv1,h2xv2,h3xv3\n");
                exit(EXIT_FAILURE);
                }
        } 
            else if (argv[i][0] != '-') {
                image = argv[i];
        } 
            else {
            
                fprintf(stderr, "Option non reconnue: %s\n", argv[i]);
                options_acceptees();
                exit(EXIT_FAILURE);
        }
    }
    if (image == NULL) {
        fprintf(stderr, "Aucun fichier image spécifié.\n");
        options_acceptees();
        exit(EXIT_FAILURE);
    }
    if (fichier_sortie == NULL) {
        fichier_sortie = nom_fichier(image);
    }
    FILE* fichier = fopen(image, "rb");
    if (!fichier) {
        fprintf(stderr, "Impossible d'ouvrir le fichier image\n");
        exit(EXIT_FAILURE);
    }
    if ((h1 % h2 != 0 || v1 % v2 != 0) || (h1 % h3 != 0 || v1 % v3 != 0)) {
        fprintf(stderr, "Erreur: h2 et v2 doivent diviser h1 and v1. Meme chose pour h3 et v3.\n");
        exit(EXIT_FAILURE);
    }
    if (h1 * v1 + h2 * v2 + h3 * v3 > 10) {
        fprintf(stderr, "Erreur: La somme de hi * vi ne doit pas dépasser 10.\n");
        exit(EXIT_FAILURE);
    }
    ImageInfo info; 
    info = lirePremiereLigne(fichier,h1,v1,h2,v2,h3,v3); 
    int mcuWidth = 8 * info.h1;
    int mcuHeight = 8 * info.v1;
    info.nbr_MCUS = ((info.largeur + mcuWidth - 1) / mcuWidth) * ((info.longueur + mcuHeight - 1) / mcuHeight);
    // printf("Type : %s\n", info.type);
    // printf("Longueur : %hu\n", info.longueur);
    // printf("Largeur : %hu\n", info.largeur);
    // printf("Nombre de MCUs : %u\n", info.nbr_MCUS);
    MCU *mat = (MCU *)malloc(sizeof(MCU));
    if (mat == NULL) {
        fprintf(stderr, "Erreur d'allocation de la mémoire pour MCU.\n");
        return 1;
    }
    Matrix *matrix = (Matrix *)malloc(sizeof(Matrix));
    if (matrix == NULL) {
        fprintf(stderr, "Erreur d'allocation de la mémoire pour Matrix.\n");
        free(mat);  
        return 1;
    }
    YCbCr *ycbcr = (YCbCr *)malloc(sizeof(YCbCr));
    if (ycbcr == NULL) {
        fprintf(stderr, "Erreur d'allocation de la mémoire pour YCbCr.\n");
        free(mat);    
        free(matrix); 
        return 1;
    }
    unsigned int x, y;
    int dc = 0;
    int dc1 = 0;
    int dc2 = 0;
    int dc3 =0;
    int nbr_de_blocs1 = info.h1*info.v1;
    int nbr_de_blocs2 = info.h2*info.v2;
    int nbr_de_blocs3 = info.h3*info.v3;
    BitWriter* writer = initBitWriter(10);
    int sample_type = 0; 
    int color_component = 0;
    
    uint8_t *symbols = htables_symbols[sample_type][color_component];
    uint8_t *num_symbols_per_length = htables_nb_symb_per_lengths[sample_type][color_component];
    int num_symbols = htables_nb_symbols[sample_type][color_component];
    HuffmanTable huffman_dc[num_symbols];
    buildCanonicalHuffman(huffman_dc, num_symbols_per_length);
    sample_type = 1; 
    color_component = 0; 
    uint8_t *symbols_ac = htables_symbols[sample_type][color_component];
    uint8_t *num_symbols_per_length_ac = htables_nb_symb_per_lengths[sample_type][color_component];
    int num_symbols_ac = htables_nb_symbols[sample_type][color_component];
    HuffmanTable huffman_ac[num_symbols_ac];
    buildCanonicalHuffman(huffman_ac, num_symbols_per_length_ac);
    int sample_type_Cb = 0; 
    int color_component_Cb = 1; 
    uint8_t *symbols_Cb = htables_symbols[sample_type_Cb][color_component_Cb];
    uint8_t *num_symbols_per_length_Cb = htables_nb_symb_per_lengths[sample_type_Cb][color_component_Cb];
    int num_symbols_Cb = htables_nb_symbols[sample_type_Cb][color_component_Cb];
    HuffmanTable huffman_dc_Cb[num_symbols_Cb];
    buildCanonicalHuffman(huffman_dc_Cb, num_symbols_per_length_Cb);
    sample_type_Cb = 1;
    color_component_Cb = 1;
    uint8_t *symbols_ac_Cb = htables_symbols[sample_type_Cb][color_component_Cb];
    uint8_t *num_symbols_per_length_ac_Cb = htables_nb_symb_per_lengths[sample_type_Cb][color_component_Cb];
    int num_symbols_ac_Cb = htables_nb_symbols[sample_type_Cb][color_component_Cb];
    HuffmanTable huffman_ac_Cb[num_symbols_ac_Cb];
    buildCanonicalHuffman(huffman_ac_Cb, num_symbols_per_length_ac_Cb);
    int sample_type_Cr = 0; 
    int color_component_Cr = 2;
    uint8_t *symbols_Cr = htables_symbols[sample_type_Cr][color_component_Cr];
    uint8_t *num_symbols_per_length_Cr = htables_nb_symb_per_lengths[sample_type_Cr][color_component_Cr];
    int num_symbols_Cr = htables_nb_symbols[sample_type_Cr][color_component_Cr];
    HuffmanTable huffman_dc_Cr[num_symbols_Cr];
    buildCanonicalHuffman(huffman_dc_Cr, num_symbols_per_length_Cr);
    sample_type_Cr = 1;
    color_component_Cr = 2; 
    uint8_t *symbols_ac_Cr = htables_symbols[sample_type_Cr][color_component_Cr];
    uint8_t *num_symbols_per_length_ac_Cr = htables_nb_symb_per_lengths[sample_type_Cr][color_component_Cr];
    int num_symbols_ac_Cr = htables_nb_symbols[sample_type_Cr][color_component_Cr];
    HuffmanTable huffman_ac_Cr[num_symbols_ac_Cr];
    buildCanonicalHuffman(huffman_ac_Cr, num_symbols_per_length_ac_Cr);  
    FILE* fichier_jpeg = fopen(fichier_sortie , "wb");   
    appx(fichier_jpeg);
    entete_DQT(fichier_jpeg,info);
    entete_SOF(fichier_jpeg, info, info.h1 , info.v1 , info.h2, info.v2 , info.h3 , info.v3);
    entete_DHT(fichier_jpeg); 
    if (strcmp(info.type, "P5") == 0){
    	uint8_t sos[] = {0xff, 0xda, 0x00, 0x08, 0x01, 0x01, 0x00, 0x00, 0x3f, 0x00};
    	fwrite(&sos, sizeof(sos), 1, fichier_jpeg);
	    for (y = 0; y < info.longueur; y += mcuHeight) {
       	    for (x = 0; x < info.largeur; x += mcuWidth) {
                recupere_MCUS(fichier, info,mat, x,y);
                int matt[8][8];
                calcul_dct(mat->matrice1, matt);               
                zigzag(matt, matt);
                int matt1[8][8];
                divide_by_quantification(matt, quantification_table_Y,matt1 );		     		     		
                encode_dc(matt1, huffman_dc, symbols, num_symbols, writer, &dc);
                encode_ac(matt1, huffman_ac, symbols_ac, num_symbols_ac, writer);                   	     		
            }
            
        }     	
	    	}  
    else {
        uint8_t sos[] = {0xff, 0xda, 0x00, 0x0c, 0x03, 0x01, 0x00, 0x02,0x11,0x03,0x11,0x00, 0x3f, 0x00};
        fwrite(&sos, sizeof(sos), 1, fichier_jpeg); 
    	for (y = 0; y < info.longueur; y += mcuHeight) {
	    for (x = 0; x < info.largeur; x += mcuWidth) {
		recupere_MCUS1(fichier, info, mat, x, y);
		Matrix YY[nbr_de_blocs1+1];
		Matrix CB[nbr_de_blocs2+1];
		Matrix CR[nbr_de_blocs3+1];
		RGB_to_YCbCr(info.h1, info.v1, mat, ycbcr);		
		if (info.h1 == info.h2 && info.v1 == info.v2) {
			} else if (info.h1 == info.h2 && info.v1 != info.v2) {
			    sous_echantillonnage_vertical(info.h1, info.v1, ycbcr->Cb[0].matrice2);
			} else if (info.h1 != info.h2 && info.v1 == info.v2) {
			    sous_echantillonnage_horizontal(info.h1, info.v1, ycbcr->Cb[0].matrice2);
			} else if (info.h1 != info.h2 && info.v1 != info.v2) {
			    sous_echantillonnage_horizontal_vertical(info.h1, info.v1, info.h2, info.v2, ycbcr->Cb[0].matrice2);
			}
        if (info.h1 == info.h3 && info.v1 == info.v3) {
			} else if (info.h1 == info.h3 && info.v1 != info.v3) {
			    sous_echantillonnage_vertical(info.h1, info.v1, ycbcr->Cr[0].matrice2);
			} else if (info.h1 != info.h3 && info.v1 == info.v3) {
			    sous_echantillonnage_horizontal(info.h1, info.v1, ycbcr->Cr[0].matrice2);
			} else if (info.h1 != info.h3 && info.v1 != info.v3) {
			    sous_echantillonnage_horizontal_vertical(info.h1, info.v1, info.h3, info.v3, ycbcr->Cr[0].matrice2);
			}
		divise_Cb(info.h2, info.v2, ycbcr);
		divise_Cr(info.h3, info.v3, ycbcr);		
		for (int k = 0; k < nbr_de_blocs1; k++){
			calcul_dct(ycbcr->Y[k].matrice1, YY[k].matrice1);
			
		}
		for (int k = 1; k < nbr_de_blocs2+1; k++){
			calcul_dct(ycbcr->Cb[k].matrice1, CB[k].matrice1);
		}
		for (int k = 1; k < nbr_de_blocs3+1; k++){
		 	calcul_dct(ycbcr->Cr[k].matrice1, CR[k].matrice1);
		 }
		for (int k = 0; k < nbr_de_blocs1; k++){
			zigzag(YY[k].matrice1, YY[k].matrice1);
		}
	    
	    for (int k = 1; k < nbr_de_blocs2+1; k++){
			zigzag(CB[k].matrice1, CB[k].matrice1);
	    }
		for (int k = 1; k < nbr_de_blocs3+1; k++){
			zigzag(CR[k].matrice1, CR[k].matrice1);
	    }	      
	 	for (int k = 0; k < nbr_de_blocs1; k++){
	 		 divide_by_quantification(YY[k].matrice1, quantification_table_Y, YY[k].matrice1);	 	
	 	}
	 	for (int k = 1; k < nbr_de_blocs2+1; k++){
	 		divide_by_quantification(CB[k].matrice1, quantification_table_CbCr, CB[k].matrice1);	 	
	 	}
		for (int k = 1; k < nbr_de_blocs3+1; k++){
	 		divide_by_quantification(CR[k].matrice1, quantification_table_CbCr, CR[k].matrice1);	 	
	 	}	
		for (int k = 0; k < nbr_de_blocs1; k++){
		    encode_dc(YY[k].matrice1, huffman_dc, symbols, num_symbols, writer, &dc1);
		    encode_ac(YY[k].matrice1, huffman_ac, symbols_ac, num_symbols_ac, writer);
		}
	   	for (int k = 1; k < nbr_de_blocs2+1; k++){
	   	    encode_dc(CB[k].matrice1, huffman_dc_Cb, symbols_Cb, num_symbols_Cb, writer, &dc2);
	       	encode_ac(CB[k].matrice1, huffman_ac_Cb, symbols_ac_Cb, num_symbols_ac_Cb, writer);
	    }
		for (int k = 1; k < nbr_de_blocs3+1; k++){
		    encode_dc(CR[k].matrice1, huffman_dc_Cr, symbols_Cr, num_symbols_Cr, writer, &dc3);
	       	encode_ac(CR[k].matrice1, huffman_ac_Cr, symbols_ac_Cr, num_symbols_ac_Cr, writer);	    
		}
	    }
	}
    }
    
	    uint8_t byte_to_write = 0;
	    size_t bit_count = 0;
	    for (size_t i = 0; i < writer->size; ++i) {
		int bit_a_ecrire = (writer->data[i / BYTE_SIZE] >> (BYTE_SIZE - 1 - i % BYTE_SIZE)) & 1;
		byte_to_write |=  bit_a_ecrire << (BYTE_SIZE - 1 - bit_count);
		bit_count++;
		if (bit_count == BYTE_SIZE) {
		    if (byte_to_write == 0xFF) {
		        fwrite(&byte_to_write, sizeof(byte_to_write), 1, fichier_jpeg);
		        uint8_t fill_byte = 0x00;
		        fwrite(&fill_byte, sizeof(fill_byte), 1, fichier_jpeg);
		    } else {
		        fwrite(&byte_to_write, 1, 1, fichier_jpeg);
		    }
		    byte_to_write = 0;
		    bit_count = 0;
		}
	    }
	    if (bit_count > 0) {
            if (byte_to_write == 0xFF) {
                fwrite(&byte_to_write, sizeof(byte_to_write), 1, fichier_jpeg);
                uint8_t fill_byte = 0x00;
                fwrite(&fill_byte, sizeof(fill_byte), 1, fichier_jpeg);
            } 
		else {
		    fwrite(&byte_to_write, 1, 1, fichier_jpeg);
	    }
	    } 
	    uint8_t sos1[] = {0xff,0xd9};
	    fwrite(&sos1, sizeof(sos1), 1, fichier_jpeg);	    
	    free(mat);
    	    free(matrix);
    	    free(ycbcr);
    	    freeBitWriter(writer);	
    } 
