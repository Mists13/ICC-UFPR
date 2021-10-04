#include <assert.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <matheval.h>


double iteracao(double x0, double f(double x), double phi(double x),
                double epsilon_x, int max_iter){

    double x_new, x_old = x0;
    int criterio1, criterio2, criterio3;
    int iter = 1;

    do {
        x_old = x_new;
        x_new = phi(x_old); // metodo linear normal;

        criterio1 = (fabs(x_old - x_new) < epsilon_x);
        // criterio2 = (fabs(f(x_new)) < epsilon_f);
        criterio3 = (iter == max_iter);
        ++iter;
    } while(!criterio1 && !criterio2);

    return x_new;
 }

double newtonRaphson(double x0, double f(double x), double epsilon_x, int max_iter){

    double x_new, x_old = x0;
    int criterio1, criterio2;
    int i = 1;

    do {
        x_old = x_new;
        x_new = x_old - (phi(x_old)/derivative(x_old));                   // xi - ( f(xi)/f'(xi) )

        criterio1 = (fabs(x_old - x_new) < epsilon_x);
        criterio2 = (i == max_iter);
        ++i;
    } while(!criterio1 && !criterio2);

    return x_new;
 }

int main(int argc, char *argv[]){
  
    char *function = argv[1];
    double initialAprox = atof(argv[2]);
    double epsilon = atof(argv[3]);
    int max_iter = atoi(argv[4]);

    // printf("F(x): %s, x0: %1.16e, Epsilon: %1.16e, Max iterations: %d\n", function, initialAprox, epsilon, max_iter);


    // Testing libmatheval
    void *f;
    
    f = evaluator_create (function);        // function: x^2-2
    assert (f);

    
    printf ("f(0.1) = %g\n", evaluator_evaluate_x (f, 0.1));
    // iteracao(initialAprox, function, phi, epsilon, max_iter);
    // newtonRaphson(initialAprox, function, epsilon, max_iter);

    return 0;

 }