//
// Created by adgp on 22-Dec-24.
//

#include "Montanha.h"

Montanha::Montanha(int x, int y) : Zona(x, y, false) {}

char Montanha::getId() const
{
    return '+';
}

void Montanha::list() const
{
    cout << "Montanha na posicao (" << x << ", " << y << ")" << endl;
}