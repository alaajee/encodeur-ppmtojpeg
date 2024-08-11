#ifndef QUANTIF_H
#define QUANTIF_H

#include <stdint.h>

#define N 8


/**
 Divise chaque élément d'une matrice par les valeurs correspondantes de la table de quantification.
 Permet de quantifier la matrice
 */
void divide_by_quantification(int input_matrix[N][N], uint8_t qtables[], int output_matrix[N][N]);


#endif // QUANTIF_H
