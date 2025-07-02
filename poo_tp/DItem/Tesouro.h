//
// Created by adgp on 25-Dec-24.
//

#ifndef TESOURO_H
#define TESOURO_H

#include "Item.h"

class Tesouro : public Item
{
public:
    Tesouro(int x, int y, int duracao);
    void atualizar(Mapa *mapa) override;
    void aplicarEfeito(Mapa *mapa) override;
};

#endif // TESOURO_H
