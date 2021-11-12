#include <stdio.h>
#include <stdlib.h>
#include <matheval.h>
#include "sistemas.h"
#include "alloc.h"
#include <string.h>
#include <math.h>

#ifndef MAX
#define MAX 100
#endif

int inicializaSistemaNaoLinear(SistemaNaoLinear *sistema, int numFuncoes, int tamFuncoes) {
    if (alocaMatrizChars(numFuncoes, tamFuncoes, &(sistema->funcoes)) != 0) {
        return -1;
    }
    sistema->numFuncoes = numFuncoes;
    sistema->tamFuncoes = tamFuncoes;

    return 0;
}

void finalizaSistemaNaoLinear(SistemaNaoLinear *sistema) {
    freeMatrizChars(&(sistema->funcoes));
}

int geraMatDerivParcial(char ****matDerivParcial, SistemaNaoLinear sistema) {
    void *f, *f_prim;
    char dest[MAX];
    char src[MAX];

    if (alocaMatrizStrings(sistema.numFuncoes, sistema.numFuncoes, matDerivParcial) != 0) {
        return -1;
    }

    for (int i = 0; i < sistema.numFuncoes; i++)
    {
        f = evaluator_create (sistema.funcoes[i]);
        for (int j = 0; j < sistema.numFuncoes; j++){
            sprintf(src,"%d", j+1);                     // transforma inteiro em string
            strcpy(dest, "x");
            strncat(dest, src, sizeof(src));            // concatena "x" e valor de j+1

            f_prim = evaluator_derivative(f, dest);        // deriva em relacao a incognita x(j+1)
            (*matDerivParcial)[i][j] = evaluator_get_string(f_prim);
        }
    }

    return 0;
}


int calculaMatrizJacobiana(char ***matDerivParcial, double ***matJacobiana, int numFuncoes, double *valoresX){
    char **names;
    int count;  
    void *f;

    if (alocaMatrizDoubles(numFuncoes, numFuncoes, matJacobiana) != 0) {
        return -1;
    }

    for (int i = 0; i < numFuncoes; i++){
        for (int j = 0; j < numFuncoes; j++){
            f = evaluator_create(matDerivParcial[i][j]);
            evaluator_get_variables (f, &names, &count);
            (*matJacobiana)[i][j] = evaluator_evaluate(f, count, names, valoresX);
        }
    }
    return 0;
}

int econtraMax(double **mat, int j, int n){
    double max = fabs(mat[j][j]);
    int indiceMax = j;

    for (int i=j+1; i<n; i++) {
        if ( fabs( mat[i][j] ) > max){
            indiceMax = i;
            max = fabs( mat[i][j] );
        }
    }
    return indiceMax;

}

void trocaLinhaMat(double ***mat, double **b, int i, int pivo) {
    double *x = (*mat)[i];
    (*mat)[i] = (*mat)[pivo];
    (*mat)[pivo] = x;

    double y = (*b)[i];
    (*b)[i] = (*b)[pivo];
    (*b)[pivo] = y;
}

void retrossubs(double **mat, double *b, double **x, int n) {
    for (int i = n-1; i >= 0; --i) {
        (*x)[i] = b[i];
        for (int j = i + 1; j < n; ++j) {
            (*x)[i] -= mat[i][j] * (*x)[j];
        }
        (*x)[i] /= mat[i][i];
    }
}

void eliminacaoGaussJordan(double ***mat, double **b, int n) {
    int pivo;

    for (int i = 0; i < n; ++i) {
        pivo = econtraMax((*mat), i, n);
        if (i != pivo) {
            trocaLinhaMat(mat, b, i, pivo);
        }

        for (int j = i+1; j < n; ++j) {
            double m = (*mat)[j][i] / (*mat)[i][i];
            (*mat)[j][i] = 0.0;

            for (int k = i+1; k < n; ++k) {
                (*mat)[j][k] -= (*mat)[i][k] * m;
            }
            (*b)[j] = (*b)[j] - (*b)[i] * m;
        }
    }
}

double resolveSistemaNaoLinear(SistemaNaoLinear sistema, char ****matDerivParcial,
                             double *aproxInicial, double epsilon, int maxIteracoes) {
    double *deltaX, *xAtuais, normaFuncoes, *termosLivres, **matrizJacobiana;
    char **names;
    int count;
    void *f;

    xAtuais = deltaX = aproxInicial;
    termosLivres = malloc(sizeof(double) * sistema.numFuncoes);
    deltaX = malloc(sizeof(double) * sistema.numFuncoes);
    for (int i = 0; i < 1; i++) {

        // Defini norma de funções e termos independentes
        normaFuncoes = 0;
        for (int i = 0; i < sistema.numFuncoes; i++){
            f = evaluator_create(sistema.funcoes[i]);
            evaluator_get_variables (f, &names, &count);
            termosLivres[i] = evaluator_evaluate(f, count, names, xAtuais);

            if (fabs(termosLivres[i]) > normaFuncoes)
                normaFuncoes = fabs(termosLivres[i]);

            termosLivres[i] *= -1;
        }

        // Verifica se funcoes nos pontos estao prox. de epsilon
        if (normaFuncoes < epsilon ) 
            return xAtuais[i];
        
        calculaMatrizJacobiana(*matDerivParcial, &matrizJacobiana, sistema.numFuncoes, xAtuais);
        eliminacaoGaussJordan(&matrizJacobiana, &termosLivres, sistema.numFuncoes);
        retrossubs(matrizJacobiana, termosLivres, &deltaX, sistema.numFuncoes);
        for(int i = 0; i < sistema.numFuncoes; i++) {
            printf("# x%d: %lf\n", i + 1, xAtuais[i] + deltaX[i]);
        }
        // for (int i = 0; i < sistema.tamFuncoes; i++){
        //     xAtual[i] = xAnteriores[i] + deltaX[i]; 
        // }
        // if ( normaDeltaX() < epsilon ) 
        //     return xAtual;
    }

    return xAtuais[0];
}
