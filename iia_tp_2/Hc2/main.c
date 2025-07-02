#include "algoritmo.h"
#include "util.h"
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
  DadosProblema problema;
  int runs;
  float melhor_soma = INFINITY;
  int melhor_resultado = INT_MAX;
  int *melhor_solucao;

  char fname[40];
  printf("\nNome Ficheiro: ");
  scanf("%s", fname);
  printf("Número de execuções: ");
  scanf("%d", &runs);

  init_dados(fname, &problema);

  // Allocate solutions
  int *solucao = malloc(sizeof(int) * problema.numMoedas);
  melhor_solucao = malloc(sizeof(int) * problema.numMoedas);

  float media = 0.0;

  for (int r = 0; r < runs; r++) {
    // Generate initial solution
    gerasolucaoInicial(solucao, problema);

    // Run hill climbing
    int resultado = trepaColinas(solucao, problema, 500);

    // Calculate total
    float soma = 0.0;
    for (int i = 0; i < problema.numMoedas; i++) {
      soma += solucao[i] * problema.moedas[i];
    }

    // Update best solution if better
    if (resultado < melhor_resultado) {
      melhor_resultado = resultado;
      melhor_soma = soma;
      memcpy(melhor_solucao, solucao, sizeof(int) * problema.numMoedas);
    }

    printf("\nExecução %d - Custo: %d\n", r + 1, resultado);
    printf("Moeda | Quantidade\n");
    for (int i = 0; i < problema.numMoedas; i++) {
      printf("%.2f | %d\n", problema.moedas[i], solucao[i]);
    }
    media += resultado;
  }

  // Print best results
  printf("\nMelhor solução encontrada:\n");
  for (int i = 0; i < problema.numMoedas; i++) {
    if (melhor_solucao[i] > 0) {
      printf("%d moedas de %.2f euros\n", melhor_solucao[i],
             problema.moedas[i]);
    }
  }
  printf("\nMelhor soma total: %.2f euros\n", melhor_soma);
  printf("Média dos custos: %.2f\n", media / runs);
  printf("Melhor custo: %d\n", melhor_resultado);

  // Free memory
  free(solucao);
  free(melhor_solucao);
  free(problema.moedas);

  return 0;
}