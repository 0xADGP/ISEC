
// ANTÓNIO DOMINGOS GONÇALVES PEDROSO 2021132042

// Trabalho Pratico Programacao - LEI-CE
// DEIS-ISEC 2021-2022

#ifndef MATDIN_H
#define MATDIN_H

#include <stdlib.h>
#include <stdio.h>

// Liberta uma matriz dinâmica de caracteres com nLin linhas
void libertaMat(char** p, int nLin);

// Cria uma matriz dinâmica de caracteres  com nLin linhas e nCol colunas
// Devolve endereço inicial da matriz
char** criaMat(int nLin, int nCol);

// Coloca o caracter c na posição (x, y) de uma matriz dinâmica de caracteres
void setPos(char **p, int x, int y, char c);

// Obtém o caracter armazenado na posição (x, y) de uma matriz de caracteres
char getPos(char **p, int x, int y);

// Imprime o conteúdo de uma matriz dinâmica de caracteres
void mostraMat(char **p, int nLin, int nCol);

#endif /* MATDIN_H */

