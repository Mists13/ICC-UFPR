#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <matheval.h>
#include "utils.h"
#include "sistemas.h"
#include "alloc.h"

#define MAX 50

int main() {
    char input[MAX], ***matDerivParcial;
    double epsilon, *aproxiIni;
    SistemaNaoLinear sistema;
    int i, maxIteracoes;

    while (scanf("%s", input) != EOF){ 
        inicializaSistemaNaoLinear(&sistema, atoi(input), MAX); // Aloca espaço para o sistema linear de acordo com o número de funções lido no scopo do while
        aproxiIni = malloc(sizeof(double) * sistema.numFuncoes); // Aloca espaço para o vetor de aproximáções iniciais
        printf("%d\n", sistema.numFuncoes);

        // Lê as funções
        for (i = 0; i < sistema.numFuncoes; i++) {
            scanf("%s", sistema.funcoes[i]);
            printf("%s = 0\n", sistema.funcoes[i]);
        }

        // Le as aproximações
        for (i = 0; i < sistema.numFuncoes; i++) {
            scanf("%lf", &aproxiIni[i]);
            getchar(); // Consome caracter espaço
            printf("x%d = %lf\n#\n", i + 1, aproxiIni[i]);
        }

        scanf("%lf", &epsilon);
        scanf("%d", &maxIteracoes);
        scanf("%*c"); // Limpa a linha - consome todos os '\n's

        geraMatDerivParcial(&matDerivParcial, sistema);

        // Limpa memória alocada
        freeMatrizStrings(&matDerivParcial, sistema.numFuncoes, sistema.numFuncoes);
        finalizaSistemaNaoLinear(&sistema);
        free(aproxiIni);

        printf("\n");
    }

    return 33;
}
