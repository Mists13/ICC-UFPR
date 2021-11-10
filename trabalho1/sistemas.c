#include <stdio.h>
#include <stdlib.h>
#include "sistemas.h"

void inicializaSistemaNaoLinear(SistemaNaoLinear *sistema) {
    (*sistema).funcoes = NULL;
    (*sistema).matDerivParcial = NULL;
    (*sistema).aproxInicial = NULL;
}

void finalizaSistemaNaoLinear(SistemaNaoLinear *sistema) {
    if ((*sistema).funcoes[0] != NULL) {
        free((*sistema).funcoes[0]);
        free((*sistema).funcoes);
    }

    if ((*sistema).aproxInicial != NULL) {
        free((*sistema).aproxInicial);
    }

    if ((*sistema).matDerivParcial != NULL) {
        for (int i = 0; i < (*sistema).dimensao; i++) {
            for (int j = 0; j < (*sistema).dimensao; j++) {
                if (((*sistema).matDerivParcial[i][j]) != NULL){
                    free((*sistema).matDerivParcial[i][j]);
                }
            }
        }

        free((*sistema).matDerivParcial[0]);
        free((*sistema).matDerivParcial);
    }
}
