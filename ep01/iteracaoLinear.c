#include <assert.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <matheval.h>

double phiNr(double x, char *f) {
    void *g = evaluator_create (f);  
    void *dg = evaluator_derivative_x (f);
    assert (g);
    assert (dg);
    return (x - (evaluator_evaluate_x(g, x)) / evaluator_evaluate_x(dg, x)); // x_{i+1} = - (f(x_i)/f'(x_i))
}

double phiSecante(double x, double xOld , char *f) {
    double resOld = function(xOld, f);
    double res = function(x, f);

    return (x - (res * (x - xOld)) / (res - resOld)); // x_{i+1} = x_i - f(x_i)*(x_i - x_{i-1}) / f(x_i) - f(x_{i-1})
}

double function(double x, char *f) {
    void *g = evaluator_create(f);
    assert(g);
    
    return evaluator_evaluate_x(g, x);
}

double functionDerivative(double x, char *f) {
    void *g = evaluator_create (f);
    assert(g);
    *g = evaluator_derivative_x(g);
    assert(g);

    return evaluator_evaluate_x(g, x);
}

double newtonRaphson(double x0, char *f, double phi(double x), double epsilon_x, int max_iter){

    double x_new, x_old = x0;
    int criterio1, criterio2;
    int i = 1;

    do {
        x_old = x_new;
        x_new = phi(x_old);  //x_old - ((f(x_old))/derivada(x_old));                   // xi - ( f(xi)/f'(xi) )
        

        criterio1 = (fabs(x_old - x_new) < epsilon_x);
        criterio2 = (i == max_iter);
        ++i;
    } while(!criterio1 && !criterio2);

    return x_new;
 }

int main(int argc, char *argv[]){
  
    char *string = argv[1];
    // double initialAprox = atof(argv[2]);
    // double epsilon = atof(argv[3]);
    // int max_iter = atoi(argv[4]);

    // printf("F(x): %s, x0: %1.16e, Epsilon: %1.16e, Max iterations: %d\n", function, initialAprox, epsilon, max_iter);

    printf ("f(0.1) = %g\n", function(0.1, string));
    printf ("f'(0.1) = %g\n", functionDerivative(0.1, string));
    // iteracao(initialAprox, function, phi, epsilon, max_iter);
    // newtonRaphson(initialAprox, function, epsilon, max_iter);

    return 0;

 }