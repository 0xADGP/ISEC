//
// Created by adgp on 22-Dec-24.
//

#ifndef CIDADE_H
#define CIDADE_H

#include <vector>

#include "Zona.h"
#include "../DCaravana/Caravana.h"

class Cidade : public Zona
{
    char id;
    vector<Caravana *> caravanas;

public:
    Cidade(char id, int x, int y);

    char getId() const override;
    void adicionarCaravana(Caravana *caravana);
    void removerCaravana(char id);
    void clearCaravanas();
    void list() const override;
};

#endif // CIDADE_H
