//
// Created by adgp on 18-Dec-24.
//

#ifndef INTERFACE_H
#define INTERFACE_H

#include <string>
#include <sstream>

using namespace std;

class Simulacao;

class Interface
{
    Simulacao *simulacao;
    int faseAtual;

public:
    Interface(Simulacao *sim);

    void iniciar();
    void atualizarFase();
    int getFaseAtual() const;
    void lerComandos();
    void lerComandos(const string &comando);
};

#endif // INTERFACE_H
