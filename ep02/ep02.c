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

// double *solveLinearSystem(double diagonalsMatrix[MAX_STR][50], int k, int n, int mainDiagIndex){

//     double *xValuesArray;
//     double *results;
//     int u, b = 0;


//     // // para cada linha da matriz de diagonais
//     // for(int i = 0; i < n-1; i++){   
//     //     xValuesArray[i] = diagonalsMatrix[k][i];        // termo inpendente na ultima linha da matriz        

//     //     for(int j = 0; j < k; j++){                 
//     //         xValuesArray[i] =- diagonalsMatrix[j][i]      
//     //     }
        

// }



// void generateLinearSystem(char functions[MAX_STR][50], int n, int k){

//     double diagonalsMatrix[k+1][n+1];                                   // diagonal size is maximum k 
//     int mainDiagIndex = k/2;
//     int thisDiagSize = 0;
//     int atBottomDiag = 0;
    
//     // for each diagonal, generates an array using functions
//     for(int i = 0; i < k; i++){   

//         if (thisDiagSize  == n ){                                  // main diagonal

//             for(int j = 0; j < thisDiagSize; j++)                  
//                 diagonalsMatrix[i][j] = fx(j, functions[i]);
//             putchar('\n');
//             atBottomDiag = 1;
//             thisDiagSize--;

//         }else if ( atBottomDiag ){

//             for(int j = 0; j < thisDiagSize ; j++)            // bottom diagonal
//                 diagonalsMatrix[i][j] = fx(j, functions[i]);
//             putchar('\n');            
//             thisDiagSize--;

//         } else {   

//             thisDiagSize = n - (mainDiagIndex - i);               
//             for(int j = 0; j < thisDiagSize ; j++)               // upper diagonal
//                 diagonalsMatrix[i][j] = fx(j, functions[i]);
//             putchar('\n');
//             thisDiagSize++;
            
//         }
//     }   
        
//     // generate independent terms
//     for(int j = 0; j < n ; j++)              
//         diagonalsMatrix[k][j] = fx(j, functions[k]);

// }


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

void resolveSistemaLinear(double **mat, double *x, double*b, int dimensao) {
    double *y = calloc(dimensao, sizeof(double));
    double *errosAbsolutos = calloc(dimensao, sizeof(double));
    double max;
    int k = 0;

    do {
        k++;
        for (int i = 0; i < dimensao; i++)
        {

            y[i] = b[i];
            for (int j = i+1; j < dimensao; j++) {
                y[i] -=  x[j] * mat[i][j];
            }
            
            for (int j = i-1; j >= 0; j--) {
                y[i] -=  x[j] * mat[i][j];
            }

            y[i] /= mat[i][i];
            errosAbsolutos[i] = fabs(y[i] - x[i]);

            x[i] = y[i];
        }

        max = errosAbsolutos[0];
        for (int l = 1; l < dimensao; l++)
        {
            if (max < errosAbsolutos[l]) {
                max = errosAbsolutos[l];
            }
        }
    } while (max > 0.0001 && k < 50);
}

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
    int n, k, i, end;
    char **functions;
    double **mat, *x, *b;

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

    geraSistemaLinear(mat, b, functions, n, k);    
    resolveSistemaLinear(mat, x, b, n);

    printf("solução: ");
    for (i = 0; i < n; i++)
    {
        printf("%lf ", x[i]);
    }
    printf("\n");

    printf("tempo: %lf\n", timestamp() - tempo);
    return 33;
}
