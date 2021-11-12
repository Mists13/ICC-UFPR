typedef struct
{
    int numFuncoes;
    void **funcoes;
} SistemaNaoLinear;

typedef struct 
{
    int dimensao;
    double **coeficientes;
    double *termosLivres;
} MatrizQuadrada;

/* Faz com que sistema.funcoes apontem para NULL
 * Retorna 0 se houve sucesso, caso contrário retorna -1 */
int inicializaSistemaNaoLinear(SistemaNaoLinear *sistema, void ***funcoes, int numFuncoes);

/* Libera memória alocada dinâmicamente */
void finalizaSistemaNaoLinear(SistemaNaoLinear *sistema);

/* Gera matriz de derivadas parciais do sistema e armaziena em mat 
 * Retorna 0 se houve sucesso, caso contrário retorna -1 */
int geraMatDerivParcial(void ****mat, SistemaNaoLinear sistema);

/* Resolve o sistema não linear através do metódo de iteração newton
 * Usa epsilon e maxIteracoes como critérios de parada. Se um dos dois forem atingidos o métódo para 
 * Retorna 0 se houve sucesso, caso contrário retorna -1 */
int metodoNewtonSistemaNaoLinear(SistemaNaoLinear sistema, void ***matDerivParcial, 
        double *aproxInicial, double epsilon, int maxIteracoes);