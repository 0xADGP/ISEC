// ANTÓNIO DOMINGOS GONÇALVES PEDROSO 2021132042

// Trabalho Pratico Programacao - LEI-CE
// DEIS-ISEC 2021-2022

#include "matdin3d.h"

#include <stdio.h>

// IMPRIME O JOGO NO ECRA
void mostraJogo(char ***p){
    printf("\nTabuleiro: \n");
    int aux = 0; // aux para verificar quando é que a função salta para o tabuleiro 10 (nao existe)
    int jIni = 0;
    int jFim = 3;

    for(int i=0; i<3; i++){
        if(aux == 9){
            return; // sair no caso do tabuleiro nao existir
        }
        printf("\t");
        for(int j=jIni; j<jFim; j++){
            for(int k = 0; k<3; k++){
                printf("%c ", p[j][i][k]);
            }
            printf("\t");
        }
        if(i == 2){
            i = -1;
            jIni = jFim;
            jFim += 3;
            printf("\n");
        }
        printf("\n");
        aux++;
    }
}

// LIBERTA A MATRIZ 3D
void libertaMat3D(char*** nTab, int nLin, int nCol){
    for(int i=0; i<nLin; i++){
        for(int j=0; j<nCol; j++){
            free(nTab[i][j]);
        }
        free(nTab[i]);
    }
    free(nTab);
}

// CRIA UMA MATRIZ 3D COM O NLIN E NCOL;
char*** criaMat3D(int nTab, int nLin, int nCol){
    char ***p = NULL;

    p = malloc(sizeof(char **) * nTab);
    if(p == NULL)
        return NULL;

    for(int i=0; i<nTab; i++){
        p[i] = malloc(sizeof(char*) * nLin);
        for(int j=0; j<nLin; j++){
            p[i][j] = malloc(sizeof(char) * nCol);

            if(p[i][j] == NULL){
                libertaMat3D(p, i-1, j-1);
                return NULL;
            }

            for(int a=0; a<nCol; a++){
                p[i][j][a] = '-';
            }
        }
    }

    return p;
}

// MUDA O VALOR DE UMA POSICAO PASSADA POR PARAMETROS NA MATRIZ 3D
void setPos3D(char ***p, int tab, int x, int y, char c){
    p[tab][x][y] = c;
}

// OBTEM O VALOR DE UMA POSICAO PASSADA POR PARAMETROS DA MATRIZ 3D
char getPos3D(char ***p, int tab, int x, int y){
    return p[tab][x][y];
}



/*
void mostraJogoV2(char ***p){
    printf("Tabuleiro:\n");
    printf("\t%c ", p[0][0][0]);
    printf("%c ", p[0][0][1]);
    printf("%c\t", p[0][0][2]);

    printf("%c ", p[1][0][0]);
    printf("%c ", p[1][0][1]);
    printf("%c\t", p[1][0][2]);

    printf("%c ", p[2][0][0]);
    printf("%c ", p[2][0][1]);
    printf("%c\n", p[2][0][2]);


    printf("\t%c ", p[0][1][0]);
    printf("%c ", p[0][1][1]);
    printf("%c\t", p[0][1][2]);

    printf("%c ", p[1][1][0]);
    printf("%c ", p[1][1][1]);
    printf("%c\t", p[1][1][2]);

    printf("%c ", p[2][1][0]);
    printf("%c ", p[2][1][1]);
    printf("%c\n", p[2][1][2]);


    printf("\t%c ", p[0][2][0]);
    printf("%c ", p[0][2][1]);
    printf("%c\t", p[0][2][2]);

    printf("%c ", p[1][2][0]);
    printf("%c ", p[1][2][1]);
    printf("%c\t", p[1][2][2]);

    printf("%c ", p[2][2][0]);
    printf("%c ", p[2][2][1]);
    printf("%c\n\n", p[2][2][2]);

    //nn

    printf("\t%c ", p[3][0][0]);
    printf("%c ", p[3][0][1]);
    printf("%c\t", p[3][0][2]);

    printf("%c ", p[4][0][0]);
    printf("%c ", p[4][0][1]);
    printf("%c\t", p[4][0][2]);

    printf("%c ", p[5][0][0]);
    printf("%c ", p[5][0][1]);
    printf("%c\n", p[5][0][2]);


    printf("\t%c ", p[3][1][0]);
    printf("%c ", p[3][1][1]);
    printf("%c\t", p[3][1][2]);

    printf("%c ", p[4][1][0]);
    printf("%c ", p[4][1][1]);
    printf("%c\t", p[4][1][2]);

    printf("%c ", p[5][1][0]);
    printf("%c ", p[5][1][1]);
    printf("%c\n", p[5][1][2]);


    printf("\t%c ", p[3][2][0]);
    printf("%c ", p[3][2][1]);
    printf("%c\t", p[3][2][2]);

    printf("%c ", p[4][2][0]);
    printf("%c ", p[4][2][1]);
    printf("%c\t", p[4][2][2]);

    printf("%c ", p[5][2][0]);
    printf("%c ", p[5][2][1]);
    printf("%c\n\n", p[5][2][2]);

    // nn

    printf("\t%c ", p[6][0][0]);
    printf("%c ", p[6][0][1]);
    printf("%c\t", p[6][0][2]);

    printf("%c ", p[7][0][0]);
    printf("%c ", p[7][0][1]);
    printf("%c\t", p[7][0][2]);

    printf("%c ", p[8][0][0]);
    printf("%c ", p[8][0][1]);
    printf("%c\n", p[8][0][2]);


    printf("\t%c ", p[6][1][0]);
    printf("%c ", p[6][1][1]);
    printf("%c\t", p[6][1][2]);

    printf("%c ", p[7][1][0]);
    printf("%c ", p[7][1][1]);
    printf("%c\t", p[7][1][2]);

    printf("%c ", p[8][1][0]);
    printf("%c ", p[8][1][1]);
    printf("%c\n", p[8][1][2]);


    printf("\t%c ", p[6][2][0]);
    printf("%c ", p[6][2][1]);
    printf("%c\t", p[6][2][2]);

    printf("%c ", p[7][2][0]);
    printf("%c ", p[7][2][1]);
    printf("%c\t", p[7][2][2]);

    printf("%c ", p[8][2][0]);
    printf("%c ", p[8][2][1]);
    printf("%c\n\n", p[8][2][2]);

    // nn
}*/