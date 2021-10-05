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

    double newton_x_new = x0;
    double newton_x = x0;
    double secante_x = x0;              // xi
    double secante_x_new = x0;          // xi+1
    double secante_x_old = x0;          // xi-1
    double x_old = x0;
    
    int criterio1, criterio2;
    int criterio_newton = 0;
    int criterio_secante = 0;
    int i = 2;
    double absError = x0;
    double relativeError = x0;

    printf("Iteração: %d, Secante x new: %1.16e, Newton x new: %1.16e\n", 0, secante_x, newton_x);
    newton_x = newton_x_new;
    newton_x_new = phiNewton(newton_x, f); 
    criterio_newton = (fabs(fx(newton_x_new, f)) < epsilon_x);
    secante_x_new = newton_x_new;

    // o valor de x1 para o metódo da secante é o resultado da 
    // primeira iteração do método newton
    printf("Iteração: %d, Secante x new: %1.16e, critério secante: %1.16e, Newton x new: %1.16e, critério newton : %1.16e\n", 1, newton_x_new, newton_x_new, fabs(fx(newton_x_new, f)), fabs(fx(newton_x_new, f)));

    
    do
    {
        if (!criterio_secante){
            secante_x_old = secante_x;
            secante_x = secante_x_new;
            secante_x_new = phiSecante(secante_x, secante_x_old, f);

            criterio_secante = (fabs(fx(secante_x_new, f)) < epsilon_x);
        }
        
        if (!criterio_newton) {
            newton_x = newton_x_new;    
            newton_x_new = phiNewton(newton_x, f); 
                                     
            criterio_newton = (fabs(fx(newton_x_new, f)) < epsilon_x);
        }

        printf("Iteração: %d, Secante x new: %1.16e, critério secante: %1.16e, Newton x new: %1.16e, critério newton : %1.16e\n", i, secante_x_new, fabs(fx(secante_x_new, f)), newton_x_new, fabs(fx(newton_x_new, f)));

        criterio1 = (criterio_newton && criterio_secante);
        criterio2 = (i == max_iter);
        ++i;
    } while (!criterio1 && !criterio2);  
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