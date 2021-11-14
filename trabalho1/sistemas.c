#include <stdio.h>
#include <stdlib.h>
#include <matheval.h>
#include <math.h>
#include "sistemas.h"
#include "alloc.h"
#include "utils.h"

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

int  inicializaMatQuad(MatQuadraticaSL *mat, int dimensao) {
    if (alocaMatDoubles(dimensao, dimensao, &mat->coeficientes) != 0) {
        return -1;
    }

    mat->termosLivres = malloc(sizeof(double) * dimensao);
    if (mat->termosLivres == NULL) {
        return -1;
    }

    mat->dimensao = dimensao;

    return 0;
}

void finalizaMatQuad(MatQuadraticaSL *mat) {
    freeMatDoubles(&mat->coeficientes);
    free(mat->termosLivres);
}

void geraMatDerivParcial(void ****mat, SNL sistema) {
    char var[MAX_SIZE_STR];

    for (int i = 0; i < sistema.numFuncoes; i++) {
        sprintf(var,"x%d", i+1); // transforma inteiro em string 
        for (int j = 0; j < sistema.numFuncoes; j++) {
            (*mat)[j][i] = evaluator_derivative(sistema.funcoes[j], var);
        }
    }
}

void retrossubs(MatQuadraticaSL mat, double **x) {
    for (int i = mat.dimensao-1; i >= 0; --i) {
        (*x)[i] = mat.termosLivres[i];
        for (int j = i + 1; j < mat.dimensao; ++j) {
            (*x)[i] -= mat.coeficientes[i][j] * (*x)[j];
        }
        (*x)[i] /= mat.coeficientes[i][i];
    }
}

int econtraMaxPivo(double ***mat, int j, int n) {
    double max = fabs((*mat)[j][j]);
    int indiceMax = j;

    for (int i = j+1; i < n; i++) {
        if (fabs((*mat)[i][j]) > max){
            indiceMax = i;
            max = fabs((*mat)[i][j]);
        }
    }

    return indiceMax;
}

void trocaLinhaMat(MatQuadraticaSL *mat, int i, int pivo) {
    double *x = mat->coeficientes[i];
    mat->coeficientes[i] = mat->coeficientes[pivo];
    mat->coeficientes[pivo] = x;

    double y = mat->termosLivres[i];
    mat->termosLivres[i] = mat->termosLivres[pivo];
    mat->termosLivres[pivo] = y;
}

void eliminacaoGaussJordan(MatQuadraticaSL *mat) {
    int pivo;

    for (int i = 0; i < mat->dimensao; ++i) {
        // Caso esteja na última coluna dos coeficientes não é mais necessário realizar troca de linha
        if (i < mat->dimensao - 1) {
            pivo = econtraMaxPivo(&mat->coeficientes, i, mat->dimensao);
            if (i != pivo) {
                trocaLinhaMat(mat, i, pivo);
            }
        }

        for (int j = i+1; j < mat->dimensao; ++j) {
            double m = (*mat).coeficientes[j][i] / mat->coeficientes[i][i];
            mat->coeficientes[j][i] = 0.0;

            for (int k = i+1; k < mat->dimensao; ++k) {
                mat->coeficientes[j][k] -= mat->coeficientes[i][k] * m;
            }

            (mat->termosLivres)[j] -= (mat->termosLivres)[i] * m;
        }
    }
}

