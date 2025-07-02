// ANTÓNIO DOMINGOS GONÇALVES PEDROSO 2021132042

// Trabalho Pratico Programacao - LEI-CE
// DEIS-ISEC 2021-2022

#include <stdio.h> // inclui stdio library stdio.h

#include "utils.h" // inclui header files
#include "matdin.h" // inclui header files
#include "matdin3d.h" // inclui header files
#include "jogofunc.h" // inclui header files
#include "list.h" // inclui header files

int main() { // funcao main
    initRandom(); //inicia o random

    char** tabGrande = criaMat(3,3); // criar matriz grande 2d que contem os tabuleiros pequenos
    char*** tabPequeno = criaMat3D(9,3,3); // cria matriz 3d que contem os 9 tabuleiros pequenos
    int auxx, auxy, tabaux, prog, currentTab, temp = 2, scanie, scaniej, escolhamodo, escolhatabinicial, numjogadas = 1, turnojog = 1; // cria variaveis a usar
    char atual = 'x'; // char do jogador atual, quando char = x o jogador atual é 1, quando char = o o jogador atual é 2

    pjogada lista = NULL; // criar a lista

    lista = recupera_lista("jogadas.bin"); // recupera a lista do ficheiro

    printf("Bem-vindo ao Jogo do Galo.\n"); // mostra no ecra informacoes acerca do jogo
    printf("Os tabuleiros serao designados da seguinte forma:\n\n"); //
    printf(" Tab1 \t Tab2 \t Tab3 \n Tab4 \t Tab5 \t Tab6 \n Tab7 \t Tab8 \t Tab9\n\n"); //
    printf("As posicoes serao designadas da seguinte forma:\n\n"); //
    printf(" L1C1 \t L1C2 \t L1C3 \n L2C1 \t L2C2 \t L2C3 \n L3C1 \t L3C2 \t L3C3\n\n"); //


    if(lista_vazia(lista) == 0){ // se a lista existir
        printf("Deseja continuar o jogo anteriormente gravado?\n\t1- Sim\n\t2- Nao\nIntroduza a escolha:"); // perguntar se quer continuar jogo guardado no ficheiro
        scanf("%d", &temp); // ler escolha do utilizador

        if(temp == 1){ // se escolha igual a sim
            atualiza_tabuleiro(lista, tabPequeno); // atualiza os tabuleiros pequenos de acordo com a informção que está na lista
            updateTabGrande(tabPequeno, tabGrande); // atualiza o tabuleiro grande de acordo com a informação dos tabuleiros pequenos
            atual = get_last_player(lista); // obtem o jugador atual
            auxx = get_last_x(lista); // obtem o ultimo x que sera usado para verificar o prox tabuleiro
            auxy = get_last_y(lista); // obtem o ultimo y que sera usado para verificar o prox tabuleiro
            numjogadas = get_last_jogada(lista) + 1; // obtem o numero de jogadas +1 que será p numero da proxima jogada
            currentTab = updateTab(tabGrande,auxx,auxy); // atualiza o currenttab que é o tabuleiro selecionado

            mostraJogo(tabPequeno); // mostra o jogo
        }else{
            lista = NULL; // limpa a lista
        }
    }

    printf("Escolha o tipo de jogo:\n\t1- Jogo entre 2 jogadores humanos.\n\t2- Jogo entre um humano e um jogador automatico.\nIntroduza a escolha:"); // pergunta qual modo o jogador quer
    do{ // faz
        scanf("%d", &escolhamodo); // regista a escolha do utilizador
        if(escolhamodo < 1 || escolhamodo > 2){ // se o modo for menor que 1 ou modo maior que 2
            printf("Escolha invalida.\nIntroduza a escolha:"); // mostra que a escolha é invalida
        }
    }while(escolhamodo < 1 || escolhamodo > 2); // enquando escolha <1 ou escolha >2

    if(temp == 2){ // se o jogador escolheu que nao quer continuar o jogo guardado
        printf("\nEscolha se quer escolher o tabuleiro inicial:\n\t1- Escolher tabuleiro inicial.\n\t2- Tabuleiro inicial aleatorio.\nIntroduza a escolha:"); // perguntar tabuleiro inicial
        do{ // faz
            scanf("%d", &escolhatabinicial); // regista a escolha do jogador
            if(escolhatabinicial < 1 || escolhatabinicial > 2){ // se a escolha for <1 ou >2
                printf("Escolha invalida.\nIntroduza a escolha:"); // mostra que é invalido
            }
        }while(escolhatabinicial < 1 || escolhatabinicial > 2); // enquando escolha <1 ou >2

        if(escolhatabinicial == 1){ // se o utilizador disser que quer escolher tabuleiro inicial
            printf("Escolha o numero do tabuleiro inicial:"); // pergunta o tabuleiro inciail
            scanf("%d", &currentTab); // le o tabuleiro inicial escolhido
        }else{ // senao
            currentTab = intUniformRnd(0,8); // escolher tabuleiro random entre 0-8 (tabuleiro 1-9)
        }
    }

    do{ // faz
        printf("\nTabuleiro numero %d selecionado\n", currentTab+1); // mostra o tabuleiro selecionado
        printf("Turno do jogador %d\n", turnojog); // mostra quem é a jogar

        do{ // faz
            if(numjogadas >1 && turnojog == 1){ // se o numero de jogadas >1 e for a vez do jogador1 jogar
                printf("Guardar progresso e sair?\n\t1- Sim\n\t2- Nao\nEscolha:"); // perguntar se quer guardar o progresso e sair
                scanf("%d", &prog); // ler escolha do utilizador
                if(prog == 1){ // caso a escolha = 1
                    int b = grava_lista(lista, "jogadas.bin"); // guarda a lista com as jogadas no ficheiro .bin
                    if(b == 1){ // se tiver guardado com sucesso
                        printf("Progesso guardado. A sair..."); // mostrar que guardou com sucesso
                    }else{ // senao
                        printf("Ocorreu um erro na gravacao. A sair..."); // mostrar que ocorreu um erro na gravacao para o ficheiro
                    }
                    return 1; // retornar 1
                }
            }

            if(numjogadas >1){ // se numero de jogadas >1
                printf("\nQuer ver as jogadas anteriores?\n\t1- Sim\n\t2- Nao\nEscolha:"); // perguntar se quer ver as jogadas anteriores
                scanf("%d", &scanie); // ler escolha do utilizador

                if(scanie == 1){ // se uttilizador quiser ver jogadas anteriores
                    do{ // faz
                        printf("Quantas jogadas quer ver? (1 a 10)\nNum de jogadas a ver:"); // perguntar quantas jogadas quer ver
                        scanf("%d", &scaniej); // ler o total de jogadas
                        if(scaniej < 1 || scaniej > 10){ // se total <1 ou total > 10
                            printf("Numero invalido.\nIntroduza um numero entre 1 e 10:"); // dizer que tem de escolher um numero entra 1 e 10
                        }
                    }while(scaniej < 1 || scaniej > 10); // enquanto o total de jogadas <1 ou total de jogadas >10

                    printf("\n"); // dá um enter
                    mostra_infoX(lista, scaniej); //mostrar o total de ultimas jogadas escolhidas pelo utilizador na lista
                }
            }

            printf("Linha (entre 1 e 3):"); // perguntar ao utilkizador a linha entra 1 e 3
            do{ // faz
                scanf("%d", &auxx); // ler escolha do utilizador
                if(auxx < 1 || auxx > 3) { // se auxx <1 ou auxx >3
                    printf("Linha invalida.\nIntroduza linha:"); // dizer que é invalido
                }
            }while(auxx < 1 || auxx > 3); // euqnato auxx <1 ou auxx >3

            printf("Coluna (entre 1 e 3):"); // perguntar ao utilkizador a coluna entra 1 e 3
            do{ // faz
                scanf("%d", &auxy); // ler escolha do utilizador
                if(auxy < 1 || auxy > 3){ // se auxy <1 ou auxy >3
                    printf("Coluna invalida.\nIntroduza coluna:"); // dizer que é invalido
                }
            }while(auxy < 1 || auxy > 3); // enquanto auxy <1 ou auxy >3

            auxx--; // remover 1 ao auxx para selecionar as coordenadas certas no array
            auxy--; // remover 1 ao auxy para selecionar as coordenadas certas no array

            if(getPos3D(tabPequeno,currentTab, auxx, auxy) != '-'){ // se a posicao escolhido estiver ocupado
                printf("Coordenadas invalidas.\n"); // dizer que é invalido
                mostraJogo(tabPequeno); // mostrar jogo
            }
        }while(getPos3D(tabPequeno,currentTab, auxx, auxy) != '-'); // enquanto a posicao escolhida estiver ocupada

        printf("\n"); // print nova linha

        setPos3D(tabPequeno, currentTab, auxx, auxy, atual); // define a posicao da matriz 3d tabqueno[tabuleiro atual][coordenada x][coordenada y] para o acracter da variavel atual
        updateTabGrande(tabPequeno, tabGrande); // atualiza o tabuleiro grande

        tabaux = currentTab; // auxiliar para guardar o tabuleiro atual
        currentTab = updateTab(tabGrande,auxx,auxy); // atualizar o tabuleiro atual para o novo tabuleiro

        if(getPos3D(tabPequeno,currentTab, auxx, auxy) == '-'){ // se a posicao estiver disponivel
            lista = insere_inicio(lista, numjogadas, turnojog, tabaux, auxx, auxy); // insere na lista a jogada
            numjogadas++; // aumenta o numero de jogadas
        }


        if(escolhamodo == 2){ // se o jogador estiver a jogar contra um robo
            tabaux = currentTab; // definiar tab auxiliar para o tab atual
            randomPlay(tabPequeno, currentTab, &auxx, &auxy); // alterar auxx e auxy para coordenadas validas na prox jogada
            setPos3D(tabPequeno, currentTab, auxx, auxy, 'o'); // meter no jogo o valor
            updateTabGrande(tabPequeno, tabGrande); // atulizar o tabuleiro grande

            lista = insere_inicio(lista, numjogadas, 2, tabaux, auxx, auxy); // adicionar a jogada à lista
            numjogadas++; // aumentar o numero de jogada

            currentTab = updateTab(tabGrande,auxx,auxy); // atualizar o tabuleiro selecionado

            turnojog = 1; // mudar o turno de jogador para o jogador1
            atual = 'x'; // mudar o caracter atual para x (jogador1)
        }

        if(escolhamodo == 1){ // se o modo for player vs player
            if(turnojog == 1){ // se o jogador1 jogou
                turnojog = 2; // turno será do jogador 2
                atual = 'o'; // alterar atual para 'o' (jogador 2)
            }else{ // senao
                turnojog = 1; // será a vez do jogador 1
                atual = 'x'; // alterar atual para o caracter do jogador 1
            }
        }

        mostraJogo(tabPequeno); // mostra o jogo
        //mostraJogoV2(tabPequeno);
    }while(checkWinner(tabGrande, tabPequeno) == 'n'); // enquanto não houver vencedor nem empate

    char winner = (checkWinner(tabGrande, tabPequeno)); // obter vencedor ou empate

    if(winner == 'x'){ // se winner for = 'x'
        printf("Ganhou o jogador 1 (%c)", winner); // vencedor é o jogador 1
    }else if(winner == 'o'){ // senao se winner for = 'o'
        printf("Ganhou o jogador 2 (%c)", winner); // vence jogador 2
    }else{ // senao é porque winner = 'd'
        printf("Empate"); // empate
    }

    mostraJogo(tabPequeno); // mostra o jogo

    /*
    lista = NULL; // limpa a lista
    int xb = grava_lista(lista, "jogadas.bin"); // limpa o ficheiro
    if(xb == 1){ // vê se teve sucesso
        printf("Sucesso a apagar"); // confirma
    }else{
        printf("Erro a apagar"); // diz que teve erro
    }*/

    return 0; // retorna 0
}




