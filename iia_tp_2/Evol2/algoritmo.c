#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "algoritmo.h"
#include "funcao.h"

void tournament(pchrom pop, struct info d, pchrom parents) {
    for (int i = 0; i < d.popsize; i++) {
        int x1 = random_l_h(0, d.popsize - 1);
        int x2;
        do {
            x2 = random_l_h(0, d.popsize - 1);
        } while (x1 == x2);

        struct individual *winner = (pop[x1].fitness < pop[x2].fitness) ? &pop[x1] : &pop[x2];
        deep_copy_individual(&parents[i], winner, d.numGenes);
    }
}

void tournament_geral(pchrom pop, struct info d, pchrom parents) {
    int i, j, k, sair, best, *pos;
    
    pos = malloc(d.tsize * sizeof(int));
    if (pos == NULL) {
        fprintf(stderr, "Memory allocation error in tournament_geral\n");
        exit(1);
    }

    for (i = 0; i < d.popsize; i++) {
        for (j = 0; j < d.tsize; j++) {
            do {
                pos[j] = random_l_h(0, d.popsize - 1);
                sair = 0;
                for (k = 0; k < j; k++) {
                    if (pos[k] == pos[j]) {
                        sair = 1;
                        break;
                    }
                }
            } while (sair);

            if (j == 0 || pop[pos[j]].fitness < pop[pos[best]].fitness) {
                best = j;
            }
        }
        deep_copy_individual(&parents[i], &pop[pos[best]], d.numGenes);
    }

    free(pos);
}

void roulette_selection(pchrom pop, struct info d, pchrom parents) {
    int i;
    float total_inverse_fitness = 0.0;
    float *probabilities;
    float random_value, sum;
    float *inverse_fitness;
    
    probabilities = (float *)malloc(d.popsize * sizeof(float));
    inverse_fitness = (float *)malloc(d.popsize * sizeof(float));
    if (probabilities == NULL || inverse_fitness == NULL) {
        fprintf(stderr, "Memory allocation error in roulette_selection\n");
        exit(1);
    }
    
    // Calculate inverse fitness values
    for (i = 0; i < d.popsize; i++) {
        inverse_fitness[i] = 1.0 / pop[i].fitness;
        total_inverse_fitness += inverse_fitness[i];
    }
    
    // Calculate selection probabilities using inverse fitness
    for (i = 0; i < d.popsize; i++) {
        probabilities[i] = inverse_fitness[i] / total_inverse_fitness;
    }
    
    // Select individuals
    for (i = 0; i < d.popsize; i++) {
        random_value = rand_01();
        sum = 0.0;
        
        for (int j = 0; j < d.popsize; j++) {
            sum += probabilities[j];
            if (random_value <= sum) {
                deep_copy_individual(&parents[i], &pop[j], d.numGenes);
                break;
            }
        }
    }
    
    free(probabilities);
    free(inverse_fitness);
}

void genetic_operators(pchrom parents, struct info d, pchrom offspring, struct dados moedas) {
    for (int i = 0; i < d.popsize; i++) {
        offspring[i].p = malloc(sizeof(int) * d.numGenes);
        if (offspring[i].p == NULL) {
            fprintf(stderr, "Erro ao alocar memória para offspring[%d].p\n", i);
            exit(1);
        }
    }

    crossover(parents, d, offspring, moedas);
    //uniform_crossover(parents, d, offspring, moedas);
    //mutation(offspring, d, moedas);
    swap_mutation(offspring, d, moedas);

    // Randomly choose crossover method
    /*if (rand_01() < 0.5) {
        crossover(parents, d, offspring, moedas);
    } else {
        uniform_crossover(parents, d, offspring, moedas);
    }*/

    // Randomly choose mutation method
    /*if (rand_01() < 0.5) {
        mutation(offspring, d, moedas);
    } else {
        swap_mutation(offspring, d, moedas);
    }*/
}

