#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include "downsampling.c"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s fichier_image\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    const char* image = argv[1];
    FILE* fichier = fopen(image, "rb");
    if (!fichier) {
        fprintf(stderr, "Impossible d'ouvrir le fichier image\n");
        exit(EXIT_FAILURE);
    }
    ImageInfo info;
    info = lirePremiereLigne(fichier,2,2,1,1,1,1); 
    int mcuWidth = 8 * info.h1;
    int mcuHeight = 8 * info.v1;
    info.nbr_MCUS = ((info.largeur + mcuWidth - 1) / mcuWidth) * ((info.longueur + mcuHeight - 1) / mcuHeight);
    printf("Type : %s\n", info.type);
    printf("Longueur : %hu\n", info.longueur);
    printf("Largeur : %hu\n", info.largeur);
    printf("Nombre de MCUs : %u\n", info.nbr_MCUS)
    printf("Sous_echantillonnage_facteur1 : %d\n", info.h1);
    printf("Sous_echantillonnage_facteur2 : %d\n", info.v1);
    Matrix matrix;
    MCU mat;
    int x, y;
    int mcuIndex = 0;
    YCbCr matrice;
    for (y = 0; y < info.longueur; y += mcuHeight) {
        for (x = 0; x < info.largeur; x += mcuWidth) {
            printf("Traitement de MCU %d à la position (%d, %d)\n", mcuIndex, x, y);
            recupere_MCUS1(fichier, info, &mat, x, y);
            RGB_to_YCbCr(2,2,&mat, &matrice);
            mcuIndex++;
            printf("[Y]:\n");
            for ( int l = 0 ; l < info.h1*info.v1;++l){
	    	      printf("\n");
		     for (int i = 0; i < 8; ++i) {
			for (int j = 0; j < 8; ++j) {
			    printf("%02x ", matrice.Y[l].matrice1[i][j]);
			}
			printf("\n");
			
		    }
            }
	   
	    for (int i = 0; i < 16;++i) {
			for (int j = 0; j < 16; ++j) {
			    printf("%x ", matrice.Cb[0].matrice2[i][j]);
			}
			printf("\n");
			
		    }
            	printf("\n");
	    printf("\n");
 		for (int i = 0; i < 16; ++i) {
			for (int j = 0; j < 16; ++j) {
			    printf("%x ", matrice.Cr[0].matrice2[i][j]);
			}
			printf("\n");
			
		    }
            	printf("\n");

	    printf("\n");
 	    sous_echantillonnage_horizontal(2,2,matrice.Cb[0].matrice2);
 	    printf("\n[Cb] :\n");
		    
	    sous_echantillonnage_horizontal(2,2,matrice.Cr[0].matrice2);
 	    printf("\n[Cr] :\n");
		for (int i = 0; i < 16; ++i) {
			for (int j = 0; j < 8; ++j) {
			    printf("%x ", matrice.Cb[0].matrice2[i][j]);
			}
			printf("\n");
			
		    }
            	printf("\n");
	
	    for (int i = 0; i < 16; ++i) {
		for (int j = 0; j <8; ++j) {
			printf("%x ", matrice.Cr[0].matrice2[i][j]);
			}
			printf("\n");
			
		    }
            	printf("\n");

 }
 }
 return 0;
 }
