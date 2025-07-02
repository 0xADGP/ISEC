//
// Created by adgp on 25-Dec-24.
//

#include "Jaula.h"
#include "../Mapa.h"

void Jaula::aplicarEfeito(Mapa *mapa)
{
    for (auto caravana : mapa->getCaravanas())
    {
        if (caravana->getX() == x && caravana->getY() == y && duracao > 0)
        {
            caravana->fazEfeito("jaula", mapa);
            duracao = 0;
        }
        else if (caravana->getX() == x && caravana->getY() == y + 1 && duracao > 0)
        {
            caravana->fazEfeito("jaula", mapa);
            duracao = 0;
        }
        else if (caravana->getX() == x && caravana->getY() == y - 1 && duracao > 0)
        {
            caravana->fazEfeito("jaula", mapa);
            duracao = 0;
        }
        else if (caravana->getX() == x + 1 && caravana->getY() == y && duracao > 0)
        {
            caravana->fazEfeito("jaula", mapa);
            duracao = 0;
        }
        else if (caravana->getX() == x - 1 && caravana->getY() == y && duracao > 0)
        {
            caravana->fazEfeito("jaula", mapa);
            duracao = 0;
        }
    }

    for (auto barbaro : mapa->getBarbaros())
    {
        if (barbaro->getX() == x && barbaro->getY() == y && duracao > 0)
        {
            barbaro->fazEfeito("jaula", mapa);
            duracao = 0;
        }
        else if (barbaro->getX() == x && barbaro->getY() == y + 1 && duracao > 0)
        {
            barbaro->fazEfeito("jaula", mapa);
            duracao = 0;
        }
        else if (barbaro->getX() == x && barbaro->getY() == y - 1 && duracao > 0)
        {
            barbaro->fazEfeito("jaula", mapa);
            duracao = 0;
        }
        else if (barbaro->getX() == x + 1 && barbaro->getY() == y && duracao > 0)
        {
            barbaro->fazEfeito("jaula", mapa);
            duracao = 0;
        }
        else if (barbaro->getX() == x - 1 && barbaro->getY() == y && duracao > 0)
        {
            barbaro->fazEfeito("jaula", mapa);
            duracao = 0;
        }
    }

    if (duracao <= 0)
    {
        mapa->removerItem(id);
    }
}

void Jaula::atualizar(Mapa *mapa)
{
    duracao--;
    aplicarEfeito(mapa);
    if (duracao <= 0)
    {
        mapa->removerItem(id);
    }
}

Jaula::Jaula(int x, int y, int duracao) : Item("jaula", x, y, duracao) {}