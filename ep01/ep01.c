#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include "iterativeMethods.h"
#include "functions.h"

int main(int argc, char *argv[]){
    char f[50];
    double x0;
    double epsilon_x;
    int max_iter;

    scanf("%s", f);
    scanf("%lf", &x0);
    scanf("%lf", &epsilon_x);
    scanf("%d", &max_iter);

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
            
            secante_crit = (secante_x_new != 0.0) ? fabs(1 - (secante_x / secante_x_new)) * 100 : 0;
            stop_secante = (secante_crit < epsilon_x);
        }
        
        if (!stop_newton) {
            newton_x = newton_x_new;    
            newton_x_new = phiNewton(newton_x, f); 
            
            // newton_crit = (aux != 0.0) ? fabs(1 - (fx(newton_x, f) / aux)) * 100 : 0;                 
            newton_crit = (newton_x_new != 0.0) ? fabs(1 - (newton_x / newton_x_new)) * 100 : 0;
            stop_newton = (newton_crit < epsilon_x);
        }

        abs_error = newton_x_new - secante_x_new;
        relative_error = fabs(abs_error / newton_x_new);
        
        printf("%d, %1.16e, %1.16e, %1.16e, %1.16e, ", i, newton_x_new, newton_crit, secante_x_new, secante_crit);
        printf("%1.16e, %1.16e, ", abs_error, relative_error);
        printf("%ld\n", ulp(newton_x_new, secante_x_new));
    }
    
    return 33;
}