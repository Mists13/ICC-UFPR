#include <stdio.h>
#include <matheval.h>
#include "utils.h"

/*  Retorna tempo em milisegundos

    Forma de uso:
 
    double tempo;
    tempo = timestamp();
    <trecho de programa do qual se deseja medir tempo>
    tempo = timestamp() - tempo;
*/
double timestamp(void)
{
  struct timeval tp;
  gettimeofday(&tp, NULL);
  return((double)(tp.tv_sec*1000.0 + tp.tv_usec/1000.0));
}

/* Aloca matriz de coeficientes */
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

/* Aloca matriz para funcoes que geram o SL */
char **alocaMatrizChars(int lin, int col) {
    char **mat;
    int i;
    
    // aloca um vetor de LIN ponteiros para linhas
    mat = malloc (lin * sizeof (char*));
    
    // aloca um vetor com todos os elementos da matriz
    mat[0] = malloc (lin * col * sizeof (char));
    
    // ajusta os demais ponteiros de linhas (i > 0)
    for (i=1; i < lin; i++)
        mat[i] = mat[0] + i * col;

    return mat;
}

// /* Executa funcao  para um valor de x */
// double fx(double x, char *f) {
//     void *g = evaluator_create(f);
    
//     double result = evaluator_evaluate_x(g, x);
//     return result;
// }

// double dx(double x, char *f) {
//     void *g = evaluator_create(f);
    
//     double result = evaluator_evaluate_x(g, x);
//     return result;
// }
