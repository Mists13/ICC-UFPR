#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <matheval.h>
#include "utils.h"
#include "sistemas.h"
#include "alloc.h"
#include "assert.h"

#define MAX 50

int main(int argc, char *argv[]){
    FILE *arq;
    FILE *saida = stdin;

    if (argc > 2 && strcmp(argv[1],"-o") == 0){
        arq = fopen(argv[2],"w");
        fprintf(arq, "teste");
        saida = arq;
    }

    double epsilon, *aproxiIni, tempo;
    void **funcoes, ***matDerivadas;
    int i, maxIteracoes, numFuncoes;
    SistemaNaoLinear sistema;
    char input[MAX];

    while (scanf("%s", input) != EOF){
        numFuncoes = atoi(input);
        funcoes = malloc(sizeof(void *) * numFuncoes);
        aproxiIni = malloc(sizeof(double) * numFuncoes); 
        
        printf("%d\n", numFuncoes);

        // Lê as funções
        for (i = 0; i < numFuncoes; i++) {
            scanf("%s", input);
            funcoes[i] = evaluator_create(input);
            assert(funcoes[i]);
            printf("%s = 0\n", evaluator_get_string(funcoes[i]));
        }
        inicializaSistemaNaoLinear(&sistema, &funcoes, numFuncoes);

        // Le as aproximações
        for (i = 0; i < numFuncoes; i++)
        {
            scanf("%lf", &aproxiIni[i]);
            getchar(); // Consome caracter espaço
        }

        scanf("%lf", &epsilon);
        scanf("%d", &maxIteracoes);
        scanf("%*c"); // Limpa a linha - consome todos os '\n's

        tempo = timestamp();  
        geraMatDerivParcial(&matDerivadas, sistema);
        tempo = timestamp() - tempo;
        printf("# Tempo de derivadas: %lf\n", tempo);

        metodoNewtonSistemaNaoLinear(sistema, matDerivadas, aproxiIni, epsilon, maxIteracoes);

        // Limpa memória alocada
        finalizaSistemaNaoLinear(&sistema);
        free(aproxiIni);

        printf("\n");
    }

    return 33;
}
