#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "sistemas.h"
#include <matheval.h>

#define MAX 50

// double derivadaParcial(char *funcao, double x, char incognita){
        // evaluator_derivative()
// }

// Matriz de constantes: Calcula derivadas parciais com base no vetor de funcoes
void geraMatrizJacobiana(SistemaNaoLinear sistema){
    char **nomesVariaveis;
    int count;
    void *f, *f_prim;

    sistema.matJacobiana = malloc(MAX * MAX * MAX * MAX);

    // percorre o vetor de funcoes
    for (int i=0; i < sistema.dimensao; i++){
        // percorre o vetor de incognitas
        for (int j=0; j < sistema.dimensao; j++){
            f = evaluator_create (sistema.funcoes[i]);
            evaluator_get_variables(f, &nomesVariaveis, &count);

            f_prim = evaluator_derivative(f, nomesVariaveis[j]);
            sistema.matJacobiana[i][j] = evaluator_get_string(f_prim);

            printf("%s ", sistema.matJacobiana[i][j]);
        }
        printf("\n"); 
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

    sistema.funcoes = alocaMatrizChars(MAX, MAX);
    sistema.aproxInicial = malloc(sizeof(double) * MAX);
    
    while (scanf("%s", input) != EOF){ 

        // A dimensão é lida no scanf dentro do scopo do while
        sistema.dimensao = atoi(input);

        printf("%d\n", sistema.dimensao);
        // Le as funções
        for (i = 0; i < sistema.dimensao; i++) {
            scanf("%s", sistema.funcoes[i]);
            printf("%s = 0\n", sistema.funcoes[i]);
        }

        // Le as aproximações
        for (i = 0; i < sistema.dimensao; i++) {
            scanf("%lf", &sistema.aproxInicial[i]);
            getchar(); 
            printf("x%d = %lf\n#\n", i + 1, sistema.aproxInicial[i]);
        }

        // Lê epsilon e maximo de iteracoes
        scanf("%lf", &sistema.epsilon);
        scanf("%d", &sistema.maxIteracoes);

        getchar(); 
        getchar();
        printf("\n\n");
        geraMatrizJacobiana(sistema);  
    }

    return 33;
}
