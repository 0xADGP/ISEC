#ifndef FUNCAO_H
#define FUNCAO_H

#include "util.h"

float calculaFit(int *sol, float *moedas, int nMoedas, float valorObjetivo);
void geraVizinho(int *solAtual, int *vizinho, int nMoedas, DadosProblema problema);

#endif // FUNCAO_H