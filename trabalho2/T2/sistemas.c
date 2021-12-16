/*
    Luzia Millena Santos Silva, GRR20185174, 	lmss18@inf.ufpr.br
    Matheus Pacheco Santos,     GRR20192786,	mps19@inf.ufpr.br
*/
#include <stdio.h>
#include <stdlib.h>
#include <matheval.h>
#include <math.h>
#include "sistemas.h"
#include "alloc.h"
#include "utils.h"
#include <likwid.h>

#define PAD(n) (isPot2(n)?(n+1):(n))

#define SUPERIOR 0
#define PRINCIPAL 1
#define INFERIOR 2


#ifndef MAX_SIZE_STR
#define MAX_SIZE_STR 100
#endif

#ifndef INDICE_TOTAL
#define INDICE_TOTAL 0
#endif

#ifndef INDICE_DERIVADAS
#define INDICE_DERIVADAS 1
#endif

#ifndef INDICE_JACOBIANA
    #define INDICE_JACOBIANA 2
#endif

#ifndef INDICE_SL
    #define INDICE_SL 3
#endif


void inicializaSNL(SNL *sistema, void ***funcoes, int numFuncoes) {
    sistema->funcoes = (*funcoes);
    sistema->numFuncoes = numFuncoes;
}

void finalizaSNL(SNL *sistema) {
    for (int i = 0; i < sistema->numFuncoes; i++) {
        evaluator_destroy(sistema->funcoes[i]);
    }
    free(sistema->funcoes);
}

void geraVars(char ***vars, int dimensao) {
    for (int i = 0; i <  dimensao; i++) {
        sprintf((*vars)[i] , "x%d", i + 1);
    }
}

void printaTemposMetodoNewtonSNL(FILE *saida, double tempos[4]) {
    fprintf(saida, "###########\n");
    fprintf(saida, "# Tempo Total: %lf\n", tempos[INDICE_TOTAL]);
    fprintf(saida, "# Tempo Derivada: %lf\n", tempos[INDICE_DERIVADAS]);
    fprintf(saida, "# Tempo Jacobiana: %lf\n", tempos[INDICE_JACOBIANA]);
    fprintf(saida, "# Tempo SL: %lf\n", tempos[INDICE_SL]);
    fprintf(saida, "###########\n");
}

void calculaMatJacobianaTriadigonal(void ***tridiagDeriv, double ***tridiagJacobiana, char **vars, int numFuncoes, double *valoresX) {
    int i = 0;
    int limit = (numFuncoes- 1) - (numFuncoes - 1) % 4; 
    for (i = 0; i < limit; i += 4) {
        (*tridiagJacobiana)[SUPERIOR][i] = evaluator_evaluate(tridiagDeriv[SUPERIOR][i], numFuncoes, vars, valoresX);
        (*tridiagJacobiana)[PRINCIPAL][i] = evaluator_evaluate(tridiagDeriv[PRINCIPAL][i], numFuncoes, vars, valoresX);
        (*tridiagJacobiana)[INFERIOR][i] = evaluator_evaluate(tridiagDeriv[INFERIOR][i], numFuncoes, vars, valoresX);
        
        (*tridiagJacobiana)[SUPERIOR][i+1] = evaluator_evaluate(tridiagDeriv[SUPERIOR][i+1], numFuncoes, vars, valoresX);
        (*tridiagJacobiana)[PRINCIPAL][i+1] = evaluator_evaluate(tridiagDeriv[PRINCIPAL][i+1], numFuncoes, vars, valoresX);
        (*tridiagJacobiana)[INFERIOR][i+1] = evaluator_evaluate(tridiagDeriv[INFERIOR][i+1], numFuncoes, vars, valoresX);

        (*tridiagJacobiana)[SUPERIOR][i+2] = evaluator_evaluate(tridiagDeriv[SUPERIOR][i+2], numFuncoes, vars, valoresX);
        (*tridiagJacobiana)[PRINCIPAL][i+2] = evaluator_evaluate(tridiagDeriv[PRINCIPAL][i+2], numFuncoes, vars, valoresX);
        (*tridiagJacobiana)[INFERIOR][i+2] = evaluator_evaluate(tridiagDeriv[INFERIOR][i+2], numFuncoes, vars, valoresX);

        (*tridiagJacobiana)[SUPERIOR][i+3] = evaluator_evaluate(tridiagDeriv[SUPERIOR][i+3], numFuncoes, vars, valoresX);
        (*tridiagJacobiana)[PRINCIPAL][i+3] = evaluator_evaluate(tridiagDeriv[PRINCIPAL][i+3], numFuncoes, vars, valoresX);
        (*tridiagJacobiana)[INFERIOR][i+3] = evaluator_evaluate(tridiagDeriv[INFERIOR][i+3], numFuncoes, vars, valoresX);
    }

    // Resíduos
    for (i = limit; i < numFuncoes - 1; i++) {
        (*tridiagJacobiana)[SUPERIOR][i] = evaluator_evaluate(tridiagDeriv[SUPERIOR][i], numFuncoes, vars, valoresX);
        (*tridiagJacobiana)[PRINCIPAL][i] = evaluator_evaluate(tridiagDeriv[PRINCIPAL][i], numFuncoes, vars, valoresX);
        (*tridiagJacobiana)[INFERIOR][i] = evaluator_evaluate(tridiagDeriv[INFERIOR][i], numFuncoes, vars, valoresX);
    }
    (*tridiagJacobiana)[PRINCIPAL][i] = evaluator_evaluate(tridiagDeriv[PRINCIPAL][i], numFuncoes, vars, valoresX);
}

