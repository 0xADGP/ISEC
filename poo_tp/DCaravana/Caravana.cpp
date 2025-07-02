//
// Created by adgp on 18-Dec-24.
//

#include "Caravana.h"

#include "../Simulacao.h"
#include "../Mapa.h"

Caravana::Caravana(int id, int x, int y) : id(id), x(x), y(y) {}

void Caravana::setX(int x)
{
    this->x = x;
}

void Caravana::setY(int y)
{
    this->y = y;
}

int Caravana::getId()
{
    return id;
}

int Caravana::getX()
{
    return x;
}

int Caravana::getY()
{
    return y;
}

int Caravana::getCarga()
{
    return carga;
}

int Caravana::getMaxAgua()
{
    return maxAgua;
}

int Caravana::getMaxCarga()
{
    return maxCarga;
}

int Caravana::getMaxTripulantes()
{
    return maxTripulantes;
}

int Caravana::getRandomBetween1and100()
{
    return rand() % 100 + 1;
}

int Caravana::getTripulantes()
{
    return tripulantes;
}

void Caravana::setCarga(int carga)
{
    this->carga = carga;
}

void Caravana::setAgua(int agua)
{
    this->agua = agua;
}

int Caravana::getAgua()
{
    return agua;
}

void Caravana::setTripulantes(int n)
{
    tripulantes = n;
}

void Caravana::fillAgua()
{
    agua = maxAgua;
}

void Caravana::setAuto(bool isAuto)
{
    this->isAuto = isAuto;
}

void Caravana::fazEfeito(string efeito, Mapa *mapa)
{
    if (efeito == "pandora")
    {
        tripulantes *= 0.8;
    }
    else if (efeito == "tesouro")
    {
        mapa->adicionaMoedasSimulacao(mapa->getMoedasSimulacao() * 0.1);
    }
    else if (efeito == "jaula")
    {
        tripulantes = maxTripulantes;
    }
    else if (efeito == "mina")
    {
        mapa->removerCaravana(id);
    }
    else if (efeito == "surpresa")
    {
        mapa->fazSurpresa(x, y);
    }

    if (id < 0)
    {
        cout << "Caravana barbara afetada por " << efeito << endl;
    }
    else
    {
        cout << "Caravana " << id << " afetada por " << efeito << endl;
    }
}

int Caravana::adicionarCarga(int carga)
{
    if (this->carga + carga > maxCarga)
    {
        cout << "Erro: Carga excede capacidade maxima\n";
        return 0;
    }
    if (carga < 1)
    {
        cout << "Erro: Carga nao pode ser 0 ou menor.\n";
        return 0;
    }
    this->carga += carga;
    return 1;
}

int Caravana::adicionarTripulantes(int n)
{
    if (tripulantes + n > maxTripulantes)
    {
        cout << "Erro: Tripulantes excede capacidade maxima\n";
        return 0;
    }
    if (n < 1)
    {
        cout << "Erro: Tripulantes nao pode ser 0 ou menor\n";
        return 0;
    }
    tripulantes += n;
    return 1;
}

void Caravana::moverRandomDirection(Mapa *mapa, int numSteps)
{
    int direcao = rand() % 8;
    for (int i = 0; i < numSteps; i++)
    {
        switch (direcao)
        {
        case 0:
            mover("D", mapa);
            break;
        case 1:
            mover("E", mapa);
            break;
        case 2:
            mover("C", mapa);
            break;
        case 3:
            mover("B", mapa);
            break;
        case 4:
            mover("CE", mapa);
            break;
        case 5:
            mover("CD", mapa);
            break;
        case 6:
            mover("BE", mapa);
            break;
        case 7:
            mover("BD", mapa);
            break;
        }
    }
}

void Caravana::moverParaCoords(int x, int y, Mapa *mapa)
{
    if (mapa->isTransitable(x, y))
    {
        this->x = x;
        this->y = y;
    }
}

void Caravana::killMyself(Mapa *mapa)
{
    if (getTipo() == '!')
    {
        mapa->removerBarbaro(id);
    }
    else
    {
        mapa->removerCaravana(id);
    }
}

void Caravana::mover(string direcao, Mapa *mapa)
{
    int newX = x;
    int newY = y;
    if (direcao == "D")
    {
        if (mapa->isTransitable(x, y + 1))
        {
            newY++;
        }
        // cout << "direita" << endl;
    }
    else if (direcao == "E")
    {
        if (mapa->isTransitable(x, y - 1))
        {
            newY--;
        }
        // cout << "esquerda" << endl;
    }
    else if (direcao == "C")
    {
        if (mapa->isTransitable(x - 1, y))
        {
            newX--;
        }
        // cout << "cima" << endl;
    }
    else if (direcao == "B")
    {
        if (mapa->isTransitable(x + 1, y))
        {
            newX++;
        }
        // cout << "baixo" << endl;
    }
    else if (direcao == "CE")
    {
        if (mapa->isTransitable(x - 1, y - 1))
        {
            newX--;
            newY--;
        }
        // cout << "cima esquerda" << endl;
    }
    else if (direcao == "CD")
    {
        if (mapa->isTransitable(x - 1, y + 1))
        {
            newX--;
            newY++;
        }
        // cout << "cima direita" << endl;
    }
    else if (direcao == "BE")
    {
        if (mapa->isTransitable(x + 1, y - 1))
        {
            newX++;
            newY--;
        }
        // cout << "baixo esquerda" << endl;
    }
    else if (direcao == "BD")
    {
        if (mapa->isTransitable(x + 1, y + 1))
        {
            newX++;
            newY++;
        }
        // cout << "baixo direita" << endl;
    }

    int linhas = mapa->getLinhas();
    int colunas = mapa->getColunas();

    if (newX >= linhas)
    { // oob baixo
        newX = 0;
    }
    else if (newX < 0)
    { // oob cima
        newX = linhas - 1;
    }

    if (newY >= colunas)
    { // oob dierita
        newY = 0;
    }
    else if (newY < 0)
    { // oob esquerda
        newY = colunas - 1;
    }

    if (mapa->isTransitable(newX, newY))
    {
        x = newX;
        y = newY;
    }
    else
    {
        // cout << "Erro: Posicao nao e transponivel\n";
    }
}

void Caravana::list()
{
    cout << "Caravana " << id << "/" << getTipo() << endl;
    cout << "Posicao: (" << x << ", " << y << ")" << endl;
    cout << "Tripulantes: " << tripulantes << "/" << maxTripulantes << endl;
    cout << "Carga: " << carga << "/" << maxCarga << endl;
    cout << "Agua: " << agua << "/" << maxAgua << endl;
    cout << "Auto: " << (isAuto ? "Sim" : "Nao") << endl;
    cout << "Instantes sem tripulantes: " << instantesSemTripulantes << endl;
}
