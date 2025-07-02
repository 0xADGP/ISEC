//
// Created by adgp on 18-Dec-24.
//

#include "Secreta.h"
#include "../Mapa.h"

Secreta::Secreta(int id, int x, int y) : Caravana(id, x, y)
{
    tripulantes = 10;
    maxCarga = 20;
    maxAgua = 100;
    carga = 0;
    agua = maxAgua;
    isAuto = false;
    instantesSemTripulantes = 0;
    maxTripulantes = 10;
}

void Secreta::atualizar(Mapa *mapa)
{
    // cout << "Secreta " << id << " atualizou" << endl;
    int hasMoved = 0;
    string direction;

    int linhas = mapa->getLinhas();
    int colunas = mapa->getColunas();

    if (isAuto && hasMoved == 0)
    {
        int closestDist = 3;
        int targetX = -1;
        int targetY = -1;

        for (auto barbaro : mapa->getBarbaros())
        {
            int barX = barbaro->getX();
            int barY = barbaro->getY();

            int dx = barX - x;
            int dy = barY - y;

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
                direction = "BD";
            }
            else if (targetX < 0 && targetY > 0)
            {
                direction = "BE";
            }
            else if (targetX > 0 && targetY < 0)
            {
                direction = "CD";
            }
            else if (targetX > 0 && targetY > 0)
            {
                direction = "CE";
            }
            else if (targetX < 0)
            {
                direction = "B";
            }
            else if (targetX > 0)
            {
                direction = "C";
            }
            else if (targetY < 0)
            {
                direction = "D";
            }
            else if (targetY > 0)
            {
                direction = "E";
            }

            cout << "Caravana " << id << " a fugir da barbara (" << x + targetX << ", " << y + targetY << ") Direcao: " << direction << endl;

            mover(direction, mapa);
            hasMoved = 1;
        }
    }

    if (isAuto && hasMoved == 0)
    {
        int steps = rand() % 3 + 1;
        moverRandomDirection(mapa, steps);
    }

    if (mapa->getTurnosSimulacao() % 2 == 0)
    {
        if (rand() % 2 == 0)
        {
            agua += 2;
            if (agua > maxAgua)
            {
                agua = maxAgua;
            }
        }
        if (rand() % 2 == 0)
        {
            carga += 2;
            if (carga > maxCarga)
            {
                carga = maxCarga;
            }
        }
    }

    if (tripulantes == 0)
    {
        instantesSemTripulantes++;
    }

    if (instantesSemTripulantes >= 15)
    {
        killMyself(mapa);
    }

    if (tripulantes > 0)
    {
        instantesSemTripulantes = 0;
    }
}

void Secreta::atualizarNoMove(Mapa *mapa)
{
    // cout << "Secreta " << id << " atualizou" << endl;
    if (mapa->getTurnosSimulacao() % 2 == 0)
    {
        if (rand() % 2 == 0)
        {
            agua += 2;
            if (agua > maxAgua)
            {
                agua = maxAgua;
            }
        }
        if (rand() % 2 == 0)
        {
            carga += 2;
            if (carga > maxCarga)
            {
                carga = maxCarga;
            }
        }
    }

    if (tripulantes == 0)
    {
        instantesSemTripulantes++;
    }

    if (instantesSemTripulantes >= 15)
    {
        killMyself(mapa);
    }

    if (tripulantes > 0)
    {
        instantesSemTripulantes = 0;
    }
}

char Secreta::getTipo()
{
    return 'S';
}

void Secreta::afetarTempestade(Mapa *mapa)
{
    cout << "Secreta " << id << " nao pode ser afetada por tempestade" << endl;
}