// Calcula as três diagonais da matriz de derivadas parciais usando as técnicas Unroll & Jam
void geraMatTridiagonalDerivParcial(void ****diagonais, SNL sistema) {
    char var[MAX_SIZE_STR];
    int i = 0;
    int limit = (sistema.numFuncoes - 1) - (sistema.numFuncoes - 1) % 4; 
    for (i = 0; i < limit; i += 4) {
        sprintf(var,"x%d", i+2);
        (*diagonais)[SUPERIOR][i] = evaluator_derivative(sistema.funcoes[i], var);
        sprintf(var,"x%d", i+1);
        (*diagonais)[PRINCIPAL][i] = evaluator_derivative(sistema.funcoes[i], var);
        (*diagonais)[INFERIOR][i] = evaluator_derivative(sistema.funcoes[i+1], var);

        sprintf(var,"x%d", i+3);
        (*diagonais)[SUPERIOR][i+1] = evaluator_derivative(sistema.funcoes[i+1], var);
        sprintf(var,"x%d", i+2);
        (*diagonais)[PRINCIPAL][i+1] = evaluator_derivative(sistema.funcoes[i+1], var);
        (*diagonais)[INFERIOR][i+1] = evaluator_derivative(sistema.funcoes[i+2], var);

        sprintf(var,"x%d", i+4);
        (*diagonais)[SUPERIOR][i+2] = evaluator_derivative(sistema.funcoes[i+2], var);
        sprintf(var,"x%d", i+3);
        (*diagonais)[PRINCIPAL][i+2] = evaluator_derivative(sistema.funcoes[i+2], var);
        (*diagonais)[INFERIOR][i+2] = evaluator_derivative(sistema.funcoes[i+3], var);

        sprintf(var,"x%d", i+5);
        (*diagonais)[SUPERIOR][i+3] = evaluator_derivative(sistema.funcoes[i+3], var);
        sprintf(var,"x%d", i+4);
        (*diagonais)[PRINCIPAL][i+3] = evaluator_derivative(sistema.funcoes[i+3], var);
        (*diagonais)[INFERIOR][i+3] = evaluator_derivative(sistema.funcoes[i+4], var);
    }

    // Resíduos
    for (i = limit; i < sistema.numFuncoes - 1; i++) {
        sprintf(var,"x%d", i+2);
        (*diagonais)[SUPERIOR][i] = evaluator_derivative(sistema.funcoes[i], var);
        sprintf(var,"x%d", i+1);
        (*diagonais)[PRINCIPAL][i] = evaluator_derivative(sistema.funcoes[i], var);
        (*diagonais)[INFERIOR][i] = evaluator_derivative(sistema.funcoes[i+1], var);
    }
    sprintf(var,"x%d", i+1);
    (*diagonais)[PRINCIPAL][i] = evaluator_derivative(sistema.funcoes[i], var);
}

void eliminacaoGaussJordanMatTridiagonal(double ***mat, double **termosLivres, double **x, int numFuncoes) {
    double m;
    for (int i = 0; i < numFuncoes - 1; i++) {
        m = (*mat)[SUPERIOR][i] / (*mat)[PRINCIPAL][i];
        (*mat)[SUPERIOR][i] = 0.0;
        (*mat)[PRINCIPAL][i+1] -= (*mat)[INFERIOR][i] *m;
        (*termosLivres)[i+1] -=  (*termosLivres)[i] * m;
    }

    (*x)[numFuncoes-1] = (*termosLivres)[numFuncoes-1] / (*mat)[PRINCIPAL][numFuncoes-1];
    for (int i = numFuncoes-2; i >= 0; --i) {
        (*x)[i] = ((*termosLivres)[i] - (*mat)[INFERIOR][i] * (*x)[i+1]) / (*mat)[1][PRINCIPAL];
    }
}

