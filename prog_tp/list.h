// ANTÓNIO DOMINGOS GONÇALVES PEDROSO 2021132042

// Trabalho Pratico Programacao - LEI-CE
// DEIS-ISEC 2021-2022

#ifndef PROJETO_LIST_H
#define PROJETO_LIST_H

typedef struct dados jogada, *pjogada;

struct dados{
    int njogada;
    int njogador;
    int ntab;
    int cox;
    int coy;
    pjogada prox;
};

// VERIFICA SE A LISTA ESTÁ VAZIA, DEVOLVE 1 NO CASO DE ESTAR VAZIA
int lista_vazia(pjogada p);

// OBTEM O JOGADOR DA ULTIMA JOGADA NA LISTA
char get_last_player(pjogada p);

// OBTEM A COORDENADA X DA ULTIMA JOGADA NA LISTA
int get_last_x(pjogada p);

// OBTEM A COORDENADA Y DA ULTIMA JOGADA NA LISTA
int get_last_y(pjogada p);

// OBTEM U NUMERO DA ULTIMA JOGADA NA LISTA
int get_last_jogada(pjogada p);

// ATUALIZA O TABULEIRO A PARTIR DOS VALORES CONTIDOS NA LISTA
void atualiza_tabuleiro(pjogada p, char*** tab);

// IMPRIME AS ULTIMAS X JOGADAS PASSADAS NO PARAMETRO NJOGADAS NA LISTA
void mostra_infoX(pjogada p, int njogadas);

// INSERE UM NOVO REGISTO NA LISTA
pjogada insere_inicio(pjogada p, int njogada, int njogador, int ntab, int cox, int coy);

// FUNÇÃO PARA AJUDAR NA INSERCAO DE UM NOVO REGISTO NA LISTA
void preenche(pjogada p, int njogada, int njogador, int ntab, int cox, int coy);

// GRAVA A LISTA PARA UM FICHEIRO, RETORNA 1 EM CASO DE SUCESSO
int grava_lista(pjogada lista, char* nomeF);

// LIBERTA O ESPAÇO EM MEMORIA USADO PELA LISTA
void liberta_lista(pjogada p);

// OBTEM A LISTA A PARTIR DE UM FICHEIRO
pjogada recupera_lista(char *nomeF);

// IMPRIME A LISTA NO ECRA
//void mostra_info(pjogada p);

#endif //PROJETO_LIST_H
