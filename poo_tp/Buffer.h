//
// Created by adgp on 25-Dec-24.
//

#ifndef BUFFER_H
#define BUFFER_H

#include <iostream>
#include <map>

#include "DCaravana/Caravana.h"
#include "DZona/Zona.h"
#include "DItem/Item.h"

using namespace std;

class Simulacao;

class Buffer
{
    int linhas, colunas;
    char **grelha;
    Simulacao *simulacao;
    map<string, char **> copias;
    int cursorX, cursorY;

public:
    Buffer(int linhas, int colunas, Simulacao *simulacao);
    ~Buffer();

    void esvaziar();
    void print();
    void moveCursor(int linha, int coluna);
    void imprimirChar(char c);
    void imprimirString(const string &s);
    void imprimirInt(int num);
    void saveBuffer(string name);
    void showBuffer(string name);
    void deleteBuffer(string name);
    void listBufferCopies();

    void updateBuffer(char **mapaGrelha);

    Buffer &operator<<(char c);
    Buffer &operator<<(const string &s);
    Buffer &operator<<(int num);
    Buffer &operator<<(Caravana &caravana);
    Buffer &operator<<(Zona &zona);
    Buffer &operator<<(Item &item);
};

#endif // BUFFER_H
