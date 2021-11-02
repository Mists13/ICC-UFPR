#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <matheval.h>

#define MAX_STR 6

double fx(double x, char *f) {
    void *g = evaluator_create(f);
    
    double result = evaluator_evaluate_x(g, x);
    printf("%0.0f ", result);
    return result;
}

double *solveLinearSystem(double diagonalsMatrix[MAX_STR][50], int k, int n, int mainDiagIndex){

    double *xValuesArray;
    double *results;
    int u, b = 0;


    // // para cada linha da matriz de diagonais
    // for(int i = 0; i < n-1; i++){   
    //     xValuesArray[i] = diagonalsMatrix[k][i];        // termo inpendente na ultima linha da matriz        

    //     for(int j = 0; j < k; j++){                 
    //         xValuesArray[i] =- diagonalsMatrix[j][i]      
    //     }
        

}



void generateLinearSystem(char functions[MAX_STR][50], int n, int k){

    double diagonalsMatrix[k+1][n+1];                                   // diagonal size is maximum k 
    int mainDiagIndex = k/2;
    int thisDiagSize = 0;
    int atBottomDiag = 0;
    
    // for each diagonal, generates an array using functions
    for(int i = 0; i < k; i++){   

        if (thisDiagSize  == n ){                                  // main diagonal

            for(int j = 0; j < thisDiagSize; j++)                  
                diagonalsMatrix[i][j] = fx(j, functions[i]);
            putchar('\n');
            atBottomDiag = 1;
            thisDiagSize--;

        }else if ( atBottomDiag ){

            for(int j = 0; j < thisDiagSize ; j++)            // bottom diagonal
                diagonalsMatrix[i][j] = fx(j, functions[i]);
            putchar('\n');            
            thisDiagSize--;

        } else {   

            thisDiagSize = n - (mainDiagIndex - i);               
            for(int j = 0; j < thisDiagSize ; j++)               // upper diagonal
                diagonalsMatrix[i][j] = fx(j, functions[i]);
            putchar('\n');
            thisDiagSize++;
            
        }
    }   
        
    // generate independent terms
    for(int j = 0; j < n ; j++)              
        diagonalsMatrix[k][j] = fx(j, functions[k]);

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
    
    generateLinearSystem(functions, n, k);
    
    
} 
