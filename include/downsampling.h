#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include "../include/read_image.h"

// Fonction pour la conversion RGB vers YCbCr
void RGB_to_YCbCr(int h, int v, MCU* mcu, YCbCr* ycbcr);

// Fonction pour le sous_echantillonnage horizontal 
void sous_echantillonnage_horizontal(int h , int v , int matrice[16 * 8][16 * 8]);

// Fonction pour le sous_echantillonnage vertical
void sous_echantillonnage_vertical(int h , int v , int matrice[16 * 8][16 * 8]);


// Fonction pour le sous_echantillonnage vertical et horizontal combiné
void sous_echantillonnage_horizontal_vertical(int h, int v,int h1,int v1, int matrice[8 * 16][8 * 16]);

// Je recupere les echantillons apres le downsampling et je les divise en des blocs 8x8 pour Cb
void divise_Cb(int h, int v, YCbCr* ycbcr);


// Je recupere les echantillons apres le downsampling et je les divise en des blocs 8x8 , mais pour Cr
void divise_Cr(int h, int v, YCbCr* ycbcr);
