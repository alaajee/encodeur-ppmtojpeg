#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>
#define LINE_WIDTH 16
#define MAX_LINE_LENGTH 1024
#define INITIAL_CAPACITY 100
#include "../src/read_image.c"

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
    // Supposons que lirePremiereLigne initialise aussi les facteurs de sous-échantillonnage h et v
    info = lirePremiereLigne(fichier,2,1); 

    // Calcul du nombre de MCUs compte tenu des facteurs de sous-échantillonnage
    int mcuWidth = 8 * info.h1;
    int mcuHeight = 8 * info.v1;
    info.nbr_MCUS = ((info.largeur + mcuWidth - 1) / mcuWidth) * ((info.longueur + mcuHeight - 1) / mcuHeight);

    printf("Type : %s\n", info.type);
    printf("Longueur : %hu\n", info.longueur);
    printf("Largeur : %hu\n", info.largeur);
    printf("Nombre de MCUs : %u\n", info.nbr_MCUS);
    
    Matrix matrix;
    MCU mat;
    int x, y;
    int mcuIndex = 0;
   
    for (y = 0; y < info.longueur; y += mcuHeight) {
        for (x = 0; x < info.largeur; x += mcuWidth) {
            printf("Traitement de MCU %d à la position (%d, %d)\n", mcuIndex, x, y);
            recupere_MCUS1(fichier, info, &mat, x, y);
            matrices(&mat ,   &matrix , 0, 8);
            mcuIndex++;
        }
    }

    fclose(fichier);
    return 0;
    }
