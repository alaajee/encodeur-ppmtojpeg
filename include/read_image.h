#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>
#define LINE_WIDTH 16 
#define MAX_LINE_LENGTH 1024
#define INITIAL_CAPACITY 100

// Structure pour recuperer les données de l'image
typedef struct {
    char type[3];
    unsigned int longueur; 
    unsigned int largeur;
    unsigned int maxColor; 
    char header[100];
    unsigned int nbr_MCUS;
    int h1;
    int v1;
    int h2;
    int v2;
    int h3;
    int v3;
    FILE* fichier;
} ImageInfo;

// Structure pour recuperer les pixels rgb 
typedef struct {
    uint8_t r, g, b;
} PixelRGB;

// Structure pour diviser recuperer les mcus , deux cas se présentent , le PGM donc matrice1 et PPM donc matrice
typedef struct {
      PixelRGB matrice[320][320];
      int matrice1[8][8];
  
} MCU;

// Structure pour diviser les mcus en des blocs de 8x8
typedef struct {
	int matrice1[8][8];
	int matrice2[8 * 16][8 * 16];
} Matrix;

// Struture des vecteurs de matrices pour RGB
typedef struct {
    Matrix Y[6];
    Matrix Cb[6];
    Matrix Cr[6];
} YCbCr;

// Fonction pour lire les données de l'image 
ImageInfo lirePremiereLigne(FILE* fichier , int h1 ,int v1, int h2 , int v2 , int h3, int v3);

// Fonction pour recuperer les MCUS dans le cas de gris 
void recupere_MCUS(FILE* fichier, ImageInfo info,MCU* mat, int x, int y);

// Fonction pour recuperer les MCUS dans le cas de rgb
void recupere_MCUS1(FILE* fichier, ImageInfo info, MCU* mat, int x, int y);
