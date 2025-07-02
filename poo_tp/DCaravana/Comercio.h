//
// Created by adgp on 18-Dec-24.
//

#ifndef COMERCIO_H
#define COMERCIO_H

#include "Caravana.h"

class Comercio : public Caravana
{
public:
    Comercio(int id, int x, int y);
    void atualizar(Mapa *mapa) override;
    void atualizarNoMove(Mapa *mapa) override;
    char getTipo() override;
    void afetarTempestade(Mapa *mapa) override;
};

#endif // COMERCIO_H
