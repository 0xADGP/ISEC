
// ANTÓNIO DOMINGOS GONÇALVES PEDROSO 2021132042

// Trabalho Pratico Programacao - LEI-CE
// DEIS-ISEC 2020-2021

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "utils.h"

// Inicializa o gerador de numeros aleatorios.
// Esta funcao deve ser chamada apenas uma vez no inicio da execucao do programa
void initRandom(){
    srand(time(NULL));
}

//Devolve um valor inteiro aleatorio distribuido uniformemente entre [a, b]
int intUniformRnd(int a, int b){
    return a + rand()%(b-a+1);
}

/*int probEvento(float prob){
    return prob > ((float)rand()/RAND_MAX);
}


// Função main () com alguns exemplos simples de utilizacao das funcoes
int main(){

  int i;

    initRandom();   // esta função só deve ser chamada uma vez

    printf("10 valores aleatorios uniformes entre [4, 10]:\n");
    for(i=0; i<10; i++)
        printf("%d\n", intUniformRnd(4, 100));

    printf(" Probabilidade 0.25 de um evento suceder: \n");
    for(i=0; i<10; i++)
        printf("%d\n", probEvento(0.25));

    return 0;
}*/
