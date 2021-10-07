#ifndef __FUNCTIONS_H__
#define __FUNCTIONS_H__

double fx(double x, char *f);
// Calcula f(x)

double dfx(double x, char *f);
// Calcula 1º derivada de f(x)

long int ulp(double newton_x_new, double secante_x_new);

#endif 