void calculaMatJacobiana(void ***matDerivParcial, double ***matJacobiana, char **vars, int numFuncoes, double *valoresX) {
    for (int i = 0; i < numFuncoes; i++){
        for (int j = 0; j < numFuncoes; j++){
            (*matJacobiana)[i][j] = evaluator_evaluate(matDerivParcial[i][j], numFuncoes, vars, valoresX);
        }
    }
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

int metodoNewtonSNL(SNL sistema, double *xAprox, double epsilon, int maxIteracoes, FILE *saida) {
    double *deltaX, normaFuncoes, normaDelta, temp, tempos[4];
    void ***matDerivParcial;
    MatQuadraticaSL mat;
    char **vars;
   
    // Alocações
    deltaX = malloc(sizeof(double) * sistema.numFuncoes);
    if (deltaX == NULL) {
        return -1;
    }
    if (alocaMatPonteirosVoids(sistema.numFuncoes, sistema.numFuncoes, &matDerivParcial) == -1) {
        free(deltaX);
        return -1;
    }
    // Inicializa estrutura de acordo com sua dimensao
    if (inicializaMatQuad(&mat, sistema.numFuncoes) == -1) {
        free(deltaX);
        freeMatPonteirosVoids(&matDerivParcial, sistema.numFuncoes, sistema.numFuncoes);
        return -1;
    }
    // Monta matriz de variavéis, para usar no evaluator_evaluate
    if (alocaMatChars(sistema.numFuncoes, MAX_SIZE_STR, &vars)) {
        free(deltaX);
        finalizaMatQuad(&mat);
        freeMatPonteirosVoids(&matDerivParcial, sistema.numFuncoes, sistema.numFuncoes);
        return -1;
    } else {
        geraVars(&vars, sistema.numFuncoes);
    }

    // Gera matriz de derivadas parciais e calcula seu tempo de execução
    tempos[INDICE_DERIVADAS] = timestamp();
    geraMatDerivParcial(&matDerivParcial, sistema);
    tempos[INDICE_DERIVADAS] = timestamp() - tempos[INDICE_DERIVADAS];

    // Printa primeiro bloco de aproximações
    fprintf(saida, "#\n");
    for (int i = 0; i < sistema.numFuncoes; i++) {
        fprintf(saida, "x%d = %lf\n", i + 1, xAprox[i]);
    }

    // Executa método newton e calcula seu tempo de execução
    tempos[INDICE_JACOBIANA] = tempos[INDICE_SL] = 0;
    tempos[INDICE_TOTAL] = timestamp();    
    for (int i = 0; i < maxIteracoes; i++) {
        // Defini norma de funções e termos independentes
        normaFuncoes = 0;
        for (int j = 0; j < sistema.numFuncoes; j++) {
            mat.termosLivres[j] = evaluator_evaluate(sistema.funcoes[j], sistema.numFuncoes, vars, xAprox);

            if (fabs(mat.termosLivres[j]) > normaFuncoes) {
                normaFuncoes = fabs(mat.termosLivres[j]);
            }
            mat.termosLivres[j] *= -1;
        }
        
        // Verifica se a norma das funções satisfaz o critério epsilon
        if (normaFuncoes < epsilon) {
            free(deltaX);
            freeMatChars(&vars);
            finalizaMatQuad(&mat);
            freeMatPonteirosVoids(&matDerivParcial, sistema.numFuncoes, sistema.numFuncoes);

            tempos[INDICE_TOTAL] = timestamp() - tempos[INDICE_TOTAL];
            printaTemposMetodoNewtonSNL(saida, tempos);
            return 0;
        }

        // Como critério espilon não foi satisfeito, vai haver uma nova aproximação
        fprintf(saida, "#\n");

        // Calcula jacobiana e seu tempo de execução
        temp = timestamp();
        calculaMatJacobiana(matDerivParcial, &mat.coeficientes, vars, sistema.numFuncoes, xAprox);
        temp = timestamp() - temp;
        tempos[INDICE_JACOBIANA] += temp;

        // Resolve SL e calcula seu tempo de execução
        temp = timestamp();
        eliminacaoGaussJordan(&mat);
        retrossubs(mat,  &deltaX);
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
            free(deltaX);
            freeMatChars(&vars);
            finalizaMatQuad(&mat);
            freeMatPonteirosVoids(&matDerivParcial, sistema.numFuncoes, sistema.numFuncoes);

            tempos[INDICE_TOTAL] = timestamp() - tempos[INDICE_TOTAL];
            printaTemposMetodoNewtonSNL(saida, tempos);
            return 0;
        }
    }
    tempos[INDICE_TOTAL] = timestamp() - tempos[INDICE_TOTAL];
    printaTemposMetodoNewtonSNL(saida, tempos);
   
    free(deltaX);
    freeMatChars(&vars);
    finalizaMatQuad(&mat);
    freeMatPonteirosVoids(&matDerivParcial, sistema.numFuncoes, sistema.numFuncoes);
    return 0;
}
