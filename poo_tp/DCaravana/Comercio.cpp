//
// Created by adgp on 18-Dec-24.
//

#include "Comercio.h"
#include "../Mapa.h"

Comercio::Comercio(int id, int x, int y) : Caravana(id, x, y)
{
    tripulantes = 20;
    maxCarga = 40;
    maxAgua = 200;
    carga = 0;
    agua = maxAgua;
    isAuto = false;
    instantesSemTripulantes = 0;
    maxTripulantes = 20;
}

void Comercio::atualizar(Mapa *mapa)
{
    // cout << "Comercio " << id << " atualizou" << endl;
    int hasMoved = 0;
    string direction;

    int linhas = mapa->getLinhas();
    int colunas = mapa->getColunas();

    if (hasMoved == 0)
    {
        int closestDist = 3;
        int targetX = -1;
        int targetY = -1;

        for (auto item : mapa->getItems())
        {
            int itemX = item->getX();
            int itemY = item->getY();

            int dx = itemX - x;
            int dy = itemY - y;

            int wrappedDx = (abs(dx) <= linhas / 2) ? dx : -dx / abs(dx) * (linhas - abs(dx));
            int wrappedDy = (abs(dy) <= colunas / 2) ? dy : -dy / abs(dy) * (colunas - abs(dy));

            int absDx = abs(wrappedDx);
            int absDy = abs(wrappedDy);

            int dist = absDx + absDy;

            if (absDx <= 2 && absDy <= 2 && dist < closestDist)
            {
                closestDist = dist;
                targetX = wrappedDx;
                targetY = wrappedDy;
            }
        }

        if (closestDist <= 2)
        {
            if (targetX < 0 && targetY < 0)
            {
                direction = "CE";
            }
            else if (targetX < 0 && targetY > 0)
            {
                direction = "CD";
            }
            else if (targetX > 0 && targetY < 0)
            {
                direction = "BE";
            }
            else if (targetX > 0 && targetY > 0)
            {
                direction = "BD";
            }
            else if (targetX < 0)
            {
                direction = "C";
            }
            else if (targetX > 0)
            {
                direction = "B";
            }
            else if (targetY < 0)
            {
                direction = "E";
            }
            else if (targetY > 0)
            {
                direction = "D";
            }

            mover(direction, mapa);
            hasMoved = 1;
        }
    }

    if (isAuto && tripulantes > 0 && hasMoved == 0)
    {
        int numSteps = rand() % 2 + 1;
        moverRandomDirection(mapa, numSteps);
        hasMoved = 1;
    }

    if (tripulantes < maxTripulantes / 2)
    {
        agua--;
    }
    else if (tripulantes >= maxTripulantes / 2)
    {
        agua -= 2;
    }
    else if (tripulantes == 0)
    {
        if (hasMoved == 0)
        {
            int numSteps = rand() % 2 + 1;
            moverRandomDirection(mapa, numSteps);
            hasMoved = 1;
        }
        instantesSemTripulantes++;
        if (instantesSemTripulantes >= 5)
        {
            killMyself(mapa);
        }
    }

    if (tripulantes > 0)
    {
        instantesSemTripulantes = 0;
    }
}

void Comercio::atualizarNoMove(Mapa *mapa)
{
    if (tripulantes < maxTripulantes / 2)
    {
        agua--;
    }
    else if (tripulantes >= maxTripulantes / 2)
    {
        agua -= 2;
    }
    else if (tripulantes == 0)
    {
        instantesSemTripulantes++;
        if (instantesSemTripulantes >= 5)
        {
            killMyself(mapa);
        }
    }

    if (tripulantes > 0)
    {
        instantesSemTripulantes = 0;
    }
}

char Comercio::getTipo()
{
    return 'C';
}

void Comercio::afetarTempestade(Mapa *mapa)
{
    cout << "Comercio " << id << " afetado por tempestade" << endl;
    carga *= 0.75;
    if (carga > maxCarga / 2)
    {
        if (getRandomBetween1and100() <= 50)
        {
            killMyself(mapa);
        }
    }
    else
    {
        if (getRandomBetween1and100() <= 25)
        {
            killMyself(mapa);
        }
    }
}