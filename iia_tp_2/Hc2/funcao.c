#include "funcao.h"
#include <math.h>
#include <stdlib.h>

float calculaFit(int *sol, float *moedas, int nMoedas, float valorObjetivo)
{
  float soma_valores = 0.0;
  int num_moedas = 0;

  for (int i = 0; i < nMoedas; i++)
  {
    soma_valores += sol[i] * moedas[i];
    num_moedas += sol[i];
  }

  // If solution exactly matches target, return number of coins
  if (fabs(soma_valores - valorObjetivo) < 0.001)
  {
    return (float)num_moedas;
  }
  else
  {
    // Heavily penalize invalid solutions, but prioritize minimizing coins
    return 10000.0f * fabs(soma_valores - valorObjetivo) + (float)num_moedas;
  }
}

void geraVizinho(int *solAtual, int *vizinho, int nMoedas, DadosProblema problema) {
    // Copy current solution
    for (int i = 0; i < nMoedas; i++) {
        vizinho[i] = solAtual[i];
    }
    
    // Randomly choose modification strategy
    //int estrategia = rand() % 3;
    int estrategia = 0;
    
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