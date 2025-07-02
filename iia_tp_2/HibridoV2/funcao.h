#ifndef TP_FUNCAO_H
#define TP_FUNCAO_H
#include "math.h"
#include "stdio.h"
#include "algoritmo.h"
#define TOLERANCIA 0.1
float eval_individual(int sol[], struct info d, float moedas[], float valorPagar, int *valido);
void evaluate(pchrom pop, struct info d, float *moedas, float valorPagar);


#endif //TP_FUNCAO_H
