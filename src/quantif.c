#include <stdio.h>
#include <stdlib.h> 
#include <stdint.h>
#include <qtables.h>
#include <quantif.h>

#define N 8


void divide_by_quantification(int input_matrix[N][N], uint8_t qtables[], int output_matrix[N][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            output_matrix[i][j] = input_matrix[i][j] / qtables[i*N + j]; // Addition for rounding
        }
    }
}

void print_uint8_matrix(uint16_t matrix[N][N]) {
    printf("Matrix quantifiew:\n");
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%04X ", matrix[i][j]);
        }
        printf("\n");
    }
}
