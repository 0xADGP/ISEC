#include "funcao.h"

// Calcula a qualidade de uma solu��o
//recebe as vals -> moedas existentes , vert -> nr de moedas que existem valor
float eval_individual(int sol[], struct info d, float moedas[], float valorPagar, int *valido) {
    float soma_valores = 0.0;
    int num_moedas = 0;

    for (int i = 0; i < d.numGenes; i++) {
        soma_valores += sol[i] * moedas[i];

        if (sol[i] < 0) {
            printf("Erro: Gene negativo detectado! sol[%d] = %d\n", i, sol[i]);
            exit(1);
        }

        num_moedas += sol[i];
    }

    // Much stricter validity check - must match EXACTLY
    if (fabs(soma_valores - valorPagar) < 0.001) {
        *valido = 1;
        // Directly use number of coins as fitness, lower is better
        return (float)num_moedas;
    } else {
        *valido = 0;
        // Heavily penalize invalid solutions
        return 10000.0f + (float)num_moedas + fabs(soma_valores - valorPagar) * 1000.0f;
    }
}

void evaluate(pchrom pop, struct info d, float *moedas, float valorPagar) {
    int i;

    for (i = 0; i < d.popsize; i++) {
        int valido;
        pop[i].fitness = eval_individual(pop[i].p, d, moedas, valorPagar, &valido);
        pop[i].valido = valido;
    }
}
