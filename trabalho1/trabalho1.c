#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"


// double derivadaParcial(char *funcao, double x, char incognita){
        // evaluator_derivative()
// }

// Matriz de constantes: Calcula derivadas parciais com base no vetor de funcoes
// double **geraMatrizJacobiana(double **mat, char **functions, int dimensao, double xAtual){

//     // percorre o vetor de funcoes
//     for (int i=0; i < dimensao; i++){
//         // percorre o vetor de incognitas
//         for (int j=0; j < dimensao; j++){
//             mat[i][j] = derivadaParcial(functions[i], xAtual, j+1);      
//     }

// }
//     }

// A cada iteração do metodo de newton pra resolver o sistema nao linear, 
// é preciso resolver um sistema linear internamente

// double metodoNewton(char **F, double **J, double *xInicial, double epsilon1, double epsilon2, int maxIteracoes, int dimensao ){

//     double *deltaX;

//     double *xAtual, *xAnterior = xInicial;
//     if ( normaFuncoesX() < epsilon1 ) 
//         return xAtual;

//     deltaX = resolveSistemaLinear(J, F, xAtual);
//     for (int i = 0; i < dimensao; i++){
//         xAtual[i] = xAnterior[i] + deltaX[i]; 
//     }
//     if ( normaDeltaX() < epsilon2 ) 
//         return xAtual;
// }

#define MAX_NUM_FUNCTION 20

int main() {

    int dimensao, maxIteracoes, i = 0;
    // dimensao = maxIteracoes = i = 0;
    char **functions;
    double **mat, epsilon, *aproxInicial;
    
    // FILE *arq;
    // char Linha[100];
    // char *result;

    // Abre um arquivo TEXTO para LEITURA
    // arq = fopen("sistemas.dat", "rt");
    // if (arq == NULL){  // Se houve erro na abertura
    //     printf("Problemas na abertura do arquivo\n");
    //     return 1;
    // }
    // i = 1;
    // while (!feof(arq)) {
    //     // Lê uma linha (inclusive com o '\n')
    //     result = fgets(Linha, 100, arq);  // o 'fgets' lê até 99 caracteres ou até o '\n'
    //     if (result)  // Se foi possível ler
    //     printf("Linha %d : %s",i, Linha);
    //     i++;
    // }
    // fclose(arq);    

    functions = alocaMatrizChars(dimensao, MAX_NUM_FUNCTION);
    mat = alocaMatrizDoubles(dimensao, dimensao);

    char resultado;      
    while (scanf("%c", &resultado) != EOF){ // Lê a dimensão da matriz

        dimensao = resultado - '0';

        printf("DIMENSAO: %d\n", dimensao);
        // Le as funções
        for (i = 0; i < dimensao; i++) {
            scanf("%s", functions[i]);
            printf("%s\n", functions[i]);
        }

        // Le as aproximações
        for (i = 0; i < dimensao; i++) {
            scanf("%lf", &aproxInicial[i]);
            getchar(); 
            printf("%lf\n", aproxInicial[i]);
        }

        // Lê a epsilon e maximo de iteracoes
        scanf("%lf", &epsilon);
        scanf("%d", &maxIteracoes);

        printf("epsilon: %lf, maxItercaoes: %d\n", epsilon, maxIteracoes);
        getchar(); 
    }


    // geraMatrizJacobiana(mat, functions, dimensao);  
    return 33;
}
