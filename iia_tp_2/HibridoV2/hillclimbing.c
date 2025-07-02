/*
 * Implementation of hill climbing algorithm for coin selection optimization
 * Contains functions for fitness calculation, neighbor generation, and local search
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include "hillclimbing.h"

// Calculates fitness of a solution
// Parameters:
// - sol: array of coin quantities
// - moedas: array of coin values
// - nMoedas: number of different coin types
// - valorObjetivo: target sum to achieve
// Returns: fitness value (lower is better)
float calculaFit(int *sol, float *moedas, int nMoedas, float valorObjetivo) {
    float soma_valores = 0.0;
    int num_moedas = 0;

    for (int i = 0; i < nMoedas; i++) {
        soma_valores += sol[i] * moedas[i];
        num_moedas += sol[i];
    }

    // If solution exactly matches target, return number of coins
    if (fabs(soma_valores - valorObjetivo) < 0.001) {
        return (float)num_moedas;
    } else {
        // Heavily penalize invalid solutions
        return 10000.0f + (float)num_moedas + fabs(soma_valores - valorObjetivo) * 1000.0f;
    }
}

// Generates a neighbor solution by applying random modifications
// Parameters:
// - solAtual: current solution
// - vizinho: neighbor solution (output)
// - nMoedas: number of coin types
// - problema: problem data structure
void geraVizinho(int *solAtual, int *vizinho, int nMoedas, dados problema) {
    // Copy current solution
    for (int i = 0; i < nMoedas; i++) {
        vizinho[i] = solAtual[i];
    }
    
    // Randomly choose modification strategy
    int estrategia = rand() % 3;
    
    switch(estrategia) {
        case 0: // Increment a random coin type
        {
            int idx = rand() % nMoedas;
            vizinho[idx]++;
            break;
        }
        case 1: // Decrement a random coin type (if possible)
        {
            int idx = rand() % nMoedas;
            if (vizinho[idx] > 0) {
                vizinho[idx]--;
            }
            break;
        }
        case 2: // Swap coin quantities
        {
            int idx1 = rand() % nMoedas;
            int idx2 = rand() % nMoedas;
            if (idx1 != idx2) {
                int temp = vizinho[idx1];
                vizinho[idx1] = vizinho[idx2];
                vizinho[idx2] = temp;
            }
            break;
        }
    }
    
    // Repair solution to ensure it doesn't exceed target
    float soma = 0.0;
    for (int i = 0; i < nMoedas; i++) {
        soma += vizinho[i] * problema.moedas[i];
    }
    
    // If solution exceeds target, reduce coins
    while (soma > problema.valorPagar + 0.01) {
        // Find coin to reduce
        for (int i = nMoedas - 1; i >= 0; i--) {
            if (vizinho[i] > 0) {
                vizinho[i]--;
                soma -= problema.moedas[i];
                break;
            }
        }
    }
}

// Main hill climbing algorithm implementation
// Parameters:
// - solInicial: initial/current solution (modified in place)
// - problema: problem data structure
// - numIter: number of iterations to perform
// Returns: best fitness value found
int trepaColinas(int *solInicial, dados problema, int numIter) {
    // Memory allocation for working solutions
    int *solAtual = malloc(sizeof(int) * problema.numMoedas);
    int *melhorSol = malloc(sizeof(int) * problema.numMoedas);
    int *vizinho = malloc(sizeof(int) * problema.numMoedas);
    
    if (!solAtual || !melhorSol || !vizinho) {
        printf("Erro na alocação de memória\n");
        exit(1);
    }
    
    // Copy initial solution
    for (int i = 0; i < problema.numMoedas; i++) {
        solAtual[i] = solInicial[i];
        melhorSol[i] = solInicial[i];
    }
    
    float custoAtual = calculaFit(solAtual, problema.moedas, problema.numMoedas, problema.valorPagar);
    float melhorCusto = custoAtual;
    
    // Main optimization loop
    for (int iter = 0; iter < numIter; iter++) {
        // Multiple neighbor exploration per iteration
        for (int j = 0; j < problema.numMoedas * 2; j++) {
            // Generate and evaluate neighbor solution
            geraVizinho(solAtual, vizinho, problema.numMoedas, problema);
            
            // Calculate fitness of neighbor
            float custoVizinho = calculaFit(vizinho, problema.moedas, 
                                            problema.numMoedas, problema.valorPagar);
            
            // Update if better solution found
            if (custoVizinho < melhorCusto) {
                // Copy neighbor to current solution
                for (int k = 0; k < problema.numMoedas; k++) {
                    solAtual[k] = vizinho[k];
                    melhorSol[k] = vizinho[k];
                }
                melhorCusto = custoVizinho;
            }
        }
    }
    
    // Update original solution with best found
    for (int i = 0; i < problema.numMoedas; i++) {
        solInicial[i] = melhorSol[i];
    }
    
    // Free allocated memory
    free(solAtual);
    free(melhorSol);
    free(vizinho);
    
    return melhorCusto;
}

