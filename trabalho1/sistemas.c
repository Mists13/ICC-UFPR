#include <stdio.h>
#include <stdlib.h>
#include <matheval.h>
#include <assert.h>
#include <string.h>
#include <math.h>
#include "sistemas.h"
#include "alloc.h"
#include "utils.h"

#ifndef MAX
#define MAX 100
#endif

int inicializaSistemaNaoLinear(SistemaNaoLinear *sistema, void ***funcoes, int numFuncoes) {
    sistema->funcoes = (*funcoes);
    sistema->numFuncoes = numFuncoes;
    return 0;
}

void finalizaSistemaNaoLinear(SistemaNaoLinear *sistema) {
    free(sistema->funcoes);
}

int geraMatDerivParcial(void ****mat, SistemaNaoLinear sistema) {
    char var[MAX];

    if (alocaMatrizPonteirosVoids(sistema.numFuncoes, sistema.numFuncoes, mat) != 0) {
        return -1;
    }

    for (int i = 0; i < sistema.numFuncoes; i++)
    {
        for (int j = 0; j < sistema.numFuncoes; j++) {
            sprintf(var,"x%d", j+1); // transforma inteiro em string 
            (*mat)[i][j] =evaluator_derivative(sistema.funcoes[i], var);
        }
    }

    return 0;
}


int calculaMatrizJacobiana(void ***matDerivParcial, double ***matJacobiana, int numFuncoes, double *valoresX) {
    char **names;
    int count;  

    if (alocaMatrizDoubles(numFuncoes, numFuncoes, matJacobiana) != 0) {
        return -1;
    }

    for (int i = 0; i < numFuncoes; i++){
        for (int j = 0; j < numFuncoes; j++){
            evaluator_get_variables (matDerivParcial[i][j], &names, &count);
            (*matJacobiana)[i][j] = evaluator_evaluate(matDerivParcial[i][j], count, names, valoresX);
        }
    }
    return 0;
}

int econtraMax(double **mat, int j, int n) {
    double max = fabs(mat[j][j]);
    int indiceMax = j;

    for (int i = j+1; i < n; i++) {
        if (fabs(mat[i][j]) > max){
            indiceMax = i;
            max = fabs(mat[i][j]);
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

double resolveSistemaNaoLinear(SistemaNaoLinear sistema, void ***matDerivParcial,
                             double *aproxInicial, double epsilon, int maxIteracoes) {
    double *deltaX, *xAtuais, normaFuncoes, *termosLivres, **matrizJacobiana;
    char **names;
    int count;

    xAtuais = deltaX = aproxInicial;
    termosLivres = malloc(sizeof(double) * sistema.numFuncoes);
    deltaX = malloc(sizeof(double) * sistema.numFuncoes);
    for (int i = 0; i < 1; i++) {

        // Defini norma de funções e termos independentes
        normaFuncoes = 0;
        for (int i = 0; i < sistema.numFuncoes; i++){
            evaluator_get_variables(sistema.funcoes[i], &names, &count);
            termosLivres[i] = evaluator_evaluate(sistema.funcoes[i], count, names, xAtuais);

            if (fabs(termosLivres[i]) > normaFuncoes)
                normaFuncoes = fabs(termosLivres[i]);

            termosLivres[i] *= -1;
        }

        // Verifica se funcoes nos pontos estao prox. de epsilon
        if (normaFuncoes < epsilon ) 
            return xAtuais[i];
        
        calculaMatrizJacobiana(matDerivParcial, &matrizJacobiana, sistema.numFuncoes, xAtuais);
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

    return 0.0;
}
