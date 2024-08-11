#ifndef DCT_H
#define DCT_H

#include <math.h>
#include <stdint.h>

#define N 8

/**
 Cette matrice contient les valeurs de cosinus pré-calculées pour optimiser le calcul de la DCT.
 */
extern const double cosines[N][N];

/**
 Cette fonction prend une matrice de données (8x8) et calcule sa transformation en cosinus discrète (DCT).
 Les valeurs calculées sont stockées dans la matrice de sortie dct.
 */
void calcul_dct(int data[N][N], int dct[N][N]);

/**
 calcule la Transformée en Cosinus Discrète (DCT) d'une matrice d'entrée 8x8, en optimisant le calcul des valeurs cosinus réutilisées. 
 */
void calcul_dct1(int data[N][N], int dct[N][N]);

#endif // DCT_H