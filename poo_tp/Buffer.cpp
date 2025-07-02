//
// Created by adgp on 25-Dec-24.
//

#include "Buffer.h"
#include "Simulacao.h"
#include "DCaravana/Barbaro.h"

Buffer::Buffer(int linhas, int colunas, Simulacao *simulacao)
    : linhas(linhas), colunas(colunas), simulacao(simulacao), cursorX(0), cursorY(0)
{
    grelha = new char *[linhas];
    for (int i = 0; i < linhas; i++)
    {
        grelha[i] = new char[colunas];
        for (int j = 0; j < colunas; j++)
        {
            grelha[i][j] = ' ';
        }
    }
}

Buffer::~Buffer()
{
    for (int i = 0; i < linhas; i++)
    {
        delete[] grelha[i];
    }
    delete[] grelha;
}

void Buffer::esvaziar()
{
    for (int i = 0; i < linhas; i++)
    {
        for (int j = 0; j < colunas; j++)
        {
            grelha[i][j] = '.';
        }
    }
    cursorX = 0;
    cursorY = 0;
}

void Buffer::print()
{
    for (int i = 0; i < linhas; i++)
    {
        for (int j = 0; j < colunas; j++)
        {
            cout << grelha[i][j];
        }
        cout << endl;
    }
}

void Buffer::moveCursor(int linha, int coluna)
{
    if (linha >= 0 && linha < linhas && coluna >= 0 && coluna < colunas)
    {
        cursorX = linha;
        cursorY = coluna;
    }
    else
    {
        cout << "Erro: Posicao do cursor fora dos limites do buffer" << endl;
    }
}

void Buffer::imprimirChar(char c)
{
    if (cursorX >= 0 && cursorX < linhas && cursorY >= 0 && cursorY < colunas)
    {
        grelha[cursorX][cursorY] = c;
        cursorY++;
        if (cursorY >= colunas)
        {
            cursorY = 0;
            cursorX++;
        }
    }
}

void Buffer::imprimirString(const string &s)
{
    for (char c : s)
    {
        imprimirChar(c);
    }
}

void Buffer::imprimirInt(int num)
{
    string str = to_string(num);
    imprimirString(str);
}

void Buffer::saveBuffer(string name)
{
    char **copy = new char *[linhas];
    for (int i = 0; i < linhas; i++)
    {
        copy[i] = new char[colunas];
        for (int j = 0; j < colunas; j++)
        {
            copy[i][j] = grelha[i][j];
        }
    }
    copias[name] = copy;
}

void Buffer::showBuffer(string name)
{
    auto it = copias.find(name);
    if (it != copias.end())
    {
        char **copy = it->second;
        for (int i = 0; i < linhas; i++)
        {
            for (int j = 0; j < colunas; j++)
            {
                cout << copy[i][j];
            }
            cout << endl;
        }
    }
    else
    {
        cout << "Erro: Copia do buffer nao encontrada" << endl;
    }
}

void Buffer::deleteBuffer(string name)
{
    auto it = copias.find(name);
    if (it != copias.end())
    {
        char **copy = it->second;
        for (int i = 0; i < linhas; i++)
        {
            delete[] copy[i];
        }
        delete[] copy;
        copias.erase(it);
    }
    else
    {
        cout << "Erro: Copia do buffer nao encontrada" << endl;
    }
}

void Buffer::listBufferCopies()
{
    cout << "Copias de buffer:" << endl;
    for (const auto &pair : copias)
    {
        cout << pair.first << endl;
    }
}

void Buffer::updateBuffer(char **mapaGrelha)
{
    esvaziar();
    for (int i = 0; i < linhas; i++)
    {
        for (int j = 0; j < colunas; j++)
        {
            char c = mapaGrelha[i][j];
            imprimirChar(c);
        }
    }
}

Buffer &Buffer::operator<<(char c)
{
    imprimirChar(c);
    return *this;
}

Buffer &Buffer::operator<<(const string &s)
{
    imprimirString(s);
    return *this;
}

Buffer &Buffer::operator<<(int num)
{
    imprimirInt(num);
    return *this;
}

Buffer &Buffer::operator<<(Caravana &caravana)
{
    int x = caravana.getX();
    int y = caravana.getY();
    if (x >= 0 && x < linhas && y >= 0 && y < colunas)
    {
        moveCursor(x, y);
        if (caravana.getTipo() == '!')
        {
            // grelha[x][y] = '!';
            imprimirChar('!');
        }
        else
        {
            // grelha[x][y] = caravana.getId() + '0';
            imprimirChar(caravana.getId() + '0');
        }
    }
    return *this;
}

Buffer &Buffer::operator<<(Zona &zona)
{
    int x = zona.getX();
    int y = zona.getY();
    if (x >= 0 && x < linhas && y >= 0 && y < colunas)
    {
        // grelha[x][y] = zona.getId();
        moveCursor(x, y);
        imprimirChar(zona.getId());
    }
    return *this;
}

Buffer &Buffer::operator<<(Item &item)
{
    int x = item.getX();
    int y = item.getY();
    if (x >= 0 && x < linhas && y >= 0 && y < colunas && item.mVisivel())
    {
        // grelha[x][y] = 'I';
        moveCursor(x, y);
        imprimirChar('I');
    }
    return *this;
}