//
// Created by adgp on 18-Dec-24.
//

#ifndef CARAVANA_H
#define CARAVANA_H

#include <iostream>
class Simulacao;
using namespace std;

class Mapa;

class Caravana
{
protected:
    Mapa *mapa;
    int id;
    int x, y;
    bool isAuto;

    int carga;
    int maxCarga;

    int agua;
    int maxAgua;

    int tripulantes;
    int maxTripulantes;
    int instantesSemTripulantes;

public:
    Caravana(int id, int x, int y);

    void fazEfeito(string efeito, Mapa *mapa);

    void killMyself(Mapa *mapa);

    // setters
    void setX(int x);
    void setY(int y);
    void setCarga(int carga);
    void setAuto(bool isAuto);
    void setAgua(int agua);
    void setTripulantes(int n);

    void fillAgua();

    // getters
    int getId();
    int getX();
    int getY();
    int getCarga();
    int getTripulantes();
    int getAgua();
    int getMaxAgua();
    int getMaxCarga();
    int getMaxTripulantes();

    int getRandomBetween1and100();

    void mover(string direcao, Mapa *mapa);
    int adicionarCarga(int carga);
    int adicionarTripulantes(int n);
    void moverRandomDirection(Mapa *mapa, int numSteps);
    void moverParaCoords(int x, int y, Mapa *mapa);
    void list();

    virtual void afetarTempestade(Mapa *mapa) = 0;
    virtual void atualizar(Mapa *mapa) = 0;
    virtual void atualizarNoMove(Mapa *mapa) = 0;
    virtual char getTipo() = 0;
};

#endif // CARAVANA_H
