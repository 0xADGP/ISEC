#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Initialize random number generator
void init_rand() {
    srand((unsigned)time(NULL));
}

// Read problem data from file
void init_dados(char *nome, DadosProblema *n) {
    FILE *fp = fopen(nome, "r");
    if (fp == NULL) {
        printf("Erro ao abrir o arquivo: %s\n", nome);
        exit(1);
    }

    if (fscanf(fp, "%d %f", &n->numMoedas, &n->valorPagar) != 2) {
        printf("Erro ao ler numero de moedas ou valor a pagar\n");
        fclose(fp);
        exit(1);
    }

    n->moedas = malloc(sizeof(float) * n->numMoedas);
    if (n->moedas == NULL) {
        printf("Erro na alocacao de memória para moedas\n");
        fclose(fp);
        exit(1);
    }

    for (int i = 0; i < n->numMoedas; i++) {
        if (fscanf(fp, "%f", &n->moedas[i]) != 1) {
            printf("Erro ao ler valor da moeda %d\n", i);
            fclose(fp);
            free(n->moedas);
            exit(1);
        }
    }

    fclose(fp);
}

void gerasolucaoInicial(int *sol, DadosProblema problema) {
    // Initialize solution with random values between 1 and 10
    for (int i = 0; i < problema.numMoedas; i++) {
        sol[i] = 1 + (rand() % 10);  // Random number between 1 and 10
    }
}