/*  mostra_info(lista);
    mostraJogo(tabPequeno);


    lista = insere_inicio(lista, 1, 1, 1, 2, 1);
    lista = insere_inicio(lista, 2, 2, 4, 1, 1);
    lista = insere_inicio(lista, 3, 1, 2, 2, 1);
    lista = insere_inicio(lista, 4, 2, 1, 1, 1);
    lista = insere_inicio(lista, 5, 1, 4, 2, 1);
    lista = insere_inicio(lista, 6, 2, 5, 1, 2);
    lista = insere_inicio(lista, 7, 1, 6, 2, 1);
    lista = insere_inicio(lista, 8, 2, 7, 1, 1);
    lista = insere_inicio(lista, 9, 1, 8, 2, 2);
    lista = insere_inicio(lista, 10, 2, 1, 1, 1);
    lista = insere_inicio(lista, 11, 1, 2, 2, 2);
    lista = insere_inicio(lista, 12, 2, 3, 1, 1);

    mostra_info(lista);
    printf("\n\n");
    mostra_infoX(lista, 4);

    int abc = grava_lista(lista, "jogadas.bin");
    if(abc == 1){
        printf("Sucesso");
    }else{
        printf("Erro");
    }*/


/*setPos3D(tabPequeno, 4, 0, 1, 'x');
   setPos3D(tabPequeno, 4, 1, 1, 'x');
   setPos3D(tabPequeno, 4, 2, 1, 'x');

   setPos3D(tabPequeno, 7, 0, 0, 'x');
   setPos3D(tabPequeno, 7, 1, 1, 'x');
   setPos3D(tabPequeno, 7, 2, 2, 'x');

   setPos3D(tabPequeno, 1, 1, 1, 'x');
   setPos3D(tabPequeno, 1, 2, 2, 'x');

   updateTabGrande(tabPequeno, tabGrande);
   mostraMat(tabGrande, 3,3);
   mostraJogo(tabPequeno);*/


//printf("%d\n", scaniej);
//mostra_info(lista);


//mostraJogo(tabPequeno);

//mostraJogo(tabPequeno);

/*mostraMat(tabGrande, 3, 3);
        mostra_info(lista);*/
