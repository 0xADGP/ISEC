/*
 * Utility functions for the hybrid optimization algorithm
 * Includes I/O, random number generation, and population management
 */

#include <math.h>
#include "util.h"

// File input handling function
// Reads problem parameters and coin values from file
void init_dados(char *nome, dados *n) {
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

// Random number generator initialization
void init_rand() {
    srand((unsigned)time(NULL));
}

// Binary random choice (0/1)
int flip() {
    if ((((float)rand()) / RAND_MAX) < 0.5)
        return 0;
    else
        return 1;
}

// Population initialization function
// Creates and initializes a new population with valid solutions
pchrom init_pop(struct info d, dados problema) {
    int i, j;
    pchrom indiv;

    // Alocar memória para a população
    indiv = malloc(sizeof(chrom) * d.popsize);
    if (indiv == NULL) {
        printf("Erro na alocação de memoria para a populacao\n");
        exit(1);
    }

    // Inicializar a população
    for (i = 0; i < d.popsize; i++) {
        // Alocar memória para os genes do indivíduo
        indiv[i].p = malloc(sizeof(int) * d.numGenes);
        if (indiv[i].p == NULL) {
            printf("Erro na alocacao de memoria para os genes do individuo %d\n", i);
            exit(1);
        }

        float soma = 0.0;

        // Inicializar todos os genes com zero
        for (j = 0; j < d.numGenes; j++) {
            indiv[i].p[j] = 0;
        }

        // Seleção inicial de moedas (garante limite válido de índices)
        for (j = 0; j < problema.numMoedas && j < d.numGenes; j++) {
            int max_moedas = (int)((problema.valorPagar - soma) / problema.moedas[j]);
            if (max_moedas > 0) {
                indiv[i].p[j] = random_l_h(0, max_moedas);
                soma += indiv[i].p[j] * problema.moedas[j];
            }
        }

        // Reparar solução, se necessário
        if (fabs(soma - problema.valorPagar) > 0.1) {
            repair_solution(indiv[i].p, problema, d.numGenes);
        }

        // Recalcular soma após reparação
        soma = 0.0;
        for (j = 0; j < d.numGenes; j++) {
            soma += indiv[i].p[j] * problema.moedas[j];
        }

        // Inicializar fitness e validade
        indiv[i].fitness = 0.0;
        indiv[i].valido = (fabs(soma - problema.valorPagar) <= 0.01) ? 1 : 0;
    }

    return indiv;
}

// Best solution tracking function
// Updates best solution found based on fitness and validity
chrom get_best(pchrom pop, struct info d, chrom best) {
    int i;

    for (i = 0; i < d.popsize; i++)
    {
        // Select solution with lowest fitness
        if (pop[i].valido && (best.fitness > pop[i].fitness || !best.valido))
            best = pop[i];
    }
    return best;
}

// Random integer generation within range
int random_l_h(int low, int high) {
    return low + rand() % (high - low + 1);
}

// Random float generation [0,1]
float rand_01() {
    return ((float)rand())/RAND_MAX;
}

// Solution output function
// Displays detailed information about a solution
void write_best(chrom x, struct info d, struct dados moedas) {
    float soma = 0.0;

    printf("\nMelhor Individuo: Fitness = %.2f\n", x.fitness);
    printf("\nMoedas escolhidas: ");

    for (int i = 0; i < d.numGenes; i++) {
        if (x.p[i] > 0) {
            printf("%d x %.2f ", x.p[i], moedas.moedas[i]);
            soma += x.p[i] * moedas.moedas[i];
        }
    }

    printf("\nSoma das moedas: %.2f\n", soma);
    printf("Valor a pagar: %.2f\n", moedas.valorPagar);
    printf("Diferenca: %.2f\n", fabs(soma - moedas.valorPagar));
    printf("Solucao %s.\n", (x.valido) ? "Valida" : "Invalida");
}

// Deep copy function for chromosome structures
// Creates independent copy of solution
void deep_copy_individual(struct individual *dest, struct individual *src, int numGenes) {
    dest->fitness = src->fitness;
    dest->valido = src->valido;

    // Aloca memória para o vetor de genes
    dest->p = malloc(sizeof(int) * numGenes);
    if (dest->p == NULL) {
        printf("Erro na alocação de memória em deep_copy_individual\n");
        exit(1);
    }

    // Copia os genes
    for (int i = 0; i < numGenes; i++) {
        dest->p[i] = src->p[i];
    }
}