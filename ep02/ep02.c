#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <matheval.h>
#include "utils.h"

#define MAX_FUNCTION 20

double **alocaMatrizDoubles(int lin, int col) {
    double **mat;
    int i;

    // aloca um vetor de LIN ponteiros para linhas
    mat = malloc (lin * sizeof (double*)) ;
    
    // aloca um vetor com todos os elementos da matriz
    mat[0] = calloc (lin * col, sizeof (double));
    
    // ajusta os demais ponteiros de linhas (i > 0)
    for (i=1; i < lin; i++)
        mat[i] = mat[0] + i * col ;

    return mat;
}

char **alocaMatrizChars(int lin, int col) {
    char **mat ;
    int i, j ;
    
    // aloca um vetor de LIN ponteiros para linhas
    mat = malloc (lin * sizeof (char*));
    
    // aloca um vetor com todos os elementos da matriz
    mat[0] = malloc (lin * col * sizeof (char));
    
    // ajusta os demais ponteiros de linhas (i > 0)
    for (i=1; i < lin; i++)
        mat[i] = mat[0] + i * col;

    return mat;
}

double fx(double x, char *f) {
    void *g = evaluator_create(f);
    
    double result = evaluator_evaluate_x(g, x);
    return result;
}


void printaSistema(double **mat, double *b, int dimensao, int k) {
    int q = (k - 1) / 2;
    
    // Printa diagonais superiores
    for (int i = q; i >= 1; i--) {
        for (int j = i, l = 0; (j < dimensao); j++, l++) {
            printf("%lf ", mat[l][j]);
        }
        printf("\n");
    }

    // Printa diagonal principal
    for (int i = 0; i < dimensao; i++)
    {
        printf("%lf ", mat[i][i]);
    }
    printf("\n");

    // Printa diagonal inferior
    for (int i = 1; i <= q; i++) {
        for (int j = i, l = 0; (j < dimensao); j++, l++) {
            printf("%lf ", mat[j][l]);
        }
        printf("\n");
    }

    // Printa termos independentes
    for (int i = 0; i < dimensao; i++)
    {
        printf("%lf ", b[i]);
    }
    printf("\n-------\n");
}

/* Resolve sistema linear atraves do metodo de Gauss Seidel */
void resolveSistemaLinear(double **mat, double *xAnterior, double*b, int dimensao, double epsilon, int maxIteracoes) {
    
    double *xAtual = calloc(dimensao, sizeof(double));
    double *errosAbsolutos = calloc(dimensao, sizeof(double));
    double max;
    int k = 0;

    do {
        k++;
        for (int i = 0; i < dimensao; i++)
        {

            // Calcula os valores depois da diagonal principal
            xAtual[i] = b[i];
            for (int j = i+1; j < dimensao; j++) {
                xAtual[i] -=  xAnterior[j] * mat[i][j];             
            }
            
            // Calcula os valores antes da diagonal principal
            for (int j = i-1; j >= 0; j--) {
                xAtual[i] -=  xAnterior[j] * mat[i][j];
            }

            xAtual[i] /= mat[i][i];
            errosAbsolutos[i] = fabs(xAtual[i] - xAnterior[i]);

            xAnterior[i] = xAtual[i];
        }

        max = errosAbsolutos[0];
        // Calcula o maior erro absoluto obtido na iteracao
        for (int l = 1; l < dimensao; l++)                        
        {
            if (max < errosAbsolutos[l]) {
                max = errosAbsolutos[l];
            }
        }
    } while (max > epsilon && k < maxIteracoes);
}

/* Gera matriz a partir de funcoes que representam cada diagonal do sistema linear */
void geraSistemaLinear(double **mat, double *b, char **functions, int dimensao, int k) {
    int q = (k - 1) / 2;

    // Seta diagonais superiores e inferiores com base no valor de q
    for (int i = 1; i <= q; i++) {
        for (int j = i, l = 0; (j < dimensao); j++, l++) {
            mat[l][j] = fx(l, functions[q-i]);
            mat[j][l] = fx(l, functions[q+i]);
        }
    }

    // Seta diagonal principal e termos independentes
    for (int i = 0; i < dimensao; i++) {
        mat[i][i] = fx(i, functions[q]);
        b[i] = fx(i, functions[k]);
    }

    printaSistema(mat, b, dimensao, k);
}

int main() {
    double tempo = timestamp();
    int n, k, i, end, maxIteracoes;
    char **functions;
    double **mat, *x, *b, epsilon;

    // Lê a dimensão da matriz e valor k-diagonal
    scanf("%d", &n);
    scanf("%d", &k);
    
    // Aloca matriz de coeficientes, vetor de variaveis x, vetor de termo independentes  e vetor de funções
    // Os valores iniciais da matriz e vetores são zeros
    functions = alocaMatrizChars(n, MAX_FUNCTION);
    mat = alocaMatrizDoubles(n, n);
    x = calloc(n, sizeof(double));
    b = calloc(n, sizeof(double));
    
    // Le as funções
    for (i = 0; i <= k; i++) {
        scanf("%s", functions[i]);
    }

    // Lê a epsilon e maximo de iteracoes
    scanf("%lf", &epsilon);
    scanf("%d", &maxIteracoes);

    geraSistemaLinear(mat, b, functions, n, k);    
    resolveSistemaLinear(mat, x, b, n, epsilon, maxIteracoes);

    printf("solução: ");
    for (i = 0; i < n; i++)
    {
        printf("%lf ", x[i]);
    }
    printf("\n");

    printf("tempo: %lf\n", timestamp() - tempo);
    return 33;
}
