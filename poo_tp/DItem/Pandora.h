//
// Created by adgp on 25-Dec-24.
//

#ifndef PANDORA_H
#define PANDORA_H

#include "Item.h"

class Pandora : public Item
{
public:
    Pandora(int x, int y, int duracao);
    void atualizar(Mapa *mapa) override;
    void aplicarEfeito(Mapa *mapa) override;
};

#endif // PANDORA_H
