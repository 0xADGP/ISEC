//
// Created by adgp on 25-Dec-24.
//

#ifndef SURPRESA_H
#define SURPRESA_H

#include "Item.h"

class Surpresa : public Item
{
public:
    Surpresa(int x, int y, int duracao);
    void atualizar(Mapa *mapa) override;
    void aplicarEfeito(Mapa *mapa) override;
};

#endif // SURPRESA_H
