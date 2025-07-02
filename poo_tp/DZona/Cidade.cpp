//
// Created by adgp on 22-Dec-24.
//

#include "Cidade.h"

Cidade::Cidade(char id, int x, int y) : Zona(x, y, true), id(id) {}

void Cidade::adicionarCaravana(Caravana *caravana)
{
    caravanas.push_back(caravana);
}

void Cidade::removerCaravana(char id)
{
    for (auto it = caravanas.begin(); it != caravanas.end();)
    {
        if (*it != nullptr && (*it)->getId() == id)
        {
            it = caravanas.erase(it);
        }
        else
        {
            it++;
        }
    }
}

void Cidade::clearCaravanas()
{
    caravanas.clear();
}

char Cidade::getId() const
{
    return id;
}

void Cidade::list() const
{
    cout << "Cidade " << id << " tem " << caravanas.size() << " caravanas:" << endl;
    for (auto caravana : caravanas)
    {
        cout << "Caravana " << caravana->getId() << "/" << caravana->getTipo() << endl;
    }
}