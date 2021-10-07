#ifndef __ITERATIVE_METHODS_H__
#define __ITERATIVE_METHODS_H__

double phiNewton(double x_old, double x, char *f);
// Calcula função phi do método newton raphson

double phiSecante(double x, double x_old, char *f);
// Calcula função phi do método da secante

#endif