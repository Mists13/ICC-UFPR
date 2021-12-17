/*
    Luzia Millena Santos Silva, GRR20185174, 	lmss18@inf.ufpr.br
    Matheus Pacheco Santos,     GRR20192786,	mps19@inf.ufpr.br
*/
#ifndef __SISTEMAS_H__
#define __SISTEMAS_H__

#define MAX_SIZE_STR 100
#define INDICE_TOTAL 0
#define INDICE_DERIVADAS 1
#define INDICE_JACOBIANA 2
#define INDICE_SL 3


/* Estrutura guarda informações para gerar um SNL. As funções são objetos evaluators da lib mathval */
typedef struct
{
    int numFuncoes;
    void **funcoes;
} SNL;


/* Estrutura utilizada para a alocação dos coeficientes da "matriz" Jacobiana. 
 * Matriz tridiagonal é alocada com 3 arrays de doubles
*/
typedef  struct
{
    double *inferior;
    double *principal;
    double *superior;
    double **diagonais;
} TridiagonalDoubles;

/* Estrutura que representa uma matriz trigiagonal, utilizando 3 arrays
 * (um para cada diagonal) para a alocação das derivadas parciais
 */
typedef struct
{
    void **inferior;
    void **principal;
    void **superior;
    void ***diagonais;
} TridiagonalPonteiros;


/* Estrutura que representa um sistema não linear tridiagonal */
typedef struct
{
    int dimensao;
    double *termosLivres;
    TridiagonalDoubles tridiagonal;

} MatSLTridiagonal;


/* Seta funções e numFuncoes */
void inicializaSNL(SNL *sistema, void ***funcoes, int numFuncoes);


/* Libera memória alocada dinâmicamente */
void finalizaSNL(SNL *sistema);


/* Gera variaveis de acordo com a dimensão e armazena em vars 
 * Por exemplo, dimensao = 3, teremos: ["x1", "x2", "x3"]
 * vars deve previamente alocado
 */
void geraVars(char ***vars, int dimensao);


/* Print os tempos do metódo do newton, na sequencia: total, cálculo das derivadas, 
   cálculo das jacobianas e cálculo das resoluçõe dos SLs  
 */
void printaTemposMetodoNewtonSNL(FILE *saida, double tempos[4]);


/* Resolve o sistema não linear através do metódo de iteração newton
 * Usa epsilon e maxIteracoes como critérios de parada. Se um dos dois forem atingidos o métódo encerra
 * Printa no arquivo "saida" as aproximações e o tempo de execução total, tempo do cálculo das jacobianas e da resolução do SNL
 * Retorna 0 se houve sucesso, caso contrário retorna -1 representando falha de alocação - única falha possível 
 */
int metodoNewtonSNLMatTridiagonal(SNL sistema, double *xAprox, double epsilon, int maxIteracoes, FILE *saida);

#endif