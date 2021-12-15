/*
    Luzia Millena Santos Silva, GRR20185174, 	lmss18@inf.ufpr.br
    Matheus Pacheco Santos,     GRR20192786,	mps19@inf.ufpr.br
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <matheval.h>
#include "sistemas.h"
#include "assert.h"

#ifndef MAX_SIZE_STR
    #define MAX_SIZE_STR 100
#endif

#define MSG_ERRO_ALOCACAO "Ocorreu um erro de alocação!"


/* Le bloco de funções de acordo com a especificação do trabalho e chama função de resolução de SNLs 
 * Imprime erros em stderr, e imprime os blocos de resolução de acordo com os argumentos -o <local de impressao>
   se a não for especificado imprime em stdout
 */
int main(int argc, char *argv[]){
    FILE *saida = (argc > 2 && (strcmp(argv[1],"-o") == 0)) ? fopen(argv[2],"w") : stdout;
    int maxIteracoes, numFuncoes;
    double epsilon, *aproxiIni;
    char input[MAX_SIZE_STR];
    void **funcoes;
    SNL sistema;

    while (scanf("%s", input) != EOF)
    {
        numFuncoes = atoi(input);

        // Alocações
        funcoes = malloc(sizeof(void *) * numFuncoes);
        if (funcoes == NULL) {
            if (saida != stdout) {
                fclose(saida);
            }

            fprintf(stderr, MSG_ERRO_ALOCACAO);
            return -1;
        }
        aproxiIni = malloc(sizeof(double) * numFuncoes); 
        if (aproxiIni == NULL) {
            if (saida != stdout) {
                fclose(saida);

            }
            free(funcoes);

            fprintf(stderr, MSG_ERRO_ALOCACAO);
            return -1;
        }

        fprintf(saida, "%d\n", numFuncoes);

        // Lê as funções
        for (int i = 0; i < numFuncoes; i++) {
            scanf("%s", input);
            funcoes[i] = evaluator_create(input);
            assert(funcoes[i]);
            fprintf(saida, "%s = 0\n", evaluator_get_string(funcoes[i]));
        }
        inicializaSNL(&sistema, &funcoes, numFuncoes);

        // Le as aproximações
        for (int i = 0; i < numFuncoes; i++) {
            scanf("%lf", &aproxiIni[i]);
            getchar(); // Consome caracter espaço
        }

        scanf("%lf", &epsilon);
        scanf("%d", &maxIteracoes);
        scanf("%*c"); // Limpa a linha - consome todos os '\n's

        // Quando o retorno é -1, é erro de alocação. Único erro possível para esta função
        // if (metodoNewtonSNL(sistema, aproxiIni, epsilon, maxIteracoes, saida) == -1) {
        //     if (saida != stdout) {
        //         fclose(saida);
        //     }
        //     finalizaSNL(&sistema);
        //     free(aproxiIni);

        //     fprintf(stderr, MSG_ERRO_ALOCACAO);
        //     return -1;
        // }

        metodoNewtonSNLMatTridiagonal(sistema, aproxiIni, epsilon, maxIteracoes, saida);

        // Limpa memória alocada
        finalizaSNL(&sistema);
        free(aproxiIni);
        fprintf(saida, "\n");
    }

    // Fecha arquivo se ele foi aberto
    if (saida != stdout) {
        fclose(saida);
    }

    return 0;
}
