#include <stdlib.h>

int alocaMatrizDoubles(int lin, int col, double ***mat) {
    int i;

    // aloca um vetor de lin ponteiros para linhas
    (*mat) = malloc(lin * sizeof (double*));
    if ((*mat) == NULL) {
        return -1;
    }

    // aloca um vetor com todos os doubles da matriz
    (*mat)[0] = malloc(lin * col * sizeof(double));
    if ((*mat)[0] == NULL) {
        free((*mat));
        return -1;
    }

    // ajusta os demais ponteiros de linhas (i > 0)
    for (i=1; i < lin; i++) {
        (*mat)[i] = (*mat) [0] + i * col ;
    }

    return 0;
}

int alocaMatrizChars(int lin, int col, char ***mat) {
    int i;
    
    // aloca um vetor de lin ponteiros para linhas
    (*mat) = malloc(lin * sizeof (char*));
    if ((*mat) == NULL) {
        return -1;
    }
    
    // aloca um vetor com todos os chars da matriz
    (*mat)[0] = malloc(lin * col * sizeof (char));
    if ((*mat)[0] == NULL) {
        free((*mat));
        return -1;
    }

    // ajusta os demais ponteiros de linhas (i > 0)
    for (i = 1; i < lin; i++)
        (*mat)[i] = (*mat)[0] + i * col;

    return 0;
}

int alocaMatrizStrings(int lin, int col, char ****mat) {
    int i, j;
    
    // aloca um vetor de lin ponteiros para linhas
    (*mat) = malloc (lin * sizeof (char**));
    
    // aloca um vetor com todos as strings da matriz
    (*mat)[0] = malloc (lin * col * sizeof (char*));
    
    // ajusta os demais ponteiros de linhas (i > 0)
    for (i = 1; i < lin; i++)
        (*mat)[i] = (*mat)[0] + i * col;

    // inicialmente as strings são nulas
    for (i = 0; i < lin; i++)
    {
        for (j = 0; j < col; j++) {
            (*mat)[i][j] = NULL;
        }
    }

    return 0;
}

void freeMatrizChars(char ***mat) {
    free((*mat)[0]);
    free((*mat));
}

void freeMatrizDoubles(double ***mat) {
    free((*mat)[0]);
    free((*mat));
}

void freeMatrizStrings(char ****mat, int lin, int col) {
    for (int i = 0; i < lin; i++){
        for (int j = 0; j < col; j++){
            free((*mat)[i][j]);
        }
    }

    free((*mat)[0]);
    free((*mat));
}