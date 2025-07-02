// ANTÓNIO DOMINGOS GONÇALVES PEDROSO 2021132042

// Trabalho Pratico Programacao - LEI-CE
// DEIS-ISEC 2021-2022

#include "stdio.h"
#include "stdlib.h"
#include "list.h"
#include "matdin3d.h"

// VERIFICA SE A LISTA ESTÁ VAZIA, DEVOLVE 1 NO CASO DE ESTAR VAZIA
int lista_vazia(pjogada p){
    if(p == NULL){
        return 1;
    }else{
        return 0;
    }
}

// OBTEM O JOGADOR DA ULTIMA JOGADA NA LISTA
char get_last_player(pjogada p){
    // se ultimo jogador for igual = 1 entao devolve o jogador 2 (o)
    if(p->njogador == 1){
        return 'o';
    }else{
        return 'x';
    }
}

// OBTEM A COORDENADA X DA ULTIMA JOGADA NA LISTA
int get_last_x(pjogada p){
    return p->cox;
}

// OBTEM A COORDENADA Y DA ULTIMA JOGADA NA LISTA
int get_last_y(pjogada p){
    return p->coy;
}

// OBTEM U NUMERO DA ULTIMA JOGADA NA LISTA
int get_last_jogada(pjogada p){
    return p->njogada;
}

// ATUALIZA O TABULEIRO A PARTIR DOS VALORES CONTIDOS NA LISTA
void atualiza_tabuleiro(pjogada p, char*** tab){
    char bac;
    while(p != NULL){
        if(p->njogador == 1){
            bac = 'x';
        }else{
            bac = 'o';
        }
        setPos3D(tab, p->ntab, p->cox, p->coy, bac);
        p = p->prox;
    }
}

// IMPRIME AS ULTIMAS X JOGADAS PASSADAS NO PARAMETRO NJOGADAS NA LISTA
void mostra_infoX(pjogada p, int njogadas){
    int nig = 0;
    while(p != NULL && nig<njogadas){
        printf("Jogada: %d\tJogador: %d\tNum Tabuleiro: %d\tLinha: %d\tColuna: %d\n", p->njogada, p->njogador, p->ntab+1, p->cox+1, p->coy+1);
        p = p->prox;
        nig++;
    }
}

// INSERE UM NOVO REGISTO NA LISTA
pjogada insere_inicio(pjogada p, int njogada, int njogador, int ntab, int cox, int coy){
    pjogada novo;

    if((novo = malloc(sizeof(jogada))) == NULL){
        printf("Erro na alocacao de memoria\n");
    }else{
        preenche(novo, njogada, njogador, ntab, cox, coy);
        novo->prox = p;
        p = novo;
    }

    return p;
}

// FUNÇÃO PARA AJUDAR NA INSERCAO DE UM NOVO REGISTO NA LISTA
void preenche(pjogada p, int njogada, int njogador, int ntab, int cox, int coy){
    p->njogada = njogada;
    p->njogador = njogador;
    p->ntab = ntab;
    p->cox = cox;
    p->coy = coy;
    p->prox = NULL;
}

// GRAVA A LISTA PARA UM FICHEIRO, RETORNA 1 EM CASO DE SUCESSO
int grava_lista(pjogada lista, char* nomeF){
    FILE *f;

    f = fopen(nomeF, "wb");
    if(f == NULL){
        return 0;
    }else{
        while(lista != NULL){
            fprintf(f, "%d # %d # %d # %d # %d\n", lista->njogada, lista->njogador, lista->ntab, lista->cox, lista->coy);
            lista = lista->prox;
        }
        fclose(f);
    }
    return 1;
}

// LIBERTA O ESPAÇO EM MEMORIA USADO PELA LISTA
void liberta_lista(pjogada p){
    pjogada aux;

    while(p != NULL){
        aux = p;
        p = p->prox;
        free(aux);
    }
}

// OBTEM A LISTA A PARTIR DE UM FICHEIRO
pjogada recupera_lista(char *nomeF){
    pjogada novo, aux, lista=NULL;
    FILE *f;
    jogada j;

    f = fopen(nomeF, "rb");

    if(f == NULL){
        return NULL;
    }

    while(fscanf(f, "%d # %d # %d # %d # %d", &(j.njogada), &(j.njogador), &(j.ntab), &(j.cox), &(j.coy)) != EOF){
        j.prox = NULL;
        novo = malloc(sizeof(jogada));
        if(novo == NULL){
            fclose(f);
            liberta_lista(lista);
            return NULL;
        }

        *novo = j;

        if(lista == NULL){
            lista = novo;
        }else{
            aux = lista;
            while(aux->prox != NULL){
                aux = aux->prox;
            }
            aux->prox = novo;
        }
    }
    fclose(f);

    return lista;
}



/*pjogada insere_inicio(pjogada p, int njogada, int njogador, int ntab, int cox, int coy){
    pjogada novo, aux;

    if((novo = malloc(sizeof(jogada))) == NULL){
        printf("Erro na alocacao de memoria\n");
        return p;
    }

    preenche(novo, njogada, njogador, ntab, cox, coy);

    if(p == NULL){
        p = novo;
    }else{
        aux = p;
        while(aux->prox != NULL){
            aux = aux->prox;
        }
        aux->prox = novo;
    }
    return p;
}*/

// IMPRIME A LISTA NO ECRA
/*void mostra_info(pjogada p){
    while(p != NULL){
        printf("Jogada: %d\tJogador: %d\tNum Tabuleiro: %d\tLinha: %d\tColuna: %d\n", p->njogada, p->njogador, p->ntab+1, p->cox+1, p->coy+1);
        p = p->prox;
    }
}*/