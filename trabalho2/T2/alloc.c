/*
    Luzia Millena Santos Silva, GRR20185174, 	lmss18@inf.ufpr.br
    Matheus Pacheco Santos,     GRR20192786,	mps19@inf.ufpr.br
*/
#include <stdlib.h>
#include "utils.h"

#define PAD(n) (isPot2(n)?(n+1):(n))

int alocaMatDoubles(int lin, int col, double ***mat) {
    int i;

    // aloca um vetor com os ponteiros e os elementos da matriz
    (*mat) = malloc (lin * sizeof (double*) + lin * PAD(col) * sizeof (double));
    if ((*mat) == NULL) {
        return -1;
    }

    // ajusta o ponteiro da primeira linha
    (*mat)[0] = (double*)((*mat) + lin);
    
    // ajusta os ponteiros das demais linhas (i > 0)
    for (i = 1; i < lin; i++)
    (*mat)[i] = (*mat)[0] + (i * PAD(col) ) ;
    
    return 0;
}

int alocaMatChars(int lin, int col, char ***mat) {
    int i;
    
    // aloca um vetor com os ponteiros e os elementos da matriz
    (*mat) = malloc (lin * sizeof (char*) + lin * PAD(col) * sizeof (char));
    if((*mat) == NULL) {
        return -1;
    }
    
    // ajusta o ponteiro da primeira linha
    (*mat)[0] = (char*)((*mat) + lin);
    
    // ajusta os ponteiros das demais linhas (i > 0)
    for (i=1; i < lin; i++)
    (*mat)[i] = (*mat)[0] + (i * PAD(col) ) ;
    
    return 0;
}

int alocaMatPonteirosVoids(int lin, int col, void ****mat) {
    int i;
    
    // aloca um vetor com os ponteiros e os elementos da matriz
    (*mat) = malloc (lin * sizeof (void**) + lin * PAD(col) * sizeof (void*));
    
    // ajusta o ponteiro da primeira linha
    (*mat)[0] = (void**)((*mat) + lin);
    
    // ajusta os ponteiros das demais linhas (i > 0)
    for (i = 1; i < lin; i++)
    (*mat)[i] = (*mat)[0] + (i * PAD(col) ) ;
    
    return 0;
}

void freeMatChars(char ***mat) {
    free((*mat));
}

void freeMatDoubles(double ***mat) {
    free((*mat));
}

void freeMatPonteirosVoids(void ****mat, int lin, int col) {
    for (int i = 0; i < lin; i++){
        for (int j = 0; j < col; j++){
            free((*mat)[i][j]);
        }
    }

    free((*mat));
}