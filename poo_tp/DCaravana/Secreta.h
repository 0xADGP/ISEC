//
// Created by adgp on 18-Dec-24.
//

#ifndef SECRETA_H
#define SECRETA_H

#include "Caravana.h"

class Secreta : public Caravana
{
public:
    Secreta(int id, int x, int y);
    void atualizar(Mapa *mapa) override;
    void atualizarNoMove(Mapa *mapa) override;
    char getTipo() override;
    void afetarTempestade(Mapa *mapa) override;
};

#endif // SECRETA_H
