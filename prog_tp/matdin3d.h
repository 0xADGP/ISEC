// ANTÓNIO DOMINGOS GONÇALVES PEDROSO 2021132042

// Trabalho Pratico Programacao - LEI-CE
// DEIS-ISEC 2021-2022

#ifndef PROJETO_MATDIN3D_H
#define PROJETO_MATDIN3D_H

#include <stdlib.h>
#include <stdio.h>

// IMPRIME O JOGO NO ECRA
void mostraJogo(char ***p);
//void mostraJogoV2(char ***p);

// CRIA UMA MATRIZ 3D COM O NLIN E NCOL;
char*** criaMat3D(int nTab, int nLin, int nCol);

// MUDA O VALOR DE UMA POSICAO PASSADA POR PARAMETROS NA MATRIZ 3D
void setPos3D(char ***p, int tab, int x, int y, char c);

// OBTEM O VALOR DE UMA POSICAO PASSADA POR PARAMETROS DA MATRIZ 3D
char getPos3D(char ***p, int tab, int x, int y);

//void mostraJogoV2(char ***p);

#endif //PROJETO_MATDIN3D_H
