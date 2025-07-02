#include "algoritmo.h"
#include "funcao.h"

float calculaFit(int *sol, float *moedas, int nMoedas, float valorObjetivo);
void geraVizinho(int *solAtual, int *vizinho, int nMoedas, dados problema);
int trepaColinas(int *solInicial, dados problema, int numIter);