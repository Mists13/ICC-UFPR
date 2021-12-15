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


/* Estrutura que respresenta um SNL. As funções são objetos evaluators da lib mathval */
typedef struct
{
    int numFuncoes;
    void **funcoes;
} SNL;

/* Estrutura que representa um SL na de forma que os coeficentes são representadis por uma matriz quadrática. Ou seja, 
   necessáriamente o SL deve ter o mesmo número de equações de variáveis
 */
typedef struct 
{
    int dimensao;
    double **coeficientes;
    double *termosLivres;
} MatQuadraticaSL;


typedef  struct
{
    int dimensao;
    double **coefecientes;
    double **termosLivres;
} MatTridiagonal;

/* Seta funções e numFuncoes */
void inicializaSNL(SNL *sistema, void ***funcoes, int numFuncoes);


/* Libera memória alocada dinâmicamente */
void finalizaSNL(SNL *sistema);


/* Aloca espaço para a matriz de acordo com a dimensão. Utiliza a lib alloc.h
 * Retorna 0 se houve sucesso, caso contrário retorna -1 representando falha de alocação - única falha possível 
 */
int inicializaMatQuad(MatQuadraticaSL *mat, int dimensao);


/* Libera memória alocada dinâmicamente */
void finalizaMatQuad(MatQuadraticaSL *mat);


/* Gera matriz de derivdas parciais do SNL sistema e armazena em mat 
 * mat deve ser previamente alocado
 */
void geraMatDerivParcial(void **diagPrincipal, SNL sistema);

void geraMatTridiagonalDerivParcial(void ****diagonais, SNL sistema);

/* Realiza retro-substituição do SL representado por mat e armazena o resultado em x 
 * A matriz de coefecientes deve triangular inferior
 */
void retrossubs(MatQuadraticaSL mat, double **x);


/* Retorna o índice da linha cujo o elemento da coluna j seja o maior dentre as colunas das linhas abaixo da linha j 
 * Deve-se tomar cuidado pois a linha não deve ser a última, pois se a linha for a última, i é o próprio pivo
 */
int econtraMaxPivo(double ***mat, int j, int n);


/* Troca a linha i pela linha j do SL representado por mat 
 * Deve-se tomar cuidado para i não ser igual a j
 */
void trocaLinhaMat(MatQuadraticaSL *mat, int i, int pivo);


/* Realiza eliminação atráves do metódo Gauss Jordan com pivoteamento parcial */
void eliminacaoGaussJordan(MatQuadraticaSL *mat);


/* Calcula matriz jacobiana de acordo com  a matDeriVParcial e valores de x e armazena em matJacobiana
 * É necessário para o cálculo o numero de funcoes (numFuncoes) e  um vetor de variaveis da equação, da forma: ["x1","x2","x3"..."xn"]
 * matJacobiana deve ser previamente alocada
 */
void calculaMatJacobiana(void **diagPrincipal, double ***matJacobiana, char **vars, int numFuncoes, double *valoresX);

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
int metodoNewtonSNL(SNL sistema, double *xAprox, double epsilon, int maxIteracoes, FILE *saida);


int metodoNewtonSNLMatTridiagonal(SNL sistema, double *xAprox, double epsilon, int maxIteracoes, FILE *saida);

#endif