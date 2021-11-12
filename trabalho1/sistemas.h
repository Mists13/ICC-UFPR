typedef struct
{
    int numFuncoes;
    void **funcoes;
} SistemaNaoLinear;

/* Faz com que sistema.funcoes apontem para NULL
 * Retorna 0 se houve sucesso, caso contrário retorna -1 */
int inicializaSistemaNaoLinear(SistemaNaoLinear *sistema, void ***funcoes, int numFuncoes);

/* Libera memória alocada dinâmicamente */
void finalizaSistemaNaoLinear(SistemaNaoLinear *sistema);

int geraMatDerivParcial(void ****mat, SistemaNaoLinear sistema);

double resolveSistemaNaoLinear(SistemaNaoLinear sistema, void ***matDerivParcial, double *aproxInicial, double epsilon, int maxIteracoes);