int metodoNewtonSNLMatTridiagonal(SNL sistema, double *xAprox, double epsilon, int maxIteracoes, FILE *saida) {
    double *deltaX, *termosLivres, **diagCoeficientes, normaFuncoes, normaDelta, temp, tempos[4];
    void ***diagDerivadas;
    char **vars;

    // Alocações
    deltaX = malloc(sizeof(double) * PAD(sistema.numFuncoes));
    termosLivres = malloc(sizeof(double) * PAD(sistema.numFuncoes));
    
    if (alocaMatPonteirosVoids(sistema.numFuncoes, sistema.numFuncoes, &diagDerivadas) == -1 ||
        alocaMatDoubles(3, sistema.numFuncoes, &diagCoeficientes) == -1 ||
        alocaMatChars(sistema.numFuncoes, MAX_SIZE_STR, &vars) == -1||            // Monta matriz de variavéis, para usar no evaluator_evaluate
        termosLivres == NULL || deltaX == NULL) {

        free(deltaX);
        free(termosLivres);
        freeMatDoubles(&diagCoeficientes);
        freeMatPonteirosVoids(&diagDerivadas, sistema.numFuncoes, sistema.numFuncoes);
        return -1;
    }
        
    geraVars(&vars, sistema.numFuncoes);

    LIKWID_MARKER_INIT;

    // Gera matriz de derivadas parciais e calcula seu tempo de execução
    tempos[INDICE_DERIVADAS] = timestamp();
    LIKWID_MARKER_START("mat-derivadas-parciais");  
    geraMatTridiagonalDerivParcial(&diagDerivadas, sistema);
    LIKWID_MARKER_STOP("mat-derivadas-parciais");  
    tempos[INDICE_DERIVADAS] = timestamp() - tempos[INDICE_DERIVADAS];
    
    // Printa primeiro bloco de aproximações
    fprintf(saida, "#\n");
    for (int i = 0; i < sistema.numFuncoes; i++) {
        fprintf(saida, "x%d = %lf\n", i + 1, xAprox[i]);
    }

    // Executa método newton e calcula seu tempo de execução
    tempos[INDICE_JACOBIANA] = tempos[INDICE_SL] = 0;
    tempos[INDICE_TOTAL] = timestamp();
    
    LIKWID_MARKER_START("metodo-newton");
    for (int i = 0; i < maxIteracoes; i++) {
        // Defini norma de funções e termos independentes
        normaFuncoes = 0;
        for (int j = 0; j < sistema.numFuncoes; j++) {
            termosLivres[j] = evaluator_evaluate(sistema.funcoes[j], sistema.numFuncoes, vars, xAprox);

            if (fabs(termosLivres[j]) > normaFuncoes) {
                normaFuncoes = fabs(termosLivres[j]);
            }
            termosLivres[j] *= -1;
        }
        
        // Verifica se a norma das funções satisfaz o critério epsilon
        if (normaFuncoes < epsilon) {
            free(deltaX);
            freeMatChars(&vars);
            freeMatPonteirosVoids(&diagDerivadas, sistema.numFuncoes, sistema.numFuncoes);
            freeMatDoubles(&diagCoeficientes);

            tempos[INDICE_TOTAL] = timestamp() - tempos[INDICE_TOTAL];
            printaTemposMetodoNewtonSNL(saida, tempos);
            return 0;
        }

        // Como critério espilon não foi satisfeito, vai haver uma nova aproximação
        fprintf(saida, "#\n");

        // Calcula jacobiana e seu tempo de execução
        temp = timestamp();
        LIKWID_MARKER_START("jacobiana");
        calculaMatJacobianaTriadigonal(diagDerivadas, &diagCoeficientes, vars, sistema.numFuncoes, xAprox);
        LIKWID_MARKER_STOP("jacobiana");
        temp = timestamp() - temp;
        tempos[INDICE_JACOBIANA] += temp;

        // // Resolve SL e calcula seu tempo de execução
        temp = timestamp();
        LIKWID_MARKER_START("resolucao-snl");
        eliminacaoGaussJordanMatTridiagonal(&diagCoeficientes, &termosLivres, &deltaX, sistema.numFuncoes);
        LIKWID_MARKER_STOP("resolucao-snl");
        temp = timestamp() - temp;
        tempos[INDICE_SL] += temp;

        // Atualiza xAprox com as novas aproximações encontradas e obtém norma do delta x
        normaDelta = 0;

        for (int j = 0; j < sistema.numFuncoes; j++) {
            xAprox[j] += deltaX[j];

            fprintf(saida, "x%d = %lf\n", j+1, xAprox[j]);
            if (fabs(deltaX[j]) > normaDelta) {
                normaDelta = fabs(deltaX[j]);
            }
        }

        // Verifica de se norma do delta x satisfaz o critério epsilon
        if (normaDelta < epsilon) {
            tempos[INDICE_TOTAL] = timestamp() - tempos[INDICE_TOTAL];
            printaTemposMetodoNewtonSNL(saida, tempos);

            free(deltaX);
            freeMatChars(&vars);
            freeMatPonteirosVoids(&diagDerivadas, sistema.numFuncoes, sistema.numFuncoes);
            freeMatDoubles(&diagCoeficientes);
            return 0;
        }
    }
    LIKWID_MARKER_STOP("metodo-newton");  
    tempos[INDICE_TOTAL] = timestamp() - tempos[INDICE_TOTAL];
    printaTemposMetodoNewtonSNL(saida, tempos);
    
    LIKWID_MARKER_CLOSE;
    free(deltaX);
    freeMatChars(&vars);
    freeMatPonteirosVoids(&diagDerivadas, sistema.numFuncoes, sistema.numFuncoes);
    freeMatDoubles(&diagCoeficientes);
    return 0;
}