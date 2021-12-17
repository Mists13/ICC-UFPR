/*
    Luzia Millena Santos Silva, GRR20185174, 	lmss18@inf.ufpr.br
    Matheus Pacheco Santos,     GRR20192786,	mps19@inf.ufpr.br
*/
#include <stdlib.h>
#include "utils.h"

#define PAD(n) (isPot2(n)?(n+1):(n))

int alocaMatDoubles(int lin, int col, double ***mat) {
    int i;
    int padding = PAD(col);
    
    // aloca um vetor de lin ponteiros para linhas
    (*mat) = malloc( lin * sizeof (double*));
    if ((*mat) == NULL) {
        return -1;
    }

    // aloca um vetor com todos os doubles da matriz
    (*mat)[0] = malloc(lin * padding * sizeof(double));
    if ((*mat)[0] == NULL) {
        free((*mat));
        return -1;
    }

    // ajusta os demais ponteiros de linhas (i > 0)
    for (i=1; i < lin; i++) {
        (*mat)[i] = (*mat) [0] + i * padding ;
    }
    return 0;
}

int alocaMatChars(int lin, int col, char ***mat) {
    int i;
    
    int padding = PAD(col);
    // aloca um vetor de lin ponteiros para linhas
    (*mat) = malloc(lin * sizeof (char*));
    if ((*mat) == NULL) {
        return -1;
    }
    /// aloca um vetor com todos os chars da matriz
    (*mat)[0] = malloc(lin * padding * sizeof (char));
    if ((*mat)[0] == NULL) {
        free((*mat));
        return -1;
    }

    // ajusta os demais ponteiros de linhas (i > 0)
    for (i = 1; i < lin; i++)
        (*mat)[i] = (*mat)[0] + i * padding;
    return 0;
}

int alocaMatPonteirosVoids(int lin, int col, void ****mat) {
    int i, j;
    int padding = PAD(col);
    
    // aloca um vetor de lin ponteiros para linhas
    (*mat) = malloc (lin * sizeof (void**));

    // aloca um vetor com todos os voids* da matriz
    (*mat)[0] = malloc (lin * padding * sizeof (void*));

    // ajusta os demais ponteiros de linhas (i > 0)
    for (i = 1; i < lin; i++)
        (*mat)[i] = (*mat)[0] + i * padding;

    // inicialmente os voids* são nulas
    for (i = 0; i < lin; i++){
        for (j = 0; j < col; j++) {
            (*mat)[i][j] = NULL;
        }
    }
    return 0;
}

void freeMatChars(char ***mat) {
    free((*mat)[0]);
}

void freeMatDoubles(double ***mat) {
    free((*mat)[0]);
}

void freeMatPonteirosVoids(void ****mat, int lin, int col) {
    for (int i = 0; i < lin; i++){
        for (int j = 0; j < col; j++){
            free((*mat)[i][j]);
        }
    }

    free((*mat)[0]);
}