// ANTÓNIO DOMINGOS GONÇALVES PEDROSO 2021132042

// Trabalho Pratico Programacao - LEI-CE
// DEIS-ISEC 2021-2022

#ifndef PROJETO_JOGOFUNC_H
#define PROJETO_JOGOFUNC_H

#include <stdlib.h>

// VERIFICA SE ALGUM JOGADOR GANHOU O JOGO, RETORNA x PARA O JOGADOR1,
// o PARA O JOGADOR 2, d EM CASO DE EMPATE E n NO CASO DO JOGO AINDA NÃO ESTAR ACABADO
char checkWinner(char **p, char ***tab);

// FUNCAO PARA OBTER O TABULEIRO SELECIONADO PARA A PROXIMA JOGADA
int updateTab(char **p, int x, int y);

// ATUALIZA O TABULEIRO GRANDE DE ACORDO COM OS TABULEIROS PEQUENOS
void updateTabGrande(char ***tab, char **p);

// FAZ UMA JOGADA ALEATORIA NO TABULEIRO SELECIONADO
void randomPlay(char ***tab, int curr, int* x, int* y);

#endif //PROJETO_JOGOFUNC_H
