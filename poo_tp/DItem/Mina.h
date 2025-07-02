//
// Created by adgp on 23-Dec-24.
//

#ifndef MINA_H
#define MINA_H

#include "Item.h"

class Mina : public Item
{
public:
    Mina(int x, int y, int duracao);
    void atualizar(Mapa *mapa) override;
    void aplicarEfeito(Mapa *mapa) override;
};

#endif // MINA_H
