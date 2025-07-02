// ANTÓNIO DOMINGOS GONÇALVES PEDROSO 2021132042

// Trabalho Pratico Programacao - LEI-CE
// DEIS-ISEC 2021-2022

#include "jogofunc.h"
#include "matdin.h"
#include "matdin3d.h"
#include "utils.h"

/*
int checkWinner(char **p){
    int accx1, accx2, accy1, accy2, accxy1, accxy2;

    accxy1 = 0;
    accxy2 = 0;
    for(int i=0; i<3; i++){
        accx1 = 0;
        accx2 = 0;
        accy1 = 0;
        accy2 = 0;
        for(int j=0; j<3; j++){
            if(getPos(p, i, j) == 'x'){
                accx1++;
            }else if(getPos(p, i, j) == 'o'){
                accx2++;
            }

            if(getPos(p, j, i) == 'x'){
                accy1++;
            }else if(getPos(p, j, i) == 'o'){
                accy2++;
            }
        }

        if(accx1 || accy1 == 3){
            return 1;
        }else if(accy1 || accy2 == 3){
            return 2;
        }

        if(getPos(p, i, i) == 'x'){
            accxy1++;
        }else if(getPos(p, i, i) == 'o'){
            accxy2++;
        }
    }

    char aux;
    if(getPos(p,0,2) == getPos(p,1,1) && getPos(p,1,1) == getPos(p,2,0)){
        aux = getPos(p,0,2);
        if(aux == 'x'){
            accxy1 = 3;
        }else if(aux == 'o'){
            accxy2 = 3;
        }
    }

    if(accxy1 == 3){
        return 1;
    }else if(accxy2 == 3){
        return 2;
    }

    return 0;
}
*/

// VERIFICA SE ALGUM JOGADOR GANHOU O JOGO, RETORNA x PARA O JOGADOR1,
// o PARA O JOGADOR 2, d EM CASO DE EMPATE E n NO CASO DO JOGO AINDA NÃO ESTAR ACABADO
char checkWinner(char **p, char ***tab){
    if(p[0][0] != '-' && p[0][0] == p[0][1] && p[0][0] == p[0][2]){ return getPos(p, 0, 0); }
    if(p[1][0] != '-' && p[1][0] == p[1][1] && p[1][0] == p[1][2]){ return getPos(p, 1, 0); }
    if(p[2][0] != '-' && p[2][0] == p[2][1] && p[2][0] == p[2][2]){ return getPos(p, 2, 0); }

    if(p[0][0] != '-' && p[0][0] == p[1][0] && p[0][0] == p[2][0]){ return getPos(p, 0, 0); }
    if(p[0][1] != '-' && p[0][1] == p[1][1] && p[0][1] == p[2][1]){ return getPos(p, 0, 1); }
    if(p[0][2] != '-' && p[0][2] == p[1][2] && p[0][2] == p[2][2]){ return getPos(p, 0, 2); }

    if(p[0][0] != '-' && p[0][0] == p[1][1] && p[0][0] == p[2][2]){ return getPos(p, 0, 0); }
    if(p[2][0] != '-' && p[2][0] == p[1][1] && p[2][0] == p[0][2]){ return getPos(p, 2, 0); }

    int total = 0;
    for(int i=0; i<9; i++){
        for(int k=0; k<3; k++){
            for(int j=0; j<3; j++){
                if(getPos3D(tab, i, k, j) == '-'){
                    total++;
                }
            }
        }
    }
    if(total == 81){
        return 'd';
    }

    return 'n';
}

// FUNCAO PARA OBTER O TABULEIRO SELECIONADO PARA A PROXIMA JOGADA
int updateTab(char **p, int x, int y){
    int val = 0;
    if(p[x][y] == '-'){
        if(x == 0 && y == 0){ return 0; }
        else if(x == 0 && y == 1){ return 1; }
        else if(x == 0 && y == 2){ return 2; }
        else if(x == 1 && y == 0){ return 3; }
        else if(x == 1 && y == 1){ return 4; }
        else if(x == 1 && y == 2){ return 5; }
        else if(x == 2 && y == 0){ return 6; }
        else if(x == 2 && y == 1){ return 7; }
        else if(x == 2 && y == 2){ return 8; }
    }else{
        for(int i=0; i<3; i++){
            for(int k=0; k<3; k++){
                if(p[i][k] == '-'){
                    printf("Tabuleiro indisponivel, o proximo tabuleiro sera o tabuleiro numero %d\n", val+1);
                    return val;
                }
                val++;
            }
        }

    }
    return val;
}

