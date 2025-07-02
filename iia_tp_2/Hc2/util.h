#ifndef UTIL_H
#define UTIL_H

typedef struct {
    int numMoedas;
    float valorPagar;
    float *moedas;
} DadosProblema;

void init_rand();
void init_dados(char *nome, DadosProblema *n);
void gerasolucaoInicial(int *sol, DadosProblema problema);

#endif // UTIL_H