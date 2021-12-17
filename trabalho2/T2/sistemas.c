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

int inicializaTridiagonalPonteiros(TridiagonalPonteiros *tridiagonal, int dimensao) {
    if (alocaMatPonteirosVoids(3, dimensao, &tridiagonal->diagonais) == -1) {
        return -1;
    }

    // Por decisão de projeto a sequência escolhida foi essa
    // Note que no caso de uma tridiagonal vai as digonais superiores e inferiors vão ter dimensao - 1 elementos
    tridiagonal->superior = tridiagonal->diagonais[0];
    tridiagonal->principal = tridiagonal->diagonais[1];
    tridiagonal->inferior = tridiagonal->diagonais[2];
    return 0;
}

void finalizaTridiagonalPonteiros(TridiagonalPonteiros *tridiagonal, int dimensao) {
    freeMatPonteirosVoids(&tridiagonal->diagonais, 3, dimensao);
    tridiagonal->superior = tridiagonal->principal = tridiagonal->inferior = NULL;
}

int inicializaTridiagonalDoubles(TridiagonalDoubles *tridiagonal, int dimensao) {
    if (alocaMatDoubles(3, dimensao, &tridiagonal->diagonais) == -1) {
        return -1;
    }

    // Por decisão de projeto a sequência escolhida foi essa
    // Note que no caso de uma tridiagonal vai as digonais superiores e inferiors vão ter dimensao - 1 elementos
    tridiagonal->superior = tridiagonal->diagonais[0];
    tridiagonal->principal = tridiagonal->diagonais[1];
    tridiagonal->inferior = tridiagonal->diagonais[2];
    return 0;
};

void finalizaTridiagonalDoubles(TridiagonalDoubles *tridiagonal) {
    freeMatDoubles(&tridiagonal->diagonais);
    tridiagonal->superior = tridiagonal->principal = tridiagonal->inferior = NULL;
}

int inicializaMatSLTridiagonal(MatSLTridiagonal *mat, int dimensao) {
    mat->dimensao = dimensao;
    mat->termosLivres = malloc(sizeof(double) * PAD(dimensao));

    if (mat->termosLivres == NULL) {
        return -1;
    } else if (inicializaTridiagonalDoubles(&mat->tridiagonal, dimensao) == -1) {
        return -1;
    } else {
        return 0;
    }
}

