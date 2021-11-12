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

void inicializaMatQuad(MatrizQuadrada *mat, double ***coeficientes, double **termosLivres) {

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
            (*mat)[i][j] = evaluator_derivative(sistema.funcoes[i], var);
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

int econtraMaxPivo(double **mat, int j, int n) {
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

void trocaLinhaMat(MatrizQuadrada *mat, int i, int pivo) {
    double *x = mat->coeficientes[i];
    mat->coeficientes[i] = mat->coeficientes[pivo];
    mat->coeficientes[pivo] = x;

    double y = mat->termosLivres[i];
    mat->termosLivres[i] = mat->termosLivres[pivo];
    mat->termosLivres[pivo] = y;
}

void retrossubs(MatrizQuadrada *mat, double **x) {
    for (int i = mat->dimensao-1; i >= 0; --i) {
        (*x)[i] = mat->termosLivres[i];
        for (int j = i + 1; j < mat->dimensao; ++j) {
            (*x)[i] -= mat->coeficientes[i][j] * (*x)[j];
        }
        (*x)[i] /= mat->coeficientes[i][i];
    }
}

void eliminacaoGaussJordan(MatrizQuadrada *mat) {
    int pivo;

    for (int i = 0; i < mat->dimensao; ++i) {
        pivo = econtraMaxPivo(mat->coeficientes, i, mat->dimensao);
        if (i != pivo) {
            trocaLinhaMat(mat, i, pivo);
        }

        for (int j = i+1; j < mat->dimensao; ++j) {
            double m = mat->coeficientes[j][i] / mat->coeficientes[i][i];
            mat->coeficientes[j][i] = 0.0;

            for (int k = i+1; k < mat->dimensao; ++k) {
                mat->coeficientes[j][k] -= mat->coeficientes[i][k] * m;
            }
            (mat->termosLivres)[j] = (mat->termosLivres)[j] - (mat->termosLivres)[i] * m;
        }
    }
}

int metodoNewtonSistemaNaoLinear(SistemaNaoLinear sistema, void ***matDerivParcial,
        double *aproxInicial, double epsilon, int maxIteracoes) {
    double *deltaX, *xAtuais, *xProx, normaFuncoes, *termosLivres, **matrizJacobiana;
    char **names;
    int count;
    MatrizQuadrada *matrizQuadrada;

    termosLivres = malloc(sizeof(double) * sistema.numFuncoes);
    if (termosLivres == NULL) {
        return -1;
    }
    deltaX = malloc(sizeof(double) * sistema.numFuncoes);
    if (deltaX == NULL) {
        free(termosLivres);
        return -1;
    }
    
    xAtuais = aproxInicial; 
    (*deltaX) = (*aproxInicial);
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
        matrizQuadrada->coeficientes = matrizJacobiana;
        matrizQuadrada->dimensao = sistema.numFuncoes;
        matrizQuadrada->termosLivres = termosLivres;
        eliminacaoGaussJordan(matrizQuadrada);
        retrossubs(matrizQuadrada,  &deltaX);

        // Encontra proximas aprximacoes ( X[i+1])
        // for (int i = 0; i < sistema.numFuncoes; i++){
        //     xProx[i] = xAtuais[i] + deltaX[i]; 
        // }

        // maxDeltaX = fabs(deltaX[0]);
        // // Calcula o maior valor de Delta obtido na iteracao
        // for (int l = 1; l < sistema.numFuncoes; l++){
        //     if (maxDeltaX > fabs(deltaX[l])){
        //         maxDeltaX = fabs(deltaX[l]);
        //     }
        // }
        // if ( maxDeltaX < epsilon ) 
        //     return xProx;
    }

    return 0;
}
