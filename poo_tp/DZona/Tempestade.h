//
// Created by adgp on 24-Dec-24.
//

#ifndef TEMPESTADE_H
#define TEMPESTADE_H

class Mapa;

class Tempestade
{
    static int id;
    int x, y;
    int raio;
    int duracao;

public:
    Tempestade(int x, int y, int raio);
    int getX() const;
    int getY() const;
    int getId() const;
    int getRaio() const;
    void atualizar(Mapa *mapa);
};

#endif // TEMPESTADE_H
