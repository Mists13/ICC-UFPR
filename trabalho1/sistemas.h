typedef struct
{
    int dimensao;
    int maxIteracoes;
    char **funcoes;
    char ***matDerivParcial;
    double *aproxInicial;
    double epsilon;
} SistemaNaoLinear;

/* Faz com que sistema.funcoes, sistema.matDerivParcial e sistema.aproxInicial apontem para NULL */
void inicializaSistemaNaoLinear(SistemaNaoLinear *sistema);

/* Usa a função free em  sistema.funcoes, sistema.matDerivParcial e sistema.aproxInicial - nos três *
 * casos testa se foi alocado antes de limpar                                                       */
void finalizaSistemaNaoLinear(SistemaNaoLinear *sistema);