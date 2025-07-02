//
// Created by adgp on 23-Dec-24.
//

#ifndef ITEM_H
#define ITEM_H
#include "../DCaravana/Caravana.h"

class Mapa;

class Item
{
protected:
    static int id;
    Mapa *mapa;
    string nome;
    bool isVisivel;
    int x, y, duracao;

public:
    Item(string nome, int x, int y, int duracao);
    int getX();
    int getY();
    int getId();
    int getDuracao();
    bool mVisivel();
    string getNome();
    virtual void atualizar(Mapa *mapa) = 0;
    virtual void aplicarEfeito(Mapa *mapa) = 0;
};

#endif // ITEM_H
