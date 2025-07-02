//
// Created by adgp on 22-Dec-24.
//

#ifndef BARBARO_H
#define BARBARO_H

#include "Caravana.h"

class Combate;

class Barbaro : public Caravana
{
    Combate *combate;
    static int idCounter;
    int duracao;
    int targetCaravanaId;

public:
    Barbaro(int x, int y, int duracao);
    int getDuracao() const;
    void atualizar(Mapa *mapa) override;
    void atualizarNoMove(Mapa *mapa) override;
    char getTipo() override;
    void combater(int idAdversario, Mapa *mapa);
    void afetarTempestade(Mapa *mapa) override;
};

#endif // BARBARO_H