void finalizaMatSLTridiagonal(MatSLTridiagonal *mat) {
    free(mat->termosLivres);
    finalizaTridiagonalDoubles(&mat->tridiagonal);
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

void calculaMatJacobianaTriadigonal(TridiagonalPonteiros derivadas, MatSLTridiagonal *matSL, char **vars, double *valoresX) {
    int i = 0;
    int limit = (matSL->dimensao- 1) - (matSL->dimensao - 1) % 4; 
    for (i = 0; i < limit; i += 4) {
        matSL->tridiagonal.superior[i] = evaluator_evaluate(derivadas.superior[i], matSL->dimensao, vars, valoresX);
        matSL->tridiagonal.principal[i] = evaluator_evaluate(derivadas.principal[i], matSL->dimensao, vars, valoresX);
        matSL->tridiagonal.inferior[i] = evaluator_evaluate(derivadas.inferior[i], matSL->dimensao, vars, valoresX);
        
        matSL->tridiagonal.superior[i+1] = evaluator_evaluate(derivadas.superior[i+1], matSL->dimensao, vars, valoresX);
        matSL->tridiagonal.principal[i+1] = evaluator_evaluate(derivadas.principal[i+1], matSL->dimensao, vars, valoresX);
        matSL->tridiagonal.inferior[i+1] = evaluator_evaluate(derivadas.inferior[i+1], matSL->dimensao, vars, valoresX);

        matSL->tridiagonal.superior[i+2] = evaluator_evaluate(derivadas.superior[i+2], matSL->dimensao, vars, valoresX);
        matSL->tridiagonal.principal[i+2] = evaluator_evaluate(derivadas.principal[i+2], matSL->dimensao, vars, valoresX);
        matSL->tridiagonal.inferior[i+2] = evaluator_evaluate(derivadas.inferior[i+2], matSL->dimensao, vars, valoresX);

        matSL->tridiagonal.superior[i+3] = evaluator_evaluate(derivadas.superior[i+3], matSL->dimensao, vars, valoresX);
        matSL->tridiagonal.principal[i+3] = evaluator_evaluate(derivadas.principal[i+3], matSL->dimensao, vars, valoresX);
        matSL->tridiagonal.inferior[i+3] = evaluator_evaluate(derivadas.inferior[i+3], matSL->dimensao, vars, valoresX);
    }

    // Resíduos
    for (i = limit; i < matSL->dimensao  - 1; i++) {
        matSL->tridiagonal.superior[i] = evaluator_evaluate(derivadas.superior[i], matSL->dimensao, vars, valoresX);
        matSL->tridiagonal.principal[i] = evaluator_evaluate(derivadas.principal[i], matSL->dimensao, vars, valoresX);
        matSL->tridiagonal.inferior[i] = evaluator_evaluate(derivadas.inferior[i], matSL->dimensao, vars, valoresX);
    }
    matSL->tridiagonal.principal[i] = evaluator_evaluate(derivadas.principal[i], matSL->dimensao, vars, valoresX);
}

// Calcula as três diagonais da matriz triadigonal de derivadas parciais usando as técnicas Unroll & Jam
void geraMatTridiagonalDerivParcial(TridiagonalPonteiros *tridiagonal, SNL sistema) {
    char var[MAX_SIZE_STR];
    int i = 0;
    int limit = (sistema.numFuncoes - 1) - (sistema.numFuncoes - 1) % 4; 
    for (i = 0; i < limit; i += 4) {
        sprintf(var,"x%d", i+2);
        tridiagonal->superior[i] = evaluator_derivative(sistema.funcoes[i], var);
        sprintf(var,"x%d", i+1);
        tridiagonal->principal[i] = evaluator_derivative(sistema.funcoes[i], var);
        tridiagonal->inferior[i] = evaluator_derivative(sistema.funcoes[i+1], var);

        sprintf(var,"x%d", i+3);
        tridiagonal->superior[i+1] = evaluator_derivative(sistema.funcoes[i+1], var);
        sprintf(var,"x%d", i+2);
        tridiagonal->principal[i+1] = evaluator_derivative(sistema.funcoes[i+1], var);
        tridiagonal->inferior[i+1] = evaluator_derivative(sistema.funcoes[i+2], var);

        sprintf(var,"x%d", i+4);
        tridiagonal->superior[i+2] = evaluator_derivative(sistema.funcoes[i+2], var);
        sprintf(var,"x%d", i+3);
        tridiagonal->principal[i+2] = evaluator_derivative(sistema.funcoes[i+2], var);
        tridiagonal->inferior[i+2] = evaluator_derivative(sistema.funcoes[i+3], var);

        sprintf(var,"x%d", i+5);
        tridiagonal->superior[i+3] = evaluator_derivative(sistema.funcoes[i+3], var);
        sprintf(var,"x%d", i+4);
        tridiagonal->principal[i+3] = evaluator_derivative(sistema.funcoes[i+3], var);
        tridiagonal->inferior[i+3] = evaluator_derivative(sistema.funcoes[i+4], var);
    }

    // Resíduos
    for (i = limit; i < sistema.numFuncoes - 1; i++) {
        sprintf(var,"x%d", i+2);
        tridiagonal->superior[i] = evaluator_derivative(sistema.funcoes[i], var);
        sprintf(var,"x%d", i+1);
        tridiagonal->principal[i] = evaluator_derivative(sistema.funcoes[i], var);
        tridiagonal->inferior[i] = evaluator_derivative(sistema.funcoes[i+1], var);
    }
    sprintf(var,"x%d", i+1);
    tridiagonal->principal[i] = evaluator_derivative(sistema.funcoes[i], var);
}

void eliminacaoGaussJordanMatTridiagonal(MatSLTridiagonal *matSL, double **x) {
    double m;
    for (int i = 0; i < matSL->dimensao - 1; i++) {
        m = matSL->tridiagonal.superior[i] / matSL->tridiagonal.principal[i];
        matSL->tridiagonal.superior[i] = 0.0;
        matSL->tridiagonal.principal[i+1] -= matSL->tridiagonal.inferior[i] *m;
        matSL->termosLivres[i+1] -=  matSL->termosLivres[i] * m;
    }

    (*x)[matSL->dimensao-1] = matSL->termosLivres[matSL->dimensao-1] / matSL->tridiagonal.principal[matSL->dimensao-1];
    for (int i = matSL->dimensao - 2; i >= 0; --i) {
        (*x)[i] = (matSL->termosLivres[i] - matSL->tridiagonal.inferior[i] * (*x)[i+1]) / matSL->tridiagonal.principal[i];
    }
}

int alocacoesNewtonSNLMatTridiagonal(double **deltaX, MatSLTridiagonal *matSL, TridiagonalPonteiros *derivadas, char ***vars, int dimensao) {
    (*deltaX) = malloc(sizeof(double) * PAD(dimensao));
    if ((*deltaX) == NULL) {
        return -1;
    } 
    if (alocaMatChars(dimensao, MAX_SIZE_STR, vars) == -1) {
        free((*deltaX));
        return -1;
    }       
    if (inicializaMatSLTridiagonal(matSL, dimensao) == -1) {
        free((*deltaX));
        freeMatChars(vars);
        return -1;
    }
    if (inicializaTridiagonalPonteiros(derivadas, dimensao) == -1) {
        free((*deltaX));
        freeMatChars(vars);
        finalizaMatSLTridiagonal(matSL);
        return-1;
    }

    return 0;
}

void desalocacoesNewtonSNLMatTridiagonal(double **deltaX, MatSLTridiagonal *matSL, TridiagonalPonteiros *derivadas, char ***vars, int dimensao) {
    free((*deltaX));
    finalizaMatSLTridiagonal(matSL);
    finalizaTridiagonalPonteiros(derivadas, dimensao);
    freeMatChars(vars);
}

int metodoNewtonSNLMatTridiagonal(SNL sistema, double *xAprox, double epsilon, int maxIteracoes, FILE *saida) {
    double *deltaX, normaFuncoes, normaDelta, temp, tempos[4];
    TridiagonalPonteiros derivadas;
    MatSLTridiagonal matSL;
    char **vars;

    // Faz alocações e gera string das variaveis que serão usadas pelas funcoes da lib matheval
    alocacoesNewtonSNLMatTridiagonal(&deltaX, &matSL, &derivadas, &vars, sistema.numFuncoes);
    geraVars(&vars, sistema.numFuncoes);

    LIKWID_MARKER_INIT;

    // Gera matriz de derivadas parciais e calcula seu tempo de execução
    tempos[INDICE_DERIVADAS] = timestamp();
    LIKWID_MARKER_START("mat-derivadas-parciais");  
    geraMatTridiagonalDerivParcial(&derivadas, sistema);
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
            matSL.termosLivres[j] = evaluator_evaluate(sistema.funcoes[j], sistema.numFuncoes, vars, xAprox);

            if (fabs(matSL.termosLivres[j]) > normaFuncoes) {
                normaFuncoes = fabs(matSL.termosLivres[j]);
            }
            matSL.termosLivres[j] *= -1;
        }
        
        // Verifica se a norma das funções satisfaz o critério epsilon
        if (normaFuncoes < epsilon) {
            tempos[INDICE_TOTAL] = timestamp() - tempos[INDICE_TOTAL];
            printaTemposMetodoNewtonSNL(saida, tempos);
            desalocacoesNewtonSNLMatTridiagonal(&deltaX, &matSL, &derivadas, &vars, sistema.numFuncoes);
            return 0;
        }

        // Como critério espilon não foi satisfeito, vai haver uma nova aproximação
        fprintf(saida, "#\n");

        // Calcula jacobiana e seu tempo de execução
        temp = timestamp();
        LIKWID_MARKER_START("jacobiana");
        calculaMatJacobianaTriadigonal(derivadas, &matSL, vars, xAprox);
        LIKWID_MARKER_STOP("jacobiana");
        temp = timestamp() - temp;
        tempos[INDICE_JACOBIANA] += temp;

        // Resolve SL e calcula seu tempo de execução
        temp = timestamp();
        LIKWID_MARKER_START("resolucao-snl");
        eliminacaoGaussJordanMatTridiagonal(&matSL, &deltaX);
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
            desalocacoesNewtonSNLMatTridiagonal(&deltaX, &matSL, &derivadas, &vars, sistema.numFuncoes);
            return 0;
        }
    }
    LIKWID_MARKER_STOP("metodo-newton");  
    tempos[INDICE_TOTAL] = timestamp() - tempos[INDICE_TOTAL];
    printaTemposMetodoNewtonSNL(saida, tempos);
    desalocacoesNewtonSNLMatTridiagonal(&deltaX, &matSL, &derivadas, &vars, sistema.numFuncoes);
    
    LIKWID_MARKER_CLOSE;
    return 0;
}