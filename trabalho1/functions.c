#ifndef __FUNCTIONS_C__
#define __FUNCTIONS_C__

#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <matheval.h>
#include "functions.h"

double fx(double x, char *f) {
    void *g = evaluator_create(f);
    
    return evaluator_evaluate_x(g, x);
}  

void *derivadaParcial(char *funcao, double x, int indice){

    char **nomesVariaveis;
    int count;
    void *f;

    f = evaluator_create (funcao);
    evaluator_get_variables(f, &nomesVariaveis, &count);

    for (int i = 0; i < count; i++)
         printf ("Variavel %d: %s\n", i, nomesVariaveis[i]);
    
    void *f_prim = evaluator_derivative(f, nomesVariaveis[indice]);
    printf ("  f'(x) = %s\n", evaluator_get_string (f_prim));
    return NULL;
}


#endif