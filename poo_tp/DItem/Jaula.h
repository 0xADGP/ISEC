//
// Created by adgp on 25-Dec-24.
//

#ifndef JAULA_H
#define JAULA_H

#include "Item.h"

class Jaula : public Item
{
public:
    Jaula(int x, int y, int duracao);
    void atualizar(Mapa *mapa) override;
    void aplicarEfeito(Mapa *mapa) override;
};

#endif // JAULA_H
