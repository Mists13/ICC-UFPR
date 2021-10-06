#include <assert.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <matheval.h>
#include "iteracaoLinear.h"


double fx(double x, char *f) {
    void *g = evaluator_create(f);
    
    return evaluator_evaluate_x(g, x);
}

double fxDerivative(double x, char *f) {
    void *g = evaluator_create (f);
    g = evaluator_derivative_x(g);

    return evaluator_evaluate_x(g, x);
}

double phiNewton(double x, char *f) {
    double result = (x - (fx(x, f)) / fxDerivative(x, f)); // x_{i+1} = x - ( f(x_i)/f'(x_i) )
    return result;
}

double phiSecante(double x, double xOld , char *f) {
    double res = fx(x, f);
    double resOld = fx(xOld, f);

    return (x - ((res * x) / (res - resOld) - (res * xOld) / (res - resOld))); // x_{i+1} = x_i - ( f(x_i)*(x_i - x_{i-1}) / f(x_i) - f(x_{i-1}) )
}

long int ulp(double newton_x_new, double secante_x_new) {
    long int secante_x_new_int, newton_x_new_int;
   
    memcpy(&newton_x_new_int, &newton_x_new, sizeof(newton_x_new));
    memcpy(&secante_x_new_int, &secante_x_new, sizeof(newton_x_new));
   
    return abs(newton_x_new_int - secante_x_new_int -1);
}

void newtonRaphsonESecante(double x0, char *f, double phiNewton(double x, char *f), double epsilon_x, int max_iter) {
    double newton_x_new, newton_x, secante_x, secante_x_new, secante_x_old;
    double newton_crit, secante_crit;
    double abs_error, relative_error, aux;
    int stop, stop_newton, stop_secante, i;

    newton_crit = secante_crit = fabs(fx(x0, f));
    // printa primeira aproximação - valor de input
    printf("%d, %1.16e, %1.16e, %1.16e, %1.16e, %1.16e, %1.16e, %d\n", 0, x0, newton_crit, x0, secante_crit, 0.0, 0.0, 0);  

    // o valor da primeira iteração para o metódo da secante é o resultado da primeira iteração do método newton
    secante_x = x0;
    newton_x_new = secante_x_new = phiNewton(x0, f);     
    newton_crit  = secante_crit = fabs(fx(newton_x_new, f));
    stop_newton = stop_secante = (newton_crit < epsilon_x);

    // printa primeira iteração
    printf("%d, %1.16e, %1.16e, %1.16e, %1.16e, %1.16e, %1.16e, %d\n", 1, newton_x_new, newton_crit, secante_x_new, secante_crit, 0.0, 0.0, 0);


    for (i = 2; (i <= max_iter) && !(stop_newton && stop_secante); ++i){
        if (!stop_secante){
            secante_x_old = secante_x;
            secante_x = secante_x_new;
            secante_x_new = phiSecante(secante_x, secante_x_old, f);
            

            aux = fx(secante_x_new, f);
            secante_crit = (aux != 0.0) ? fabs(1 - (fx(secante_x, f) / aux)) * 100 : 0;
            stop_secante = (secante_crit < epsilon_x);
        }
        
        if (!stop_newton) {
            newton_x = newton_x_new;    
            newton_x_new = phiNewton(newton_x, f); 
            
            aux = fx(newton_x_new, f);
            newton_crit = (aux != 0.0) ? fabs(1 - (fx(newton_x, f) / aux)) * 100 : 0;                 
            stop_newton = (newton_crit < epsilon_x);
        }

        abs_error = newton_x_new - secante_x_new;
        relative_error = fabs(abs_error / newton_x_new);
        
        printf("%d, %1.16e, %1.16e, %1.16e, %1.16e, ", i, newton_x_new, newton_crit, secante_x_new, secante_crit);
        printf("%1.16e, %1.16e, ", abs_error, relative_error);
        printf("%ld\n", ulp(newton_x_new, secante_x_new));
    }
 }

int main(int argc, char *argv[]){
    char functionStr[50];
    double initialAprox;
    double epsilon;
    int max_iter;

    scanf("%s", functionStr);
    scanf("%lf", &initialAprox);
    scanf("%lf", &epsilon);
    scanf("%d", &max_iter);

    newtonRaphsonESecante(initialAprox, functionStr, phiNewton, epsilon, max_iter);
    return 0;
}