#include <stdio.h>
#include <stdlib.h>
#include <matheval.h>
#include "sistemas.h"
#include "alloc.h"
#include <string.h>
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
    
    for (int i = 0; i < sistema.numFuncoes; i++){
        f = evaluator_create (sistema.funcoes[i]);
        for (int j = 0; j < sistema.numFuncoes; j++){
            sprintf(src,"%d", j+1);                     // transforma inteiro em string
            strcpy(dest, "x");
            strncat(dest, src, sizeof(src));            // concatena "x" e valor de j+1

            f_prim = evaluator_derivative(f, dest);     // deriva em relacao a incognita x(j+1)
            (*matDerivParcial)[i][j] = evaluator_get_string(f_prim);
            printf("f(x):%s ,f'(x): %s\n", sistema.funcoes[i], (*matDerivParcial)[i][j]);
        }
        printf("\n");         
    }

    return 0;
}


double normaFuncoesNoPonto(SistemaNaoLinear sistema, double *vetorXAtual){

    void *f;
    char **names;
    int count;
    double maiorValorFuncoes = 0 ;
    double valorFuncao = 0;

    // calcula valor para a primeira funcao
    f = evaluator_create(sistema.funcoes[0]);
    // printf (" At normaFuncoesNoPonto = %s\n", evaluator_get_string (f));

    evaluator_get_variables(f, &names, &count);
    maiorValorFuncoes = evaluator_evaluate(f, count, names, vetorXAtual);        
    printf("Valor na funcao %lf\n", maiorValorFuncoes);

    // Aplica os valores de x nas funcoes
    for ( int i = 1; i < sistema.numFuncoes; i++){

        f = evaluator_create(sistema.funcoes[i]);
        evaluator_get_variables (f, &names, &count);
        valorFuncao = evaluator_evaluate(f, count, names, vetorXAtual);

        if (valorFuncao > maiorValorFuncoes)
            maiorValorFuncoes = valorFuncao;
    }
    return maiorValorFuncoes;

}

double resolveSistemaNaoLinear(SistemaNaoLinear sistema, char ****matDerivParcial,
                             double *aproxInicial, double epsilon, int maxIteracoes){

    char ****matrizJacobiana;
    double *deltaX;
    double *xAtuais = aproxInicial;
    double maiorValorFuncoes;
    void *f;
    
    for (int i = 0; i < maxIteracoes; i++){
        
        maiorValorFuncoes = normaFuncoesNoPonto(sistema, xAtuais);
        // verifica se funcoes nos pontos estao prox. de epsilon
        if ( maiorValorFuncoes < epsilon ) 
            return xAtuais[i];
        printf("Maior valor funcoes: %lf", maiorValorFuncoes);

        // calculaMatrizJacobiana(&matrizJacobiana);

        // deltaX = resolveSistemaLinear(matrizJacobiana, sistema.funcoes, xAtuais);
        // for (int i = 0; i < sistema.tamFuncoes; i++){
        //     xAtual[i] = xAnteriores[i] + deltaX[i]; 
        // }
        // if ( normaDeltaX() < epsilon ) 
        //     return xAtual;
    }

    return xAtuais[0];
}
