#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include "iterativeMethods.h"
#include "functions.h"

int main(int argc, char *argv[]) {
    // x = aproximação atual (x_i)
    // x_old = aproximação anterior (x_{i-1})
    // x_new = a nova aproximação (x_{i+1})
    double newton_x_new, newton_x, newton_x_old, secante_x, secante_x_new, secante_x_old;
    double abs_error, relative_error, x_0, epsilon_x;
    double newton_crit, secante_crit;
    int stop_newton, stop_secante, i, max_iter;
    char f[50];

    // Le função, aproximição inicial (x_0), epsilon e o máximo de iterações
    scanf("%[^\n]%*c", f); // Le atê achar um \n
    scanf("%lf", &x_0);
    scanf("%lf", &epsilon_x);
    scanf("%d", &max_iter);

    // printa primeiros valores da aproximação - valor de input
    printf("%d, %1.16e, %1.16e, %1.16e, %1.16e, %1.16e, %1.16e, %d\n", 
            0, x_0, fabs(x_0), x_0, fabs(x_0), 0.0, 0.0, 0);

    // o valor da primeira iteração para o metódo da secante é a primeira do método newton
    secante_x = newton_x = x_0;
    newton_x_new = secante_x_new = phiNewton(x_0, x_0, f);
    if (isnan(secante_x_new) || isinf(secante_x_new)) {
        printf("\nA derivada da primeira iteração deu 0 ou infinito\n");
        return 0;
    }

    // o valor da funcao na raiz estimada é o primeiro criteiro de parada
    // pois nao ha uma aprox. anterior para calcular o erro relativo caso a derivada seja 0 
    newton_crit = secante_crit = (newton_x_new != 0.0) ? fabs(1 - (newton_x / newton_x_new)) * 100 : fabs(newton_x);
    stop_newton = stop_secante = (newton_crit < epsilon_x);

    // printa segunda iteração
    printf("%d, %1.16e, %1.16e, %1.16e, %1.16e, %1.16e, %1.16e, %d\n",
            1, newton_x_new, newton_crit, secante_x_new, secante_crit, 0.0, 0.0, 0);

    for (i = 2; (i <= max_iter) && !(stop_newton && stop_secante); ++i) {
        if (!stop_secante) {
            secante_x_old = secante_x;
            secante_x = secante_x_new;
            secante_x_new = phiSecante(secante_x, secante_x_old, f);
            
            secante_crit = (secante_x_new != 0.0) ? fabs(1 - (secante_x / secante_x_new)) * 100 : fabs(secante_x);
            stop_secante = (secante_crit < epsilon_x);
        }
        
        if (!stop_newton) {
            newton_x_old = newton_x;
            newton_x = newton_x_new;
            newton_x_new = phiNewton(newton_x_old, newton_x, f); 
                   
            newton_crit = (newton_x_new != 0.0) ? fabs(1 - (newton_x / newton_x_new)) * 100 : fabs(newton_x);
            stop_newton = (newton_crit < epsilon_x);
        }

        abs_error = newton_x_new - secante_x_new;
        relative_error = fabs(abs_error / newton_x_new);
        
        printf("%d, %1.16e, %1.16e, %1.16e, %1.16e, %1.16e, %1.16e, %ld\n",
                i, newton_x_new, newton_crit, secante_x_new, secante_crit, abs_error,
                relative_error, ulp(newton_x_new, secante_x_new));
    }

    return 33;
}