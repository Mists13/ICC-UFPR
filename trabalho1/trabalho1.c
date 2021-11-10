#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "sistemas.h"
#include "alloc.h"
#include <matheval.h>

#define MAX 500

void geraMatDerivParcial(SistemaNaoLinear *sistema){
    char **nomesVariaveis;
    int count;
    void *f, *f_prim;

    (*sistema).matDerivParcial = alocaMatrizStrings((*sistema).dimensao, (*sistema).dimensao);

    for (int i = 0; i < (*sistema).dimensao; i++){
        for (int j = 0; j < (*sistema).dimensao; j++){
            // f = evaluator_create ((*sistema).funcoes[i]);
            // evaluator_get_variables(f, &nomesVariaveis, &count);

            // f_prim = evaluator_derivative(f, nomesVariaveis[j]);
            // (*sistema).matDerivParcial[i][j] = evaluator_get_string(f_prim);
        }
    }
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


int main() {
    SistemaNaoLinear sistema;
    char input[MAX];
    int i;

    inicializaSistemaNaoLinear(&sistema);
    while (scanf("%s", input) != EOF){ 
        // A dimensão é lida no scanf dentro do scopo do while
        sistema.dimensao = atoi(input);
        printf("%d\n", sistema.dimensao);
        
        sistema.funcoes = alocaMatrizChars(sistema.dimensao, MAX);
        sistema.aproxInicial = malloc(sizeof(double) * sistema.dimensao);

        // Le as funções
        for (i = 0; i < sistema.dimensao; i++) {
            scanf("%s", sistema.funcoes[i]);
            printf("%s = 0\n", sistema.funcoes[i]);
        }

        // Le as aproximações
        for (i = 0; i < sistema.dimensao; i++) {
            scanf("%lf", &sistema.aproxInicial[i]);
            getchar(); // Consome caracter espaço
            printf("x%d = %lf\n#\n", i + 1, sistema.aproxInicial[i]);
        }

        // Lê epsilon e maximo de iteracoes
        scanf("%lf", &sistema.epsilon);
        scanf("%d", &sistema.maxIteracoes);

        // Limpa a linha - consome todos os '\n's
        scanf("%*c");

        geraMatDerivParcial(&sistema);
        finalizaSistemaNaoLinear(&sistema);
        printf("\n");
    }
    return 33;
}
