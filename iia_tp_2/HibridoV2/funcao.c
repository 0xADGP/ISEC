#include "funcao.h"

// Evaluates the quality of a solution for the coin change problem
// Parameters:
// sol[] - array representing the solution (number of each coin type)
// d - structure containing problem information
// moedas[] - array of coin values
// valorPagar - target amount to pay
// valido - pointer to store solution validity (1 if valid, 0 if invalid)
// Returns: fitness value (lower is better)
float eval_individual(int sol[], struct info d, float moedas[], float valorPagar, int *valido) {
    // Initialize sum of coin values and total number of coins
    float soma_valores = 0.0;
    int num_moedas = 0;

    // Calculate total value and count coins
    for (int i = 0; i < d.numGenes; i++) {
        // Add value contribution of current coin type
        soma_valores += sol[i] * moedas[i];

        // Sanity check for negative coin counts
        if (sol[i] < 0) {
            printf("Erro: Gene negativo detectado! sol[%d] = %d\n", i, sol[i]);
            exit(1);
        }

        // Count total number of coins used
        num_moedas += sol[i];
    }

    // Check if solution matches target amount (using very small epsilon for floating point comparison)
    if (fabs(soma_valores - valorPagar) < 0.001) {
        *valido = 1;
        // For valid solutions, fitness is simply the number of coins (minimize)
        return (float)num_moedas;
    } else {
        *valido = 0;
        // Invalid solutions are heavily penalized:
        // 10000 base penalty
        // Plus number of coins used
        // Plus scaled difference from target amount
        return 10000.0f + (float)num_moedas + fabs(soma_valores - valorPagar) * 1000.0f;
    }
}

// Evaluates entire population by calculating fitness for each individual
// Parameters:
// pop - array of chromosomes (population)
// d - structure containing problem information
// moedas - array of coin values
// valorPagar - target amount to pay
void evaluate(pchrom pop, struct info d, float *moedas, float valorPagar) {
    // Evaluate each individual in the population
    for (int i = 0; i < d.popsize; i++) {
        int valido;
        // Calculate fitness and validity for current individual
        pop[i].fitness = eval_individual(pop[i].p, d, moedas, valorPagar, &valido);
        pop[i].valido = valido;
    }
}
