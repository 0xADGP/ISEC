//
// Created by adgp on 25-Dec-24.
//

#include "Tesouro.h"
#include "../Mapa.h"

void Tesouro::aplicarEfeito(Mapa *mapa)
{
    for (auto caravana : mapa->getCaravanas())
    {
        if (caravana->getX() == x && caravana->getY() == y && duracao > 0)
        {
            caravana->fazEfeito("tesouro", mapa);
            duracao = 0;
        }
        else if (caravana->getX() == x && caravana->getY() == y + 1 && duracao > 0)
        {
            caravana->fazEfeito("tesouro", mapa);
            duracao = 0;
        }
        else if (caravana->getX() == x && caravana->getY() == y - 1 && duracao > 0)
        {
            caravana->fazEfeito("tesouro", mapa);
            duracao = 0;
        }
        else if (caravana->getX() == x + 1 && caravana->getY() == y && duracao > 0)
        {
            caravana->fazEfeito("tesouro", mapa);
            duracao = 0;
        }
        else if (caravana->getX() == x - 1 && caravana->getY() == y && duracao > 0)
        {
            caravana->fazEfeito("tesouro", mapa);
            duracao = 0;
        }
    }

    for (auto barbaro : mapa->getBarbaros())
    {
        if (barbaro->getX() == x && barbaro->getY() == y && duracao > 0)
        {
            barbaro->fazEfeito("tesouro", mapa);
            duracao = 0;
        }
        else if (barbaro->getX() == x && barbaro->getY() == y + 1 && duracao > 0)
        {
            barbaro->fazEfeito("tesouro", mapa);
            duracao = 0;
        }
        else if (barbaro->getX() == x && barbaro->getY() == y - 1 && duracao > 0)
        {
            barbaro->fazEfeito("tesouro", mapa);
            duracao = 0;
        }
        else if (barbaro->getX() == x + 1 && barbaro->getY() == y && duracao > 0)
        {
            barbaro->fazEfeito("tesouro", mapa);
            duracao = 0;
        }
        else if (barbaro->getX() == x - 1 && barbaro->getY() == y && duracao > 0)
        {
            barbaro->fazEfeito("tesouro", mapa);
            duracao = 0;
        }
    }

    if (duracao <= 0)
    {
        mapa->removerItem(id);
    }
}

void Tesouro::atualizar(Mapa *mapa)
{
    duracao--;
    aplicarEfeito(mapa);
    if (duracao <= 0)
    {
        mapa->removerItem(id);
    }
}

Tesouro::Tesouro(int x, int y, int duracao) : Item("tesouro", x, y, duracao) {}