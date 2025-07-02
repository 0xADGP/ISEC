#ifndef TP_ALGORITMO_H
#define TP_ALGORITMO_H
#define MAX_OBJ 1000		// Numero maximo de objectos
#include "util.h"
typedef struct dados dados;

// EStrutura para armazenar parametros
struct info
{
    // Tamanho da popula��o
    int     popsize;
    // Probabilidade de muta��o
    float   pm;
    // Probabilidade de recombina��o
    float   pr;
    // Tamanho do torneio para sele��o do pai da pr�xima gera��o
    int     tsize;
    // Constante para avalia��o com penaliza��o
    float   ro;
    // N�mero de objetos que se podem colocar na mochila
    int     numGenes;
    // Capacidade da mochila
    float     capacity;
    // N�mero de gera��es
    int     numGenerations;
};

// Individuo (solu��o)
typedef struct individual chrom, *pchrom;

struct individual
{
    // Solu��o (objetos que est�o dentro da mochila)
    int     *p;
    // Valor da qualidade da solu��o
    float   fitness;
    // 1 se for uma solu��o v�lida e 0 se n�o for
    int     valido;
};


void tournament(pchrom pop, struct info d, pchrom parents);

void genetic_operators(pchrom parents, struct info d, pchrom offspring, dados moedas);

void crossover(pchrom parents, struct info d, pchrom offspring, dados moedas);

void mutation(pchrom offspring, struct info d, dados moedas);

// Add these to your existing prototypes
void uniform_crossover(pchrom parents, struct info d, pchrom offspring, struct dados moedas);
void tournament_geral(pchrom pop, struct info d, pchrom parents);

void roulette_selection(pchrom pop, struct info d, pchrom parents);

void swap_mutation(pchrom offspring, struct info d, struct dados moedas);

void repair_solution(int *genes, dados problema, int numGenes);

void free_population(pchrom pop, int popsize);

#endif //TP_ALGORITMO_H