/*
void updateTabGrande(char ***tab, char **p){
    int curr = 0;
    for(int i=0; i<3; i++){
        for(int k=0; k<3; k++){
            if(getPos(p, i, k)  == '-'){
                if(getPos3D(tab, curr, 0, 0) == getPos3D(tab, curr, 0, 1) && getPos3D(tab, curr, 0, 2) == getPos3D(tab, curr, 0, 1)){
                    setPos(p, i, k, getPos3D(tab, curr, 0, 0));
                }
                if(getPos3D(tab, curr, 1, 0) == getPos3D(tab, curr, 1, 1) && getPos3D(tab, curr, 1, 2) == getPos3D(tab, curr, 1, 1)){
                    setPos(p, i, k, getPos3D(tab, curr, 0, 0));
                }
                if(getPos3D(tab, curr, 2, 0) == getPos3D(tab, curr, 2, 1) && getPos3D(tab, curr, 2, 2) == getPos3D(tab, curr, 2, 1)){
                    setPos(p, i, k, getPos3D(tab, curr, 0, 0));
                }

                if(getPos3D(tab, curr, 0, 0) == getPos3D(tab, curr, 1, 0) && getPos3D(tab, curr, 2, 0) == getPos3D(tab, curr, 1, 0)){
                    setPos(p, i, k, getPos3D(tab, curr, 0, 0));
                }
                if(getPos3D(tab, curr, 0, 1) == getPos3D(tab, curr, 1, 1) && getPos3D(tab, curr, 2, 1) == getPos3D(tab, curr, 1, 1)){
                    setPos(p, i, k, getPos3D(tab, curr, 0, 0));
                }
                if(getPos3D(tab, curr, 0, 2) == getPos3D(tab, curr, 1, 2) && getPos3D(tab, curr, 2, 2) == getPos3D(tab, curr, 1, 2)){
                    setPos(p, i, k, getPos3D(tab, curr, 0, 0));
                }

                if(getPos3D(tab, curr, 0, 0) == getPos3D(tab, curr, 1, 1) && getPos3D(tab, curr, 2, 2) == getPos3D(tab, curr, 1, 1)){
                    setPos(p, i, k, getPos3D(tab, curr, 0, 0));
                }

                if(getPos3D(tab, curr, 2, 0) == getPos3D(tab, curr, 1, 1) && getPos3D(tab, curr, 0, 2) == getPos3D(tab, curr, 1, 1)){
                    setPos(p, i, k, getPos3D(tab, curr, 0, 0));
                }
            }
            curr++;
        }
    }
}*/

// ATUALIZA O TABULEIRO GRANDE DE ACORDO COM OS TABULEIROS PEQUENOS
void updateTabGrande(char ***tab, char **p){
    int curr = 0;
    for(int i=0; i<3; i++){
        for(int j=0; j<3; j++){
            if(p[i][j] == '-'){
                if(getPos3D(tab, curr, 0, 0) != '-' && getPos3D(tab, curr, 0, 0) == getPos3D(tab, curr, 0, 1) && getPos3D(tab, curr, 0, 0) == getPos3D(tab, curr, 0, 2)){
                    setPos(p, i, j, getPos3D(tab, curr, 0, 0)); }

                if(getPos3D(tab, curr, 1, 0) != '-' && getPos3D(tab, curr, 1, 0) == getPos3D(tab, curr, 1, 1) && getPos3D(tab, curr, 1, 0) == getPos3D(tab, curr, 1, 2)){
                    setPos(p, i, j, getPos3D(tab, curr, 1, 0)); }

                if(getPos3D(tab, curr, 2, 0) != '-' && getPos3D(tab, curr, 2, 0) == getPos3D(tab, curr, 2, 1) && getPos3D(tab, curr, 2, 0) == getPos3D(tab, curr, 2, 2)){
                    setPos(p, i, j, getPos3D(tab, curr, 2, 0)); }


                if(getPos3D(tab, curr, 0, 0) != '-' && getPos3D(tab, curr, 0, 0) == getPos3D(tab, curr, 1, 0) && getPos3D(tab, curr, 0, 0) == getPos3D(tab, curr, 2, 0)){
                    setPos(p, i, j, getPos3D(tab, curr, 0, 0)); }

                if(getPos3D(tab, curr, 0, 1) != '-' && getPos3D(tab, curr, 0, 1) == getPos3D(tab, curr, 1, 1) && getPos3D(tab, curr, 0, 1) == getPos3D(tab, curr, 2, 1)){
                    setPos(p, i, j, getPos3D(tab, curr, 0, 1)); }

                if(getPos3D(tab, curr, 0, 2) != '-' && getPos3D(tab, curr, 0, 2) == getPos3D(tab, curr, 1, 2) && getPos3D(tab, curr, 0, 2) == getPos3D(tab, curr, 2, 2)){
                    setPos(p, i, j, getPos3D(tab, curr, 0, 2)); }

                if(getPos3D(tab, curr, 0, 0) != '-' && getPos3D(tab, curr, 0, 0) == getPos3D(tab, curr, 1, 1) && getPos3D(tab, curr, 0, 0) == getPos3D(tab, curr, 2, 2)){
                    setPos(p, i, j, getPos3D(tab, curr, 0, 0)); }

                if(getPos3D(tab, curr, 2, 2) != '-' && getPos3D(tab, curr, 2, 2)  == getPos3D(tab, curr, 1, 1) && getPos3D(tab, curr, 2, 2) == getPos3D(tab, curr, 0, 3)){
                    setPos(p, i, j, getPos3D(tab, curr, 0, 0)); }
            }
            curr++;
        }
    }
}

// FAZ UMA JOGADA ALEATORIA NO TABULEIRO SELECIONADO
void randomPlay(char ***tab, int curr, int* x, int* y){
    initRandom();
    int a,b;
    do{
        a = intUniformRnd(0,2);
        b = intUniformRnd(0,2);
        if(getPos3D(tab, curr, a, b) == '-'){
            *x = a;
            *y = b;
        }
    }while(getPos3D(tab, curr, a, b) != '-');
}

/*
void randomPlay(char ***tab, int curr, int* x, int* y){
    initRandom();
    for(int i=0; i<3; i++){
        for(int k=0; k<3; k++){
            if(getPos3D(tab, curr, i, k) == '-'){
                *x = i;
                *y = k;
                return;
            }
        }
    }
}*/