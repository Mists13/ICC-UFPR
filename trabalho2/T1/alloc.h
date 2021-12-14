/*
    Luzia Millena Santos Silva, GRR20185174, 	lmss18@inf.ufpr.br
    Matheus Pacheco Santos,     GRR20192786,	mps19@inf.ufpr.br
*/
#ifndef __ALLOC_H__
#define __ALLOC_H__

/* Aloca matriz de doubles em mat
 * Retorna 0 se houve sucesso, caso contrário retorna -1 representando falha de alocação - única falha possível 
 */
int alocaMatDoubles(int lin, int col, double ***mat);

/* Aloca matriz de chars em mat
 * Retorna 0 se houve sucesso, caso contrário retorna -1 representando falha de alocação - única falha possível 
 */
int alocaMatChars(int lin, int col, char ***mat);

/* Aloca matrizs de void* em mat
 * Retorna 0 se houve sucesso, caso contrário retorna -1 representando falha de alocação - única falha possível 
 */
int alocaMatPonteirosVoids(int lin, int col, void ****mat);

/* Limpa memória alocada para a matriz mat */
void freeMatChars(char ***mat);

/* Limpa memória alocada para a matriz mat */
void freeMatDoubles(double ***mat);

/* Limpa memória alocada para a matriz mat */
void freeMatPonteirosVoids(void ****mat, int lin, int col);

#endif