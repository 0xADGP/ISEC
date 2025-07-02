//
// Created by adgp on 18-Dec-24.
//

#ifndef MAPA_H
#define MAPA_H

#include <iostream>
#include <vector>

#include "DCaravana/Caravana.h"
#include "DItem/Item.h"
#include "DZona/Tempestade.h"
#include "DZona/Zona.h"

using namespace std;

class Simulacao;

struct Posicao
{
    int x, y;
};

class Mapa
{
    int linhas, colunas;
    char **grelha;
    Simulacao *simulacao;

    vector<Caravana *> caravanas;
    vector<Caravana *> barbaros;
    vector<Zona *> cidades;
    vector<Zona *> montanhas;
    vector<Item *> items;
    vector<Tempestade *> tempestades;

public:
    Mapa(int linhas, int colunas, Simulacao *simulacao);
    ~Mapa();

    void setGridCell(int linha, int coluna, char valor);

    void atualizarEstado();
    void atualizarEstado(int id);

    void print();
    void atualizarMapa();

    int getTurnosSimulacao() const;

    vector<Caravana *> getCaravanas() const;
    vector<Caravana *> getBarbaros() const;
    vector<Item *> getItems() const;

    int getAvailableCaravanaId() const;
    void mostrarCaravanas() const;
    void adicionarCaravana(Caravana *caravana);
    void removerCaravana(int id);

    void fazSurpresa(int x, int y);

    bool existeBarbaro(int id);

    void removerBarbaro(int id);

    void adicionarZona(Zona *zona, char tipo);

    void adicionarBarbaro(int x, int y);

    void adicionarItem(string nome, int x, int y);
    void removerItem(int id);

    void comprarCaravana(char idCidade, char tipo);

    Caravana *getCaravanaById(int id);

    Posicao getRandomEmptyPosition() const;

    void listCidade(char c);

    void atualizarCidadesCaravanas();

    void listCaravana(int id);

    void adicionaMoedasSimulacao(int moedas);
    int getMoedasSimulacao() const;

    void adicionaCargaCaravana(int id, int carga);

    bool isAffectedByTempestade(Tempestade *tempestade, int x, int y);

    void venderCargaCaravana(int id);

    void moverCaravana(int id, string direcao);

    bool isPositionEmpty(int x, int y);

    bool isTransitable(int x, int y);

    bool isCidade(int x, int y);

    void criarTempestade(int x, int y, int raio);
    void apagarTempestade(int id);

    void ativarAutoGestao(int id);
    void desativarAutoGestao(int id);

    void adicionarTripulantes(int id, int n);

    void afetarCaravanasTempestades(Tempestade *tempestade);

    void printDebug();

    void ganharCombate();

    int getLinhas() const;
    int getColunas() const;
};

#endif // MAPA_H