void crossover(pchrom parents, struct info d, pchrom offspring, struct dados moedas) {
    for (int i = 0; i < d.popsize; i += 2) {
        if (rand_01() < d.pr) {
            int point = random_l_h(0, d.numGenes - 1);
            for (int j = 0; j < d.numGenes; j++) {
                if (j < point) {
                    offspring[i].p[j] = parents[i].p[j];
                    offspring[i + 1].p[j] = parents[i + 1].p[j];
                } else {
                    offspring[i].p[j] = parents[i + 1].p[j];
                    offspring[i + 1].p[j] = parents[i].p[j];
                }
            }
        } else {
            for (int j = 0; j < d.numGenes; j++) {
                offspring[i].p[j] = parents[i].p[j];
                offspring[i + 1].p[j] = parents[i + 1].p[j];
            }
        }

        repair_solution(offspring[i].p, moedas, d.numGenes);
        repair_solution(offspring[i + 1].p, moedas, d.numGenes);
    }
}

void mutation(pchrom offspring, struct info d, struct dados moedas) {
    for (int i = 0; i < d.popsize; i++) {
        for (int j = 0; j < d.numGenes; j++) {
            if (rand_01() < d.pm) {
                offspring[i].p[j] = !(offspring[i].p[j]);
            }
        }
        repair_solution(offspring[i].p, moedas, d.numGenes);
    }
}

// Uniform crossover - each gene has a 50% chance of being swapped
void uniform_crossover(pchrom parents, struct info d, pchrom offspring, struct dados moedas) {
    for (int i = 0; i < d.popsize; i += 2) {
        if (rand_01() < d.pr) {
            for (int j = 0; j < d.numGenes; j++) {
                // 50% chance to swap genes
                if (rand_01() < 0.5) {
                    offspring[i].p[j] = parents[i + 1].p[j];
                    offspring[i + 1].p[j] = parents[i].p[j];
                } else {
                    offspring[i].p[j] = parents[i].p[j];
                    offspring[i + 1].p[j] = parents[i + 1].p[j];
                }
            }
        } else {
            // If not performing crossover, copy parents directly
            for (int j = 0; j < d.numGenes; j++) {
                offspring[i].p[j] = parents[i].p[j];
                offspring[i + 1].p[j] = parents[i + 1].p[j];
            }
        }

        // Repair solutions after crossover
        repair_solution(offspring[i].p, moedas, d.numGenes);
        repair_solution(offspring[i + 1].p, moedas, d.numGenes);
    }
}



// Swap mutation - randomly swap two gene positions
void swap_mutation(pchrom offspring, struct info d, struct dados moedas) {
    for (int i = 0; i < d.popsize; i++) {
        if (rand_01() < d.pm) {
            // Randomly select two different gene positions to swap
            int pos1 = random_l_h(0, d.numGenes - 1);
            int pos2;
            do {
                pos2 = random_l_h(0, d.numGenes - 1);
            } while (pos1 == pos2);

            // Swap the genes at these positions
            int temp = offspring[i].p[pos1];
            offspring[i].p[pos1] = offspring[i].p[pos2];
            offspring[i].p[pos2] = temp;
        }
        
        // Repair the solution after potential mutation
        repair_solution(offspring[i].p, moedas, d.numGenes);
    }
}

void repair_solution(int *genes, dados problema, int numGenes) {
    float soma = 0.0;
    for (int i = 0; i < numGenes; i++) {
        soma += genes[i] * problema.moedas[i];
    }

    // Mostrar a soma antes da reparação para depuração
   // printf("Soma inicial antes de reparacao: %.6f\n", soma);

    // Reparar só se a diferença for significativa
    if (fabs(soma - problema.valorPagar) > 0.01) {
        int ajustes = 0;
        while (ajustes < 1000 && fabs(soma - problema.valorPagar) > 0.001) {
            if (soma > problema.valorPagar) {
                for (int i = numGenes - 1; i >= 0; i--) {
                    if (genes[i] > 0) {
                        genes[i]--;
                        soma -= problema.moedas[i];
                        ajustes++;
                        break;
                    }
                }
            } else {
                for (int i = 0; i < numGenes; i++) {
                    if (soma + problema.moedas[i] <= problema.valorPagar + 0.001) {
                        genes[i]++;
                        soma += problema.moedas[i];
                        ajustes++;
                        break;
                    }
                }
            }
        }
    }
}


void free_population(pchrom pop, int popsize) {
    for (int i = 0; i < popsize; i++) {
        if (pop[i].p != NULL) {
            free(pop[i].p);
            pop[i].p = NULL;
        }
    }
}