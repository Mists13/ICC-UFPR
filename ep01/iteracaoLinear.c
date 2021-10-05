#include <assert.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <matheval.h>
#include "iteracaoLinear.h"


double fx(double x, char *f) {
    void *g = evaluator_create(f);
    assert(g);
    
    return evaluator_evaluate_x(g, x);
}

double fxDerivative(double x, char *f) {
    void *g = evaluator_create (f);
    // assert(g);
    
    g = evaluator_derivative_x(g);
    // assert(g);
    
    return evaluator_evaluate_x(g, x);
}

double phiNewton(double x, char *f) {
    double result = (x - (fx(x, f)) / fxDerivative(x, f)); // x_{i+1} = x - ( f(x_i)/f'(x_i) )
    // double resDeriv = fxDerivative(x, f);
    // printf("PhiNr -> derivative result: = %lf\n", resDeriv );
    return result;
}

double phiSecante(double x, double xOld , char *f) {
    double res = fx(x, f);
    double resOld = fx(xOld, f);

    return (x - (res * (x - xOld)) / (res - resOld)); // x_{i+1} = x_i - ( f(x_i)*(x_i - x_{i-1}) / f(x_i) - f(x_{i-1}) )
}


void newtonRaphsonESecante(double x0, char *f, double phiNewton(double x, char *f), double epsilon_x, int max_iter) {

    double newton_x_new, newton_x, secante_x, secante_x_new, secante_x_old;
    double newton_crit, secante_crit;
    double abs_error, relative_error;
    int stop, stop_newton, stop_secante, i;

    printf("i, newton_x, newton_crit, secante_x, secante_crit\n");
    newton_crit = secante_crit = fabs(fx(x0, f));
    // printa primeira aproximação - valor de input
    printf("%d,%1.16e,%1.16e,%1.16e,%1.16e", 0, x0, newton_crit, x0, secante_crit);  

    // o valor da primeira iteração para o metódo da secante é o resultado da primeira iteração do método newton
    secante_x = x0;
    newton_x_new = secante_x_new = phiNewton(x0, f);     
    newton_crit  = secante_crit = fabs(fx(newton_x_new, f));
    stop_newton = stop_secante = (newton_crit < epsilon_x);

    // printa primeira iteração
    printf("%d,%1.16e,%1.16e,%1.16e,%1.16e\n", 0, newton_x_new, newton_crit, secante_x_new, secante_crit);

    i = 2;
    while ((i != max_iter) && !(stop_newton && stop_secante))
    {
        if (!stop_secante){
            secante_x_old = secante_x;
            secante_x = secante_x_new;
            secante_x_new = phiSecante(secante_x, secante_x_old, f);
            
            secante_crit = fabs(fx(secante_x_new, f));
            stop_secante = (secante_crit < epsilon_x);
        }
        
        if (!stop_newton) {
            newton_x = newton_x_new;    
            newton_x_new = phiNewton(newton_x, f); 

            newton_crit = fabs(fx(newton_x_new, f));                   
            stop_newton = (newton_crit < epsilon_x);
        }

        printf("%d,%1.16e,%1.16e,%1.16e,%1.16e\n", i, newton_x_new, newton_crit, secante_x_new, secante_crit); 
        ++i;
    }
 }

int main(int argc, char *argv[]){
  
    char *functionStr = argv[1];
    double initialAprox = atof(argv[2]);
    double epsilon = atof(argv[3]);
    int max_iter = atoi(argv[4]);

    // printf("F(x): %s, x0: %1.16e, Epsilon: %1.16e, Max iterations: %d\n", functionStr, initialAprox, epsilon, max_iter);

    newtonRaphsonESecante(initialAprox, functionStr, phiNewton, epsilon, max_iter);
    return 0;
}