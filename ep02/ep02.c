#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <matheval.h>

#define MAX_STR 5

double fx(double x, char *f) {
    void *g = evaluator_create(f);
    
    double result = evaluator_evaluate_x(g, x);
    printf("%0.0f ", result);
    return result;
}

void generateLinearSystem(char functions[MAX_STR][50], int n, int k){

    double upperDiagonals[k];                                   // diagonal size is maximum k 
    double bottomDiagonals[k];
    double mainDiagonal[k];
    int mainDiag = k/2;
    int thisDiagSize = 0;

    // generate upper diagonals 
    for(int i = 0; i < mainDiag; i++){                       
        thisDiagSize = n - (mainDiag - i);               
        for(int j = 0; j < thisDiagSize ; j++)               // for each function, generates array of diagonal
            upperDiagonals[j] = fx(j, functions[i]);
            putchar('\n');
    }   

    // generate bottom diagonals
    thisDiagSize++;
    for(int i = mainDiag; i < k ; i++){    

        if (thisDiagSize  == n ){                           // main diagonal
            for(int j = 0; j < thisDiagSize; j++)                  
            mainDiagonal[j] = fx(j, functions[i]);
            putchar('\n');
        } else {   
        for(int j = 0; j < thisDiagSize ; j++)                  // for each function, generates array of diagonal
            bottomDiagonals[j] = fx(j, functions[i]);
            putchar('\n');
        }
        thisDiagSize--;
    }                


    /*
        GERAR TERMOS INDEPENTENTES
    */

}



int main() {

    int n, k, i, end = 0;

    char functions[MAX_STR][50] = { "" };
    char independentTerms[50];

    for (i = 0; i < MAX_STR; i++) {        // for a max of NWDS
        scanf("%[^\n]%*c", functions[i]);
        printf (" function[%d] : %s\n", i, functions[i]);
    }
    // Dimension (n) and diagonal amount (k) of linear system
    printf("Type n and k");
    scanf("%d", &n);
    scanf("%d", &k);

    scanf("%[^\n]%*c", independentTerms);
    generateLinearSystem(functions, n, k);
    
} 
