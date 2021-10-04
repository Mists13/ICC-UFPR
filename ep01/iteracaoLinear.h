

double phiNr(double x, char *f);
double phiSecante(double x, double xOld, char *f);
double fx(double x, char *f);
double fxDerivative(double x, char *f);
double newtonRaphson(double x0, char *f, double phi(double x), double epsilon_x, int max_iter);