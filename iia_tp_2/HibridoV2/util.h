#ifndef TP_UTIL_H
#define TP_UTIL_H
#include "algoritmo.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct individual chrom, *pchrom;
typedef struct dados{
    float  valorPagar;
    int numMoedas;
    float *moedas;
}dados;

void init_dados(char *nome, dados *n);

pchrom init_pop(struct info d, struct dados problema);

void print_pop(pchrom pop, struct info d);

chrom get_best(pchrom pop, struct info d, chrom best);

void write_best(chrom x, struct info d, dados moedas);

void init_rand();

int random_l_h(int min, int max);

float rand_01();

int flip();

void deep_copy_individual(struct individual *dest, struct individual *src, int numGenes);


#endif //TP_UTIL_H

