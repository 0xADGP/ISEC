//
// Created by adgp on 22-Dec-24.
//

#include "Barbaro.h"
#include "../Mapa.h"
#include "../Combate.h"

#include <algorithm>

int Barbaro::idCounter = -500;

Barbaro::Barbaro(int x, int y, int duracao) : Caravana(idCounter--, x, y)
{
    agua = 40;
    tripulantes = 40;
    maxTripulantes = 40;
    this->duracao = duracao;
    targetCaravanaId = -1;
    isAuto = true;
}

int Barbaro::getDuracao() const
{
    return duracao;
}

void Barbaro::atualizar(Mapa *mapa)
{
    // cout << "Barbaro " << id << " atualizou" << endl;
    int hasMoved = 0;
    int caravanasLado = 0;
    vector<int> caravanasParaCombater;

    for (auto caravana : mapa->getCaravanas())
    {
        int caravanaX = caravana->getX();
        int caravanaY = caravana->getY();

        int dx = abs(caravanaX - x);
        int dy = abs(caravanaY - y);

        dx = min(dx, mapa->getLinhas() - dx);
        dy = min(dy, mapa->getColunas() - dy);

        if (dx <= 1 && dy <= 1)
        {
            if (!mapa->isCidade(caravanaX, caravanaY))
            {
                caravanasLado++;
                caravanasParaCombater.push_back(caravana->getId());
            }
        }
    }

    if (caravanasLado >= 2)
    {
        if (mapa->isCidade(x, y))
        {
            return;
        }
        for (auto idCar : caravanasParaCombater)
        {
            combater(idCar, mapa);

            if (!mapa->existeBarbaro(id))
            {
                break;
            }
        }
    }

    int closestDist = 9;
    int targetX = -1;
    int targetY = -1;

    for (auto caravana : mapa->getCaravanas())
    {
        if (caravana != nullptr)
        {
            int carX = caravana->getX();
            int carY = caravana->getY();

            int dx = carX - x;
            int dy = carY - y;

            int wrappedDx = (abs(dx) <= mapa->getLinhas() / 2) ? dx : -dx / abs(dx) * (mapa->getLinhas() - abs(dx));
            int wrappedDy = (abs(dy) <= mapa->getColunas() / 2) ? dy : -dy / abs(dy) * (mapa->getColunas() - abs(dy));

            int absDx = abs(wrappedDx);
            int absDy = abs(wrappedDy);

            int dist = absDx + absDy;

            if (absDx <= 8 && absDy <= 8 && dist < closestDist)
            {
                closestDist = dist;
                targetX = wrappedDx;
                targetY = wrappedDy;
            }
        }
    }

    if (closestDist <= 8)
    {
        if (targetX < 0 && targetY < 0)
        {
            if (x - 1 >= 0 && y - 1 >= 0 && x - 1 < mapa->getLinhas() && y - 1 < mapa->getColunas() && mapa->isPositionEmpty(x - 1, y - 1))
            {
                mover("CE", mapa);
            }
            else
            {
                if (rand() % 2 == 0)
                {
                    mover("C", mapa);
                }
                else
                {
                    mover("E", mapa);
                }
            }
        }
        else if (targetX < 0 && targetY > 0)
        {
            if (x - 1 >= 0 && y - 1 >= 0 && x - 1 < mapa->getLinhas() && y - 1 < mapa->getColunas() && mapa->isPositionEmpty(x - 1, y + 1))
            {
                mover("CD", mapa);
            }
            else
            {
                if (rand() % 2 == 0)
                {
                    mover("C", mapa);
                }
                else
                {
                    mover("D", mapa);
                }
            }
        }
        else if (targetX > 0 && targetY < 0)
        {
            if (x - 1 >= 0 && y - 1 >= 0 && x - 1 < mapa->getLinhas() && y - 1 < mapa->getColunas() && mapa->isPositionEmpty(x + 1, y - 1))
            {
                mover("BE", mapa);
            }
            else
            {
                if (rand() % 2 == 0)
                {
                    mover("B", mapa);
                }
                else
                {
                    mover("E", mapa);
                }
            }
        }
        else if (targetX > 0 && targetY > 0)
        {
            if (x - 1 >= 0 && y - 1 >= 0 && x - 1 < mapa->getLinhas() && y - 1 < mapa->getColunas() && mapa->isPositionEmpty(x + 1, y + 1))
            {
                mover("BD", mapa);
            }
            else
            {
                if (rand() % 2 == 0)
                {
                    mover("B", mapa);
                }
                else
                {
                    mover("D", mapa);
                }
            }
        }
        else if (targetX < 0)
        {
            if (x - 1 >= 0 && y - 1 >= 0 && x - 1 < mapa->getLinhas() && y - 1 < mapa->getColunas() && mapa->isPositionEmpty(x - 1, y))
            {
                mover("C", mapa);
            }
            else
            {
                if (rand() % 2 == 0)
                {
                    mover("CE", mapa);
                }
                else
                {
                    mover("CD", mapa);
                }
            }
        }
        else if (targetX > 0)
        {
            if (x - 1 >= 0 && y - 1 >= 0 && x - 1 < mapa->getLinhas() && y - 1 < mapa->getColunas() && mapa->isPositionEmpty(x + 1, y))
            {
                mover("B", mapa);
            }
            else
            {
                if (rand() % 2 == 0)
                {
                    mover("BE", mapa);
                }
                else
                {
                    mover("BD", mapa);
                }
            }
        }
        else if (targetY < 0)
        {
            if (x - 1 >= 0 && y - 1 >= 0 && x - 1 < mapa->getLinhas() && y - 1 < mapa->getColunas() && mapa->isPositionEmpty(x, y - 1))
            {
                mover("E", mapa);
            }
            else
            {
                if (rand() % 2 == 0)
                {
                    mover("CE", mapa);
                }
                else
                {
                    mover("BE", mapa);
                }
            }
        }
        else if (targetY > 0)
        {
            if (x - 1 >= 0 && y - 1 >= 0 && x - 1 < mapa->getLinhas() && y - 1 < mapa->getColunas() && mapa->isPositionEmpty(x, y + 1))
            {
                mover("D", mapa);
            }
            else
            {
                if (rand() % 2 == 0)
                {
                    mover("CD", mapa);
                }
                else
                {
                    mover("BD", mapa);
                }
            }
        }
    }
    else
    {
        moverRandomDirection(mapa, 1);
    }

    duracao--;
    if (duracao <= 0)
    {
        killMyself(mapa);
    }
}

void Barbaro::atualizarNoMove(Mapa *mapa)
{
    cout << "Impossivel atualizar barbaro sem movimento" << endl;
}

void Barbaro::afetarTempestade(Mapa *mapa)
{
    cout << "Barbaro " << id << " afetado por tempestade" << endl;
    tripulantes *= 0.9;
    if (getRandomBetween1and100() <= 25)
    {
        killMyself(mapa);
    }
    if (tripulantes < 1)
    {
        killMyself(mapa);
    }
}

char Barbaro::getTipo()
{
    return '!';
}

void Barbaro::combater(int idAdversario, Mapa *mapa)
{
    Caravana *adversario = mapa->getCaravanaById(idAdversario);
    if (adversario != nullptr)
    {
        Combate::combate(adversario, this, mapa);
    }
}
