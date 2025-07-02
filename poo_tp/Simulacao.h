//
// Created by adgp on 18-Dec-24.
//

#ifndef SIMULACAO_H
#define SIMULACAO_H

#include <iostream>
#include <map>
#include <fstream>
#include <cstdlib>
#include <ctime>

#include "DCaravana/Caravana.h"
#include "DZona/Zona.h"

using namespace std;

class Interface;
class Mapa;
class Buffer;

class Simulacao
{
    Mapa *mapa = nullptr;
    Buffer *buffer = nullptr;
    Interface *interface;
    int turnos, vencidos;
    map<string, int> configuracao;
    bool configuracaoCarregada;

public:
    Simulacao();
    ~Simulacao();

    void print();

    void criarMapa(int linhas, int colunas);
    int carregarConfiguracao(const string &ficheiro);
    void handleCell(char cell, int x, int y);

    void executarComandosDoFicheiro(const string &ficheiro);
    void adicionarMoedas(int n);

    int getConfiguracao(const string &key) const;

    void avancarInstantes(int n);

    void comprarCaravana(char idCidade, char tipo);

    void adicionarBarbaro(int x, int y);

    void listCidade(char c);

    void removerCaravana(int id);

    void listCaravana(int id);

    void adicionaCargaCaravana(int id, int carga);

    void adicionarAoBuffer(Caravana *caravana);
    void adicionarAoBuffer(Zona *zona);

    void venderCargaCaravana(int id);

    void moverCaravana(int id, string direcao);

    int getTurnos();

    Buffer *getBuffer();
    void updateBuffer(char **grelha);

    void atualizarMapa();

    void terminar();

    void ativarAutoGestao(int id);
    void desativarAutoGestao(int id);

    void adicionarTempestade(int x, int y, int raio);

    void adicionarTripulantes(int id, int n);

    void printDebug();

    void vencerCombate();
};

#endif // SIMULACAO_H
