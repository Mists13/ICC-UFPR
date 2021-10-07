#ifndef __FUNCTIONS_C__
#define __FUNCTIONS_C__

#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <matheval.h>
#include "iterativeMethods.h"
#include "functions.h"

double fx(double x, char *f) {
    void *g = evaluator_create(f);
    
    return evaluator_evaluate_x(g, x);
}

double dfx(double x, char *f) {
    void *g = evaluator_create (f);
    g = evaluator_derivative_x(g);

    return evaluator_evaluate_x(g, x);
}

long int ulp(double newton_x_new, double secante_x_new) {
    long int secante_x_new_int, newton_x_new_int;
   
    memcpy(&newton_x_new_int, &newton_x_new, sizeof(newton_x_new));
    memcpy(&secante_x_new_int, &secante_x_new, sizeof(newton_x_new));
   
    return abs(newton_x_new_int - secante_x_new_int - 1);
}

#endif