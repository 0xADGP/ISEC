//
// Created by adgp on 22-Dec-24.
//

#ifndef ZONA_H
#define ZONA_H

#include <iostream>

class Caravana;
using namespace std;

class Zona
{
protected:
    int x, y;
    bool isTransitable;

public:
    Zona(int x, int y, bool isTransitable);

    int getX() const;
    int getY() const;
    bool getIsTransitable() const;

    virtual char getId() const = 0;
    virtual void list() const = 0;
    virtual void adicionarCaravana(Caravana *caravana) {}
    virtual void removerCaravana(char id) {}
    virtual void clearCaravanas() {}
};

#endif // ZONA_H
