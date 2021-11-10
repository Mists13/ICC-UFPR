typedef struct
{
    int numFuncoes;
    int tamFuncoes;
    char **funcoes;
} SistemaNaoLinear;

/* Faz com que sistema.funcoes apontem para NULL
 * Retorna 0 se houve sucesso, caso contrário retorna -1 */
int inicializaSistemaNaoLinear(SistemaNaoLinear *sistema, int numFuncoes, int tamFuncoes);

/* Libera memória alocada dinâmicamente */
void finalizaSistemaNaoLinear(SistemaNaoLinear *sistema);

int geraMatDerivParcial(char ****matDerivParcial, SistemaNaoLinear sistema);