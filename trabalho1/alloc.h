/* Aloca matriz de doubles em mat
 * Retorna 0 se houve sucesso, caso contrário retorna -1 */
int alocaMatrizDoubles(int lin, int col, double ***mat);

/* Aloca matriz de chars em mat
 * Retorna 0 se houve sucesso, caso contrário retorna -1 */
int alocaMatrizChars(int lin, int col, char ***mat);

/* Aloca matriz de strings (char**) em mat, sendo seus elementos nulos (NULL) 
 * Retorna 0 se houve sucesso, caso contrário retorna -1 */
int alocaMatrizStrings(int lin, int col, char ****mat);

/* Limpa memória alocada para a matriz mat */
void freeMatrizChars(char ***mat);

/* Limpa memória alocada para a matriz mat */
void freeMatrizDoubles(double ***mat);

/* Limpa memória alocada para a matriz mat
 * Recebe como parâmetro a quantidade de linha em lin e coluna em col para liberar a memória alocada para as strings */
void freeMatrizStrings(char ****mat, int lin, int col);