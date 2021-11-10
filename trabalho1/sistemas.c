#include <stdio.h>
#include <stdlib.h>
#include "sistemas.h"
#include "alloc.h"

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
    char **nomesVariaveis;
    void *f, *f_prim;
    int count;

    if (alocaMatrizStrings(sistema.numFuncoes, sistema.numFuncoes, matDerivParcial) != 0) {
        return -1;
    }

    for (int i = 0; i < sistema.numFuncoes; i++){
        for (int j = 0; j < sistema.numFuncoes; j++){
            // f = evaluator_create (sistema.funcoes[i]);
            // evaluator_get_variables(f, &nomesVariaveis, &count);

            // f_prim = evaluator_derivative(f, nomesVariaveis[j]);
            // (*matDerivParcial)[i][j] = evaluator_get_string(f_prim);
        }
    }

    return 0;
}

// char **F, double **J, double *xInicial, double epsilon1, double epsilon2, int maxIteracoes, int dimensao ){
void resolveSistemaNaoLinear(SistemaNaoLinear sistema){

    // J = geraMatrizJacobiana()

    // double *deltaX;
    // double *xAtual, *xAnterior = sistema.aproxInicial;
    // if ( normaFuncoesX() < epsilon1 ) 
    //     return xAtual;


    // deltaX = resolveSistemaLinear(J, F, xAtual);
    // for (int i = 0; i < dimensao; i++){
    //     xAtual[i] = xAnterior[i] + deltaX[i]; 
    // }
    // if ( normaDeltaX() < epsilon2 ) 
    //     return xAtual;

}
