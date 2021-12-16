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

// void geraMatDerivParcial(void ****mat, SNL sistema) {
//     char var[MAX_SIZE_STR];

//     for (int i = 0; i < sistema.numFuncoes; i++) {
//         sprintf(var,"x%d", i+1); // transforma inteiro em string 
//         for (int j = 0; j < sistema.numFuncoes; j++) {
//             (*mat)[j][i] = evaluator_derivative(sistema.funcoes[j], var);
//         }
//     }
// }


void geraMatDerivParcial(void **diagPrincipal, SNL sistema) {
    char var[MAX_SIZE_STR];
    int n = sistema.numFuncoes;

    for (int i = 0; i < n; i++) {
        sprintf(var,"x%d", i+1);
        diagPrincipal[i] = evaluator_derivative(sistema.funcoes[i], var);
        printf("f'(x):%s\n", evaluator_get_string(diagPrincipal[i]));
    }
}



void eliminacaoGaussJordan(MatQuadraticaSL *mat) {

    int n = mat->dimensao;
    int a = 0;      // Diagonal inferior
    int d = 1;      // Diagonal principal
    int c = 2;      // Diagonal superior
    
    for (int i = 0; i < n; ++i) {

        double m = mat->coeficientes[a][i]/ mat->coeficientes[d][i];
        mat->coeficientes[a][i] = 0.0;

        mat->coeficientes[d][i+1] -= mat->coeficientes[c][i]*m;
        mat->termosLivres[i+1] -= mat->termosLivres[i]*m;
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

// void eliminacaoGaussJordan(MatQuadraticaSL *mat) {
//     int pivo;

//     for (int i = 0; i < mat->dimensao; ++i) {
//         // Caso esteja na última coluna dos coeficientes não é mais necessário realizar troca de linha
//         if (i < mat->dimensao - 1) {
//             pivo = econtraMaxPivo(&mat->coeficientes, i, mat->dimensao);
//             if (i != pivo) {
//                 trocaLinhaMat(mat, i, pivo);
//             }
//         }

//         for (int j = i+1; j < mat->dimensao; ++j) {
//             double m = (*mat).coeficientes[j][i] / mat->coeficientes[i][i];
//             mat->coeficientes[j][i] = 0.0;

//             for (int k = i+1; k < mat->dimensao; ++k) {
//                 mat->coeficientes[j][k] -= mat->coeficientes[i][k] * m;
//             }

//             (mat->termosLivres)[j] -= (mat->termosLivres)[i] * m;
//         }
//     }
// }

void calculaMatJacobiana(void **diagPrincipal, double ***matJacobiana, char **vars, int numFuncoes, double *valoresX) {

    for (int i = 0; i < numFuncoes; i+=3){
        (*matJacobiana)[0][i] = -1.0;
        (*matJacobiana)[1][i+1] = evaluator_evaluate(diagPrincipal[i], numFuncoes, vars, valoresX);
        printf("na funcao %s\n", evaluator_get_string(diagPrincipal[i]));
        printf("valor %lf\n", (*matJacobiana)[1][i+1]);
        (*matJacobiana)[2][i+2] = -2.0;
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

void calculaMatJacobianaTriadigonal(void ***matDiagDeriv, double ***matDiagJacobiana, char **vars, int numFuncoes, double *valoresX) {
    int i = 0;
    int limit = (numFuncoes- 1) - (numFuncoes - 1) % 4; 
    for (i = 0; i < limit; i += 4) {
        (*matDiagJacobiana)[0][i] = evaluator_evaluate(matDiagDeriv[0][i], numFuncoes, vars, valoresX);
        (*matDiagJacobiana)[1][i] = evaluator_evaluate(matDiagDeriv[1][i], numFuncoes, vars, valoresX);
        (*matDiagJacobiana)[2][i] = evaluator_evaluate(matDiagDeriv[2][i], numFuncoes, vars, valoresX);
        
        (*matDiagJacobiana)[0][i+1] = evaluator_evaluate(matDiagDeriv[0][i+1], numFuncoes, vars, valoresX);
        (*matDiagJacobiana)[1][i+1] = evaluator_evaluate(matDiagDeriv[1][i+1], numFuncoes, vars, valoresX);
        (*matDiagJacobiana)[2][i+1] = evaluator_evaluate(matDiagDeriv[2][i+1], numFuncoes, vars, valoresX);

        (*matDiagJacobiana)[0][i+2] = evaluator_evaluate(matDiagDeriv[0][i+2], numFuncoes, vars, valoresX);
        (*matDiagJacobiana)[1][i+2] = evaluator_evaluate(matDiagDeriv[1][i+2], numFuncoes, vars, valoresX);
        (*matDiagJacobiana)[2][i+2] = evaluator_evaluate(matDiagDeriv[2][i+2], numFuncoes, vars, valoresX);

        (*matDiagJacobiana)[0][i+3] = evaluator_evaluate(matDiagDeriv[0][i+3], numFuncoes, vars, valoresX);
        (*matDiagJacobiana)[1][i+3] = evaluator_evaluate(matDiagDeriv[1][i+3], numFuncoes, vars, valoresX);
        (*matDiagJacobiana)[2][i+3] = evaluator_evaluate(matDiagDeriv[2][i+3], numFuncoes, vars, valoresX);
    }

    // Resíduos
    for (i = limit; i < numFuncoes - 1; i++) {
        (*matDiagJacobiana)[0][i] = evaluator_evaluate(matDiagDeriv[0][i], numFuncoes, vars, valoresX);
        (*matDiagJacobiana)[1][i] = evaluator_evaluate(matDiagDeriv[1][i], numFuncoes, vars, valoresX);
        (*matDiagJacobiana)[2][i] = evaluator_evaluate(matDiagDeriv[2][i], numFuncoes, vars, valoresX);
    }
    (*matDiagJacobiana)[1][i] = evaluator_evaluate(matDiagDeriv[1][i], numFuncoes, vars, valoresX);
}

// Calcula as três diagonais da matriz de derivadas parciais usando as técnicas Unroll & Jam
void geraMatTridiagonalDerivParcial(void ****diagonais, SNL sistema) {
    char var[MAX_SIZE_STR];
    int i = 0;
    int limit = (sistema.numFuncoes - 1) - (sistema.numFuncoes - 1) % 4; 
    for (i = 0; i < limit; i += 4) {
        sprintf(var,"x%d", i+2);
        (*diagonais)[0][i] = evaluator_derivative(sistema.funcoes[i], var);
        sprintf(var,"x%d", i+1);
        (*diagonais)[1][i] = evaluator_derivative(sistema.funcoes[i], var);
        (*diagonais)[2][i] = evaluator_derivative(sistema.funcoes[i+1], var);

        sprintf(var,"x%d", i+3);
        (*diagonais)[0][i+1] = evaluator_derivative(sistema.funcoes[i+1], var);
        sprintf(var,"x%d", i+2);
        (*diagonais)[1][i+1] = evaluator_derivative(sistema.funcoes[i+1], var);
        (*diagonais)[2][i+1] = evaluator_derivative(sistema.funcoes[i+2], var);

        sprintf(var,"x%d", i+4);
        (*diagonais)[0][i+2] = evaluator_derivative(sistema.funcoes[i+2], var);
        sprintf(var,"x%d", i+3);
        (*diagonais)[1][i+2] = evaluator_derivative(sistema.funcoes[i+2], var);
        (*diagonais)[2][i+2] = evaluator_derivative(sistema.funcoes[i+3], var);

        sprintf(var,"x%d", i+5);
        (*diagonais)[0][i+3] = evaluator_derivative(sistema.funcoes[i+3], var);
        sprintf(var,"x%d", i+4);
        (*diagonais)[1][i+3] = evaluator_derivative(sistema.funcoes[i+3], var);
        (*diagonais)[2][i+3] = evaluator_derivative(sistema.funcoes[i+4], var);
    }

    // Resíduos
    for (i = limit; i < sistema.numFuncoes - 1; i++) {
        sprintf(var,"x%d", i+2);
        (*diagonais)[0][i] = evaluator_derivative(sistema.funcoes[i], var);
        sprintf(var,"x%d", i+1);
        (*diagonais)[1][i] = evaluator_derivative(sistema.funcoes[i], var);
        (*diagonais)[2][i] = evaluator_derivative(sistema.funcoes[i+1], var);
    }
    sprintf(var,"x%d", i+1);
    (*diagonais)[1][i] = evaluator_derivative(sistema.funcoes[i], var);
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
    deltaX = malloc(sizeof(double) * sistema.numFuncoes);
    if (deltaX == NULL) {
        return -1;
    }
    termosLivres = malloc(sizeof(double) * sistema.numFuncoes);
    if (termosLivres == NULL) {
        return -1;
    }
    if (alocaMatPonteirosVoids(sistema.numFuncoes, sistema.numFuncoes, &diagDerivadas) == -1) {
        free(deltaX);
        return -1;
    }
    if (alocaMatDoubles(3, sistema.numFuncoes, &diagCoeficientes) == -1) {
        free(deltaX);
        freeMatPonteirosVoids(&diagDerivadas, sistema.numFuncoes, sistema.numFuncoes);
    }
    // Monta matriz de variavéis, para usar no evaluator_evaluate
    if (alocaMatChars(sistema.numFuncoes, MAX_SIZE_STR, &vars)) {
        free(deltaX);
        freeMatPonteirosVoids(&diagDerivadas, sistema.numFuncoes, sistema.numFuncoes);
        freeMatDoubles(&diagCoeficientes);
        return -1;
    } else {
        geraVars(&vars, sistema.numFuncoes);
    }

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