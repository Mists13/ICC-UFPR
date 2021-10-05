

double phiNewton(double x, char *f);
// Calcula função phi do método newton raphson

double phiSecante(double x, double xOld, char *f);
// Calcula função phi do método da secante

double fx(double x, char *f);
// Calcula f(x)

double fxDerivative(double x, char *f);


void newtonRaphsonESecante(double x0, char *f, double phiNewton(double x, char *f), double epsilon_x, int max_iter);