typedef struct
{
    int dimensao;
    int maxIteracoes;
    char **funcoes;
    char ***matJacobiana;
    double *aproxInicial;
    double epsilon;

} SistemaNaoLinear;