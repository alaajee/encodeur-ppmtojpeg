#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include "jpeg_write.c"

void RGB_to_YCbCr(int h, int v, MCU* mcu, YCbCr* ycbcr) {
    int index = 0;
    for (int i = 0; i < v; ++i) {
        for (int j = 0; j < h; ++j) {
            int ii = i * 8;
            int jj = j * 8;         
            for (int i = 0; i < 8; ++i) {
                for (int j = 0; j < 8; ++j) {
                    PixelRGB pixel = mcu->matrice[ii + i][jj + j];
                    ycbcr->Y[index ].matrice1[i][j] = round(0.299 * pixel.r + 0.587 * pixel.g + 0.114 * pixel.b);
                    ycbcr->Cb[0].matrice2[ii + i][jj + j] = round(-0.1687 * pixel.r - 0.3313 * pixel.g + 0.5 * pixel.b + 128);
                    ycbcr->Cr[0].matrice2[ii + i][jj + j] = round(0.5 * pixel.r - 0.4187 * pixel.g - 0.0813 * pixel.b + 128);
                }
            }
            index++;
        }
    }
}

void sous_echantillonnage_horizontal(int h , int v , int matrice[16 * 8][16 * 8]) {
    for (int i = 0; i < h * 8; i++) {
        for (int j = 0; j < v * 8; j++) {
            matrice[i][j] = (matrice[i][2 * j] + matrice[i][2 * j + 1]) / 2;
        }
    }
}

void sous_echantillonnage_vertical(int h , int v , int matrice[16 * 8][16 * 8]) {
    for (int i = 0; i < h * 8; i++) {
        for (int j = 0; j < v * 8; j++) {
            matrice[i][j] = (matrice[i*2][ j] + matrice[2*i + 1][ j ]) / 2;
        }
    }
}

void sous_echantillonnage_horizontal_vertical(int h, int v,int h1,int v1, int matrice[8 * 16][8 * 16]) {
    int temp[h1 * 8][v1 * 8];
    int a = h*v;  
    for (int i = 0; i < h1 * 8; i++) {
        for (int j = 0; j < v1 * 8; j++ ) {
        int somme = 0;
           for (int x = 0; x < v; ++x) {
                for (int y = 0; y < h; ++y) {
                    somme += matrice[v * i + x][h * j + y];
                }
            }
            temp[i][j] = somme / a;
        }
    }
    for (int i = 0; i < h1 * 8; i++) {
        for (int j = 0; j < v1 * 8; j++) {
            matrice[i][j] = temp[i][j];
        }
    }
}

void divise_Cb(int h, int v, YCbCr* ycbcr) {
    for (int i = 0; i < v; ++i) {
        for (int j = 0; j < h; ++j) {
            int ii = i * 8;
            int jj = j * 8;
            int index = i  + j * v + 1; 
            for (int i = 0; i < 8; ++i) {
                for (int j = 0; j < 8; ++j) {
                    ycbcr->Cb[index].matrice1[i][j] = ycbcr->Cb[0].matrice2[ii + i][jj + j];
                }
            }
        }
    }
}

void divise_Cr(int h, int v, YCbCr* ycbcr) {
    for (int i = 0; i < v; ++i) {
        for (int j = 0; j < h; ++j) {
            int ii = i * 8;
            int jj = j * 8;
            int index = i  + j * v+ 1;
            for (int i = 0; i < 8; ++i) {
                for (int j = 0; j < 8; ++j) {
                    ycbcr->Cr[index].matrice1[i][j] = ycbcr->Cr[0].matrice2[ii + i][jj + j];
                }
            }
        }
    }   
}

