#include <stdlib.h>

double **alocaMatrizDoubles(int lin, int col) {
    double **mat;
    int i;

    // aloca um vetor de LIN ponteiros para linhas
    mat = malloc (lin * sizeof (double*)) ;
    
    // aloca um vetor com todos os elementos da matriz
    mat[0] = calloc (lin * col, sizeof (double));
    
    // ajusta os demais ponteiros de linhas (i > 0)
    for (i=1; i < lin; i++)
        mat[i] = mat[0] + i * col ;

    return mat;
}

char **alocaMatrizChars(int lin, int col) {
    char **mat;
    int i;
    
    // aloca um vetor de LIN ponteiros para linhas
    mat = malloc (lin * sizeof (char*));
    
    // aloca um vetor com todos os elementos da matriz
    mat[0] = malloc (lin * col * sizeof (char));
    
    // ajusta os demais ponteiros de linhas (i > 0)
    for (i=1; i < lin; i++)
        mat[i] = mat[0] + i * col;

    return mat;
}

char ***alocaMatrizStrings(int lin, int col) {
    char ***mat;
    int i, j;
    
    // aloca um vetor de LIN ponteiros para linhas
    mat = malloc (lin * sizeof (char**));
    
    // aloca um vetor com todos os elementos da matriz
    mat[0] = malloc (lin * col * sizeof (char*));
    
    // ajusta os demais ponteiros de linhas (i > 0)
    for (i = 1; i < lin; i++)
        mat[i] = mat[0] + i * col;

    // inicialmente as strings devem apontar para null
    for (i = 0; i < lin; i++)
    {
        for (j = 0; j < col; j++) {
            mat[i][j] = NULL;
        }
    }

    return mat;
}