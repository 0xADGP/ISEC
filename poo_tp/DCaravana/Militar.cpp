//
// Created by adgp on 18-Dec-24.
//

#include "Militar.h"
#include "../Mapa.h"

Militar::Militar(int id, int x, int y) : Caravana(id, x, y)
{
    ultimaDirecao = "none";
    tripulantes = 40;
    maxCarga = 5;
    maxAgua = 400;
    carga = 0;
    agua = maxAgua;
    isAuto = false;
    instantesSemTripulantes = 0;
    maxTripulantes = 40;
}

void Militar::atualizar(Mapa *mapa)
{
    // cout << "Militar " << id << " atualizou" << endl;
    int hasMoved = 0;

    int linhas = mapa->getLinhas();
    int colunas = mapa->getColunas();

    if (tripulantes == 0)
    {
        instantesSemTripulantes++;
        if (ultimaDirecao != "none")
        {
            mover(ultimaDirecao, mapa);
        }
        hasMoved = 1;
        if (instantesSemTripulantes >= 7)
        {
            killMyself(mapa);
        }
    }

    if (isAuto && hasMoved == 0)
    {
        for (auto barbaro : mapa->getBarbaros())
        {
            int barX = barbaro->getX();
            int barY = barbaro->getY();

            // Calculate wrapped distances for toroidal map
            int dx = barX - x;
            int dy = barY - y;

            int wrappedDx = (abs(dx) <= linhas / 2) ? dx : -dx / abs(dx) * (linhas - abs(dx));
            int wrappedDy = (abs(dy) <= colunas / 2) ? dy : -dy / abs(dy) * (colunas - abs(dy));

            int absDx = abs(wrappedDx);
            int absDy = abs(wrappedDy);

            if (absDx <= 6 && absDy <= 6)
            {
                cout << "Barbaro encontrado em (" << barX << ", " << barY << "). dx=" << dx << ", dy=" << dy << endl;

                if (wrappedDx < 0 && wrappedDy < 0)
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
                else if (wrappedDx < 0 && wrappedDy > 0)
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
                else if (wrappedDx > 0 && wrappedDy < 0)
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
                else if (wrappedDx > 0 && wrappedDy > 0)
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
                else if (wrappedDx < 0)
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
                else if (wrappedDx > 0)
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
                else if (wrappedDy < 0)
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
                else if (wrappedDy > 0)
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
                break;
            }
        }
    }

    if (tripulantes > 0)
    {
        instantesSemTripulantes = 0;
    }
}

void Militar::atualizarNoMove(Mapa *mapa)
{
    // cout << "Militar " << id << " atualizou" << endl;

    if (tripulantes == 0)
    {
        instantesSemTripulantes++;
        if (ultimaDirecao != "none")
        {
            mover(ultimaDirecao, mapa);
        }
        if (instantesSemTripulantes >= 7)
        {
            killMyself(mapa);
        }
    }

    if (tripulantes > 0)
    {
        instantesSemTripulantes = 0;
    }
}

char Militar::getTipo()
{
    return 'M';
}

void Militar::afetarTempestade(Mapa *mapa)
{
    cout << "Militar " << id << " afetado por tempestade" << endl;
    tripulantes *= 0.9;
    if (getRandomBetween1and100() <= 33)
    {
        killMyself(mapa);
    }
}