//
// Created by adgp on 22-Dec-24.
//

#include "Zona.h"

Zona::Zona(int x, int y, bool isTransitable) : x(x), y(y), isTransitable(isTransitable) {}

int Zona::getX() const
{
    return x;
}

int Zona::getY() const
{
    return y;
}

bool Zona::getIsTransitable() const
{
    return isTransitable;
}