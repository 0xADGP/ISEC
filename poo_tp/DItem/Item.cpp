//
// Created by adgp on 23-Dec-24.
//

#include "Item.h"

#include "../Mapa.h"

int Item::id = 0;

int Item::getX()
{
    return x;
}

int Item::getY()
{
    return y;
}

int Item::getId()
{
    return id;
}

int Item::getDuracao()
{
    return duracao;
}

string Item::getNome()
{
    return nome;
}

bool Item::mVisivel()
{
    return isVisivel;
}

Item::Item(string nome, int x, int y, int duracao) : nome(nome), x(x), y(y), duracao(duracao)
{
    id++;
    isVisivel = true;
}