//
// Created by adgp on 22-Dec-24.
//

#ifndef MONTANHA_H
#define MONTANHA_H

#include "Zona.h"

class Montanha : public Zona
{
public:
    Montanha(int x, int y);
    char getId() const override;
    void list() const override;
};

#endif // MONTANHA_H
