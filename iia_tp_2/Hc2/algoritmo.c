#include "algoritmo.h"
#include "funcao.h"
#include <stdio.h>
#include <stdlib.h>

// Hill climbing algorithm
int trepaColinas(int *solInicial, DadosProblema problema, int numIter) {
  // Allocate memory for solutions
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

  float custoAtual = calculaFit(solAtual, problema.moedas, problema.numMoedas,
                                problema.valorPagar);
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