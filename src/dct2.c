#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include "zigzag.c"
#include "quantif.c"
#include <dct2.h>



#define N 8

double coefficient(int i) {
    if (i == 0) {
        return 1 /sqrt(2);
    } else {
        return 1.0;
    }
}

// Matrice pré-calculée des valeurs de cosinus
const double cosines[N][N] = {
    {1.000000, 0.980785, 0.923880, 0.831470, 0.707107, 0.555570, 0.382683, 0.195090},
    {1.000000, 0.831470, 0.382683, -0.195090, -0.707107, -0.980785, -0.923880, -0.555570},
    {1.000000, 0.555570, -0.382683, -0.980785, -0.707107, 0.195090, 0.923880, 0.831470},
    {1.000000, 0.195090, -0.923880, -0.555570, 0.707107, 0.831470, -0.382683, -0.980785},
    {1.000000, -0.195090, -0.923880, 0.555570, 0.707107, -0.831470, -0.382683, 0.980785},
    {1.000000, -0.555570, -0.382683, 0.980785, -0.707107, -0.195090, 0.923880, -0.831470},
    {1.000000, -0.831470, 0.382683, 0.195090, -0.707107, 0.980785, -0.923880, 0.555570},
    {1.000000, -0.980785, 0.923880, -0.831470, 0.707107, -0.555570, 0.382683, -0.195090}
};

void calcul_dct(int data[N][N], int dct[N][N]) {
    double c[N];

    for (int i = 0; i < N; i++) {
        c[i] = coefficient(i);
    }

    for (int u = 0; u < N; u++) {
        for (int v = 0; v < N; v++) {
            double sum = 0.0;
            for (int x = 0; x < N; x++) {
                for (int y = 0; y < N; y++) {
                    sum += (data[x][y] - 128) * cosines[x][u] * cosines[y][v];
                }
            }
            dct[u][v] = (int)(2 * sum * c[u] * c[v] / N);
        }
    }
}

void print_decimal_table(uint8_t table[], int size) {
    printf("Matrice en décimal :\n");
    for (int i = 0; i < size; i++) {
        printf("%d ", table[i]);
        if ((i + 1) % N == 0) 
            printf("\n");
    }
}

//Autre version de la dct moins performante ou on cree et calcul la matrice ou on stocke les valeur des cos

double cosinus(int x, int u) {
    double argument = ((2 * x + 1) * u * M_PI) / (16);
    return cos(argument);
}

void calcul_dct1(int data[N][N], int dct[N][N]) {
    double c[N];
    double cosines[N][N];  
    int marq[N][N] = {0}; 

    for (int i = 0; i < N; i++) {
        c[i] = coefficient(i);
    }

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (!marq[i][j]) {
                cosines[i][j] = cosinus(i, j);
                marq[i][j] = 1;

                if (i == 1 && j == 1) { 
                    cosines[2][7] = cosines[3][5] = cosines[1][1];
                    marq[2][7] = marq[3][5] = 1;
                }
                if (i == 1 && j == 2) {
                    cosines[1][4] = cosines[7][6] = cosines[1][2];
                    marq[6][2] = marq[7][6] = 1;
                }
                if (i == 1 && j == 3) {
                    cosines[4][1] = cosines[5][5] = cosines[7][7] = cosines[1][3];
                    marq[4][1] = marq[5][5] = marq[7][7] = 1;
                }
                if (i == 1 && j == 4) {
                    cosines[2][4] = cosines[6][4] = cosines[5][4] = cosines[1][4];
                    marq[2][4] = marq[6][4] = marq[5][4] = 1;
                }
                if (i == 1 && j == 5) {
                    cosines[2][3] = cosines[3][7] = cosines[7][1] = cosines[1][5];
                    marq[2][3] = marq[3][7] = marq[7][1] = 1;
                }
                if (i == 1 && j == 6) {
                    cosines[3][2] = cosines[4][2] = cosines[6][6] = cosines[1][6];
                    marq[3][2] = marq[4][2] = marq[6][6] = 1;
                }
                if (i == 1 && j == 7) {
                    cosines[3][3] = cosines[5][1] = cosines[7][5] = cosines[1][7];
                    marq[3][3] = marq[5][1] = marq[7][5] = 1;
                }
                if (i == 2 && j == 1) {
                    cosines[4][3] = cosines[6][7] = cosines[2][1];
                    marq[4][3] = marq[6][7] = 1;
                }
                if (i == 2 && j == 2) {
                    cosines[3][6] = cosines[4][6] = cosines[5][2] = cosines[2][2];
                    marq[3][6] = marq[4][6] = marq[5][2] = 1;
                }
                if (i == 2 && j == 5) {
                    cosines[6][3] = cosines[3][1] = cosines[2][5];
                    marq[6][3] = marq[3][1] = 1;
                }
                if (i == 2 && j == 6) {
                    cosines[5][6] = cosines[7][2] = cosines[2][6];
                    marq[5][6] = marq[7][2] = 1;
                }
                if (i == 3 && j == 4) {
                    cosines[4][4] = cosines[7][4] = cosines[3][4];
                    marq[4][4] = marq[7][4] = 1;
                }
                if (i == 4 && j == 5) {
                    cosines[5][7] = cosines[6][1] = cosines[7][3] = cosines[4][5];
                    marq[5][7] = marq[6][1] = marq[7][3] = 1;
                }
                if (i == 4 && j == 7) {
                    cosines[5][3] = cosines[6][5] = cosines[4][7];
                    marq[5][3] = marq[6][5] = 1;
                }
            }
        }
    }

    for (int u = 0; u < N; u++) {
        for (int v = 0; v < N; v++) {
            double sum = 0.0;
            for (int x = 0; x < N; x++) {
                for (int y = 0; y < N; y++) {
                    sum += (data[x][y] - 128) * cosines[x][u] * cosines[y][v];
                }
            }
            dct[u][v] = (int)(2 * sum * c[u] * c[v] / N);
        }
    }
}




//version basique de la dct sans optimisation


void calcul_dct3(double data[N][N], double dct[N][N]) {
    double c[N];
    for (int i = 0; i < N; i++) {
        c[i] = coefficient(i);
    }

    for (int u = 0; u < N; u++) {
        for (int v = 0; v < N; v++) {
            double sum = 0.0;
            for (int x = 0; x < N; x++) {
                for (int y = 0; y < N; y++) {
                    sum += (data[x][y]-128) * cos((M_PI / (2 * N)) * (2 * x + 1) * u) * cos((M_PI / (2 * N)) * (2 * y + 1) * v);
                }
            }
            dct[u][v] = 2 * sum * c[u] * c[v] / N;
        }
    }
}