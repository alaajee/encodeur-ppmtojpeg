#include <stdlib.h>
#include <stdio.h>
#include <zigzag.h>
#include <stdint.h>
#include <zigzag.h>

#define N 8

int zigzag(int input[N][N], int output[N][N]) {
    int order[N][N];
    int index = 0;
    int x = 0;
    int y = 0;
    int i = 0;

    while (i<N+N-1) {
        if (i % 2 == 1) {
            for (x = (i < N) ? 0 : i - N + 1, y = (i < N) ? i : N - 1; x < N && y >= 0; x++, y--) {
                order[x][y] = index++;
            }
        } else {
            for (x = (i < N) ? i : N - 1, y = (i < N) ? 0 : i - N + 1; x >= 0 && y < N; x--, y++) {
                order[x][y] = index++;
            }
        }
    i++;
    }

    int ordre[N * N];
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            int idx = order[i][j];
            ordre[idx] = input[i][j];
        }
    }

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            output[i][j] = ordre[i * N + j];
        }
    }
    return output[N][N];
}
