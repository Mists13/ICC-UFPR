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

long int ulp(double x, double y) {
    long int x_long_int, y_long_int;
   
    memcpy(&x_long_int, &x, sizeof(x));
    memcpy(&y_long_int, &y, sizeof(y));
   
    return abs(x_long_int - y_long_int - 1);
}

#endif