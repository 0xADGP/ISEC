//
// Created by adgp on 18-Dec-24.
//

#ifndef MILITAR_H
#define MILITAR_H

#include "Caravana.h"

class Militar : public Caravana
{
    string ultimaDirecao;

public:
    Militar(int id, int x, int y);
    void atualizar(Mapa *mapa) override;
    void atualizarNoMove(Mapa *mapa) override;
    char getTipo() override;
    void afetarTempestade(Mapa *mapa) override;
};

#endif // MILITAR_H
