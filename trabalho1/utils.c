#include <stdio.h>
#include <matheval.h>
#include "utils.h"

/*  Retorna tempo em milisegundos

    Forma de uso:
 
    double tempo;
    tempo = timestamp();
    <trecho de programa do qual se deseja medir tempo>
    tempo = timestamp() - tempo;
*/
double timestamp(void)
{
  struct timeval tp;
  gettimeofday(&tp, NULL);
  return((double)(tp.tv_sec*1000.0 + tp.tv_usec/1000.0));
}




// /* Executa funcao  para um valor de x */
// double fx(double x, char *f) {
//     void *g = evaluator_create(f);
    
//     double result = evaluator_evaluate_x(g, x);
//     return result;
// }

// double dx(double x, char *f) {
//     void *g = evaluator_create(f);
    
//     double result = evaluator_evaluate_x(g, x);
//     return result;
// }
