//
// Created by adgp on 24-Dec-24.
//

#include "Tempestade.h"

#include "../Mapa.h"

int Tempestade::id = 0;

Tempestade::Tempestade(int x, int y, int raio) : x(x), y(y), raio(raio)
{
    id++;
    duracao = 1;
}

int Tempestade::getX() const
{
    return x;
}

int Tempestade::getY() const
{
    return y;
}

int Tempestade::getId() const
{
    return id;
}

int Tempestade::getRaio() const
{
    return raio;
}

void Tempestade::atualizar(Mapa *mapa)
{
    mapa->afetarCaravanasTempestades(this);
    duracao--;
    if (duracao == 0)
    {
        mapa->apagarTempestade(id);
    }
}