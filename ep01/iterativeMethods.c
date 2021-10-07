#ifndef __ITERATIVE_METHODS_C__
#define __ITERATIVE_METHODS_C__

#include "functions.h"

double phiNewton(double x, char *f) {
    double derivative = dfx(x, f);
    double result = (x - (fx(x, f)) / derivative ); // x_{i+1} = x - ( f(x_i)/f'(x_i) )
    
    return result;
}   

double phiSecante(double x, double xOld , char *f) {
    double res = fx(x, f);
    double resOld = fx(xOld, f);

    return (x - ((res * x) / (res - resOld) - (res * xOld) / (res - resOld))); // x_{i+1} = x_i - ( f(x_i)*(x_i - x_{i-1}) / f(x_i) - f(x_{i-1}) )
}

#endif