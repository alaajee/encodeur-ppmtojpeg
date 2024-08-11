#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>
#define LINE_WIDTH 16
#define MAX_LINE_LENGTH 1024
#define INITIAL_CAPACITY 100
#include <read_image.h>

ImageInfo lirePremiereLigne(FILE* fichier, int h1 ,int v1,int h2,int v2, int h3, int v3) {
    ImageInfo info ;
    if (fichier == NULL) {
        printf("Erreur : pointeur de fichier invalide.\n");
        return info;
    }
    if (fscanf(fichier, "%2s", info.type) != 1) {
        printf("Erreur de lecture du type de fichier.\n");
        return info;
    }
    strcat(info.header, info.type);
    strcat(info.header, " ");
    char ch;
    do {
        ch = fgetc(fichier);
    } while (ch == '\n' || ch == ' ');
    fseek(fichier, -1, SEEK_CUR);
    if (fscanf(fichier, "%d %d %d", &info.largeur, &info.longueur, &info.maxColor) != 3) {
        printf("Erreur de lecture des dimensions ou de la valeur maximale des pixels.\n");
        return info;
    }
    char dimensions[200];
    sprintf(dimensions, "%d %d %d", info.largeur, info.longueur, info.maxColor);
    strcat(info.header, dimensions);
    if (strcmp(info.type, "P6") == 0){
        info.h1 = h1;
        info.v1 = v1;
        info.h2 = h2;
        info.v2 = v2;
        info.h3 = h3;
        info.v3 = v3;
    }
    else {
        info.h1 = 1;
        info.v1 = 1;
        info.h2 = 1;
        info.v2 = 1;
        info.h3 = 1;
        info.v3 = 1;
    }
    return info;
}

void recupere_MCUS(FILE* fichier, ImageInfo info,MCU* mat, int x, int y) {
    if (fichier == NULL) {
        fprintf(stderr, "Pointeur de fichier ou de matrice MCU invalide\n");
        return;
    }
    int mculargeur = (info.largeur - x < 8 ) ? info.largeur - x : 8;
    int mculongueur = (info.longueur - y < 8) ? info.longueur - y : 8;
    fseek(fichier, 0, SEEK_SET);
    char line[1024];
    if (y == 0 && x == 0) {
        while (fgets(line, sizeof(line), fichier) && strncmp(line, "255", 3) != 0);
    } else {
        while (fgets(line, sizeof(line), fichier) && strncmp(line, "255", 3) != 0);
        fseek(fichier, (info.largeur * y + x) * sizeof(uint8_t), SEEK_CUR);
    }
    int ligne_de_debut = ftell(fichier);
    int i, j;
    for (i = 0; i < mculongueur; i++) {
        fseek(fichier,  ligne_de_debut , SEEK_SET);
        for (j = 0; j < mculargeur; j++) {
            uint8_t byte;
            if (fread(&byte, 1, 1, fichier) == 1) {
               mat->matrice1[i][j] = byte;
            }
        }
        ligne_de_debut += info.largeur;
    }
    for (i = 0; i < mculongueur; i++) {
        for (j = mculargeur; j < 8; j++) {
            mat->matrice1[i][j] = mat->matrice1[i][mculargeur - 1];
        }
    }
    if (mculongueur != 8){
          for (i = mculongueur; i < 8; i++) {
                    memcpy(mat->matrice1[i], mat->matrice1[mculongueur - 1], 32 * sizeof(uint8_t));
                }
    }
}

void recupere_MCUS1(FILE* fichier, ImageInfo info, MCU* mat, int x, int y) {
    if (fichier == NULL || mat == NULL) {
        fprintf(stderr, "Pointeur de fichier ou de matrice MCU invalide\n");
        return;
    }
    unsigned int mcuLargeur = 8 * info.h1;
    unsigned int mcuLongueur = 8 * info.v1;
    unsigned int largeurEffective = (x + mcuLargeur > info.largeur) ? (info.largeur - x) : mcuLargeur;
    unsigned int longueurEffective = (y + mcuLongueur > info.longueur) ? (info.longueur - y) : mcuLongueur;
    fseek(fichier, 0, SEEK_SET);
    char line[1024];
    while (fgets(line, sizeof(line), fichier) && strncmp(line, "255", 3) != 0);
    if (y != 0 || x != 0) {
        fseek(fichier, (info.largeur * y + x) * 3, SEEK_CUR);
    }
    int ligne_de_debut = ftell(fichier);
    unsigned int i, j;
    for (i = 0; i < longueurEffective; i++) {
        fseek(fichier, ligne_de_debut + i * info.largeur * 3, SEEK_SET);
        for (j = 0; j < largeurEffective; j++) {
            uint8_t r, g, b;
            if (fread(&r, 1, 1, fichier) == 1 && fread(&g, 1, 1, fichier) == 1 && fread(&b, 1, 1, fichier) == 1) {
                mat->matrice[i][j].r = r;
                mat->matrice[i][j].g = g;
                mat->matrice[i][j].b = b;
            }
        }
    }
    for (i = 0; i < longueurEffective; i++) {
        for (j = largeurEffective; j < mcuLargeur; j++) {
            mat->matrice[i][j] = mat->matrice[i][largeurEffective - 1];
        }
    }
    for (i = longueurEffective; i < mcuLongueur; i++) {
        for (j = 0; j < mcuLargeur; j++) {
            mat->matrice[i][j] = mat->matrice[longueurEffective - 1][j];
        }
    }
}

/*void matrices(MCU* matrice, Matrix* matrix, int x, int y) {
    // Copie des premières 8x8 blocs dans matrix->matrice1
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            matrix->matrice1[i][j] = matrice->matrice[i][j];
        }
    }
    // Affichage de matrix->matrice1
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            printf("%02x%02x%02x ", matrix->matrice1[i][j].r, matrix->matrice1[i][j].g, matrix->matrice1[i][j].b);
        }
        printf("\n");
    }
	
     printf("\n");
    // Copie des 8x8 blocs suivants dans matrix->matrice2
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            matrix->matrice2[i][j] = matrice->matrice[i + x][j + y];
        }
    }

    // Affichage de matrix->matrice2
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            printf("%02x%02x%02x ", matrix->matrice2[i][j].r, matrix->matrice2[i][j].g, matrix->matrice2[i][j].b);
        }
        printf("\n");
    }
}
 */

/*int main(int argc, char *argv[]) {
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
}*/

