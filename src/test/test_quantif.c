#include <stdio.h>
#include <stdlib.h> 
#include <stdint.h>
#include <include/quantif.h>
#include "../src/quantif.c"
#define N 8



int main() {
    int block1[N][N] = {
        {0, 0, 0, 255, 255, 0, 0, 0},
        {0, 0, 255, 255, 255, 255, 0, 0},
        {0, 255, 255, 255, 255, 255, 255, 0},
        {255, 255, 0, 255, 255, 0, 255, 255},
        {255, 255, 255, 255, 255, 255, 255, 255},
        {0, 0, 255, 0, 0, 255, 0, 0},
        {0, 255, 0, 255, 255, 0, 255, 0},
        {255, 0, 255, 0, 0, 255, 0, 255}
    };

    int block2[N][N] = {
        {12, 34, 56, 78, 90, 123, 45, 67},
        {89, 101, 112, 134, 156, 178, 190, 202},
        {210, 220, 230, 240, 250, 260, 270, 280},
        {290, 300, 310, 320, 330, 340, 350, 360},
        {370, 380, 390, 400, 410, 420, 430, 440},
        {450, 460, 470, 480, 490, 500, 510, 520},
        {530, 540, 550, 560, 570, 580, 590, 600},
        {610, 620, 630, 640, 650, 660, 670, 680}
    };

    int block3[N][N] = {
        {100, 200, 150, 50, 25, 75, 125, 175},
        {175, 125, 75, 25, 50, 150, 200, 100},
        {80, 160, 120, 40, 20, 60, 100, 140},
        {140, 100, 60, 20, 40, 120, 160, 80},
        {30, 60, 90, 120, 150, 180, 210, 240},
        {240, 210, 180, 150, 120, 90, 60, 30},
        {10, 20, 30, 40, 50, 60, 70, 80},
        {80, 70, 60, 50, 40, 30, 20, 10}
    };

    uint16_t quantified_cbcr1[N][N];
    uint16_t quantified_cbcr2[N][N];
    uint16_t quantified_cbcr3[N][N];

    divide_by_quantification(block1, quantification_table_Y, quantified_cbcr1);
    divide_by_quantification(block2, quantification_table_Y, quantified_cbcr2);
    divide_by_quantification(block3, quantification_table_Y, quantified_cbcr3);

    printf("Quantification result for block1:\n");
    print_uint8_matrix(quantified_cbcr1);

    printf("Quantification result for block2:\n");
    print_uint8_matrix(quantified_cbcr2);

    printf("Quantification result for block3:\n");
    print_uint8_matrix(quantified_cbcr3);

    return 0;
}