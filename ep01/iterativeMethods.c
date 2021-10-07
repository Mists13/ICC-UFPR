#ifndef __ITERATIVE_METHODS_C__
#define __ITERATIVE_METHODS_C__

#include "functions.h"

// x_{i+1} = x - ( f(x_i)/f'(x_i) )
double phiNewton(double x, char *f) {

    double derivative = dfx(x, f);
    double result = (x - (fx(x, f)) / derivative ); 
    
    return result;
}   

// x_{i+1} = x_i - ( f(x_i)*(x_i - x_{i-1}) / f(x_i) - f(x_{i-1}) )
double phiSecante(double x, double x_old , char *f) {
    double res = fx(x, f);
    double res_old = fx(x_old, f);

    return (x - ((res * x) / (res - res_old) - (res * x_old) / (res - res_old))); 
}

#endif