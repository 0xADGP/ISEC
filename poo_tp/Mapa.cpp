//
// Created by adgp on 18-Dec-24.
//

#include "Mapa.h"
#include "Buffer.h"
#include "Simulacao.h"
#include "DCaravana/Barbaro.h"
#include "DCaravana/Comercio.h"
#include "DCaravana/Militar.h"
#include "DCaravana/Secreta.h"
#include "DItem/Jaula.h"
#include "DItem/Mina.h"
#include "DItem/Pandora.h"
#include "DItem/Surpresa.h"
#include "DItem/Tesouro.h"
#include "DZona/Cidade.h"

Mapa::Mapa(int linhas, int colunas, Simulacao *simulacao) : simulacao(simulacao)
{
    this->linhas = linhas;
    this->colunas = colunas;
    grelha = new char *[linhas];
    for (int i = 0; i < linhas; i++)
    {
        grelha[i] = new char[colunas];
        for (int j = 0; j < colunas; j++)
        {
            grelha[i][j] = '.';
        }
    }
}

Mapa::~Mapa()
{
    for (int i = 0; i < linhas; i++)
    {
        delete[] grelha[i];
    }
    delete[] grelha;

    for (auto caravana : caravanas)
    {
        delete caravana;
    }

    for (auto barbaro : barbaros)
    {
        delete barbaro;
    }

    for (auto cidade : cidades)
    {
        delete cidade;
    }

    for (auto montanha : montanhas)
    {
        delete montanha;
    }

    for (auto item : items)
    {
        delete item;
    }

    for (auto tempestade : tempestades)
    {
        delete tempestade;
    }
}

void Mapa::print()
{
    atualizarMapa();
    cout << "Mapa:" << endl;
    for (int i = 0; i < linhas; i++)
    {
        for (int j = 0; j < colunas; j++)
        {
            cout << grelha[i][j];
        }
        cout << endl;
    }
}

void Mapa::atualizarMapa()
{
    /*for (int i = 0; i < linhas; i++)
    {
        for (int j = 0; j < colunas; j++)
        {
            grelha[i][j] = '.';
        }
    }

    for (auto caravana : caravanas)
    {
        grelha[caravana->getX()][caravana->getY()] = caravana->getId() + '0';
    }

    for (auto barbaro : barbaros)
    {
        grelha[barbaro->getX()][barbaro->getY()] = '!';
    }

    for (auto cidade : cidades)
    {
        grelha[cidade->getX()][cidade->getY()] = cidade->getId();
    }

    for (auto montanha : montanhas)
    {
        grelha[montanha->getX()][montanha->getY()] = montanha->getId();
    }

    simulacao->updateBuffer(grelha);*/

    simulacao->getBuffer()->esvaziar();

    for (auto caravana : caravanas)
    {
        simulacao->getBuffer()->operator<<(*caravana);
    }

    for (auto barbaro : barbaros)
    {
        simulacao->getBuffer()->operator<<(*barbaro);
    }

    for (auto cidade : cidades)
    {
        simulacao->getBuffer()->operator<<(*cidade);
    }

    for (auto montanha : montanhas)
    {
        simulacao->getBuffer()->operator<<(*montanha);
    }

    for (auto item : items)
    {
        simulacao->getBuffer()->operator<<(*item);
    }
}

vector<Caravana *> Mapa::getCaravanas() const
{
    return caravanas;
}

vector<Caravana *> Mapa::getBarbaros() const
{
    return barbaros;
}

bool Mapa::existeBarbaro(int id)
{
    for (auto barbaro : barbaros)
    {
        if (barbaro->getId() == id)
        {
            return true;
        }
    }
    return false;
}

vector<Item *> Mapa::getItems() const
{
    return items;
}

int Mapa::getAvailableCaravanaId() const
{
    int id = 1;
    for (const auto &caravana : caravanas)
    {
        if (caravana->getId() == id)
        {
            id++;
        }
    }
    return id;
}

void Mapa::atualizarEstado()
{
    if (simulacao->getTurnos() % simulacao->getConfiguracao("instantes_entre_novos_itens") == 0)
    {
        Posicao posicao = getRandomEmptyPosition();
        int random = rand() % 5;
        string nome;
        if (random == 0)
        {
            nome = "surpresa";
        }
        else if (random == 1)
        {
            nome = "jaula";
        }
        else if (random == 2)
        {
            nome = "tesouro";
        }
        else if (random == 3)
        {
            nome = "pandora";
        }
        else if (random == 4)
        {
            nome = "mina";
        }
        adicionarItem(nome, posicao.x, posicao.y);
    }

    if (simulacao->getTurnos() % simulacao->getConfiguracao("instantes_entre_novos_barbaros") == 0)
    {
        Posicao posicao = getRandomEmptyPosition();
        adicionarBarbaro(posicao.x, posicao.y);
    }

    for (auto caravana : caravanas)
    {
        caravana->atualizar(this);
    }

    for (auto item : items)
    {
        item->atualizar(this);
    }

    for (auto barbaro : barbaros)
    {
        barbaro->atualizar(this);
    }

    for (auto tempestade : tempestades)
    {
        tempestade->atualizar(this);
    }

    atualizarCidadesCaravanas();

    atualizarMapa();

    simulacao->print();
}

void Mapa::atualizarEstado(int id)
{
    if (simulacao->getTurnos() % simulacao->getConfiguracao("instantes_entre_novos_itens") == 0)
    {
        Posicao posicao = getRandomEmptyPosition();
        int random = rand() % 5;
        string nome;
        if (random == 0)
        {
            nome = "surpresa";
        }
        else if (random == 1)
        {
            nome = "jaula";
        }
        else if (random == 2)
        {
            nome = "tesouro";
        }
        else if (random == 3)
        {
            nome = "pandora";
        }
        else if (random == 4)
        {
            nome = "mina";
        }
        adicionarItem(nome, posicao.x, posicao.y);
    }

    if (simulacao->getTurnos() % simulacao->getConfiguracao("instantes_entre_novos_barbaros") == 0)
    {
        Posicao posicao = getRandomEmptyPosition();
        adicionarBarbaro(posicao.x, posicao.y);
    }

    for (auto caravana : caravanas)
    {
        if (caravana->getId() != id)
        {
            caravana->atualizar(this);
        }
        else
        {
            caravana->atualizarNoMove(this);
        }
    }

    for (auto item : items)
    {
        item->atualizar(this);
    }

    for (auto barbaro : barbaros)
    {
        barbaro->atualizar(this);
    }

    for (auto tempestade : tempestades)
    {
        tempestade->atualizar(this);
    }

    atualizarCidadesCaravanas();

    atualizarMapa();

    simulacao->print();
}

void Mapa::afetarCaravanasTempestades(Tempestade *tempestade)
{
    for (auto caravana : caravanas)
    {
        if (isAffectedByTempestade(tempestade, caravana->getX(), caravana->getY()))
        {
            caravana->afetarTempestade(this);
        }
    }
}

bool Mapa::isAffectedByTempestade(Tempestade *tempestade, int x, int y)
{
    int linhas = getLinhas();
    int colunas = getColunas();

    int centerX = tempestade->getX();
    int centerY = tempestade->getY();
    int raio = tempestade->getRaio();

    int deltaX = abs(x - centerX);
    int deltaY = abs(y - centerY);

    if (deltaX > linhas / 2)
        deltaX = linhas - deltaX;
    if (deltaY > colunas / 2)
        deltaY = colunas - deltaY;

    return deltaX <= raio && deltaY <= raio;
}

void Mapa::criarTempestade(int x, int y, int raio)
{
    Tempestade *tempestade = new Tempestade(x, y, raio);
    tempestades.push_back(tempestade);
}

void Mapa::apagarTempestade(int id)
{
    for (auto tempestade = tempestades.begin(); tempestade != tempestades.end(); ++tempestade)
    {
        if ((*tempestade)->getId() == id)
        {
            tempestades.erase(tempestade);
            break;
        }
    }
}

void Mapa::setGridCell(int linha, int coluna, char valor)
{
    if (linha >= 0 && linha < linhas && coluna >= 0 && coluna < colunas)
    {
        grelha[linha][coluna] = valor;
    }
}

void Mapa::mostrarCaravanas() const
{
    cout << "Caravanas no mapa: " << endl;
    for (const auto &caravana : caravanas)
    {
        cout << "Caravana " << caravana->getTipo() << " id: " << caravana->getId() << " na posição (" << caravana->getX() << ", " << caravana->getY() << ")\n";
    }
}

void Mapa::adicionaMoedasSimulacao(int moedas)
{
    simulacao->adicionarMoedas(moedas);
}

void Mapa::adicionarCaravana(Caravana *caravana)
{
    if (caravana->getId() > 9)
    {
        simulacao->adicionarMoedas(simulacao->getConfiguracao("preço_caravana"));
        delete caravana;
        return;
    }
    caravanas.push_back(caravana);
}

void Mapa::adicionarItem(string nome, int x, int y)
{
    if (items.size() < simulacao->getConfiguracao("max_itens"))
    {
        Item *item = nullptr;
        if (nome == "surpresa")
        {
            item = new Surpresa(x, y, simulacao->getConfiguracao("duração_item"));
        }
        else if (nome == "jaula")
        {
            item = new Jaula(x, y, simulacao->getConfiguracao("duração_item"));
        }
        else if (nome == "tesouro")
        {
            item = new Tesouro(x, y, simulacao->getConfiguracao("duração_item"));
        }
        else if (nome == "pandora")
        {
            item = new Pandora(x, y, simulacao->getConfiguracao("duração_item"));
        }
        else if (nome == "mina")
        {
            item = new Mina(x, y, simulacao->getConfiguracao("duração_item"));
        }
        items.push_back(item);
    }
}

void Mapa::removerCaravana(int id)
{
    for (auto it = caravanas.begin(); it != caravanas.end();)
    {
        if (*it != nullptr && (*it)->getId() == id)
        {
            it = caravanas.erase(it);
        }
        else
        {
            it++;
        }
    }
}

void Mapa::removerBarbaro(int id)
{
    for (auto it = barbaros.begin(); it != barbaros.end();)
    {
        if (*it != nullptr && (*it)->getId() == id)
        {
            it = barbaros.erase(it);
        }
        else
        {
            it++;
        }
    }
}

void Mapa::removerItem(int id)
{
    for (auto item = items.begin(); item != items.end();)
    {
        if (*item != nullptr && (*item)->getId() == id)
        {
            item = items.erase(item);
        }
        else
        {
            item++;
        }
    }
}

void Mapa::adicionarBarbaro(int x, int y)
{
    Caravana *caravana = new Barbaro(x, y, simulacao->getConfiguracao("duração_barbaros"));
    barbaros.push_back(caravana);
}

void Mapa::adicionarZona(Zona *zona, char tipo)
{
    if (tipo == 'C')
    {
        cidades.push_back(zona);
    }
    else if (tipo == 'M')
    {
        montanhas.push_back(zona);
    }
}

void Mapa::comprarCaravana(char idCidade, char tipo)
{
    for (auto cidade : cidades)
    {
        if (cidade->getId() == idCidade)
        {
            if (simulacao->getConfiguracao("moedas") >= simulacao->getConfiguracao("preço_caravana"))
            {
                simulacao->adicionarMoedas(-simulacao->getConfiguracao("preço_caravana"));
                Caravana *caravana = nullptr;

                if (tipo == 'C')
                {
                    caravana = new Comercio(getAvailableCaravanaId(), cidade->getX(), cidade->getY());
                }
                else if (tipo == 'M')
                {
                    caravana = new Militar(getAvailableCaravanaId(), cidade->getX(), cidade->getY());
                }
                else if (tipo == 'S')
                {
                    caravana = new Secreta(getAvailableCaravanaId(), cidade->getX(), cidade->getY());
                }

                if (caravana != nullptr)
                {
                    cidade->adicionarCaravana(caravana);
                    adicionarCaravana(caravana);
                }
            }
        }
    }
}

bool Mapa::isCidade(int x, int y)
{
    for (auto cidade : cidades)
    {
        if (cidade->getX() == x && cidade->getY() == y)
        {
            return true;
        }
    }
    return false;
}

int Mapa::getMoedasSimulacao() const
{
    return simulacao->getConfiguracao("moedas");
}

Posicao Mapa::getRandomEmptyPosition() const
{
    Posicao posicao;
    do
    {
        posicao.x = rand() % linhas;
        posicao.y = rand() % colunas;
    } while (grelha[posicao.x][posicao.y] != '.');
    return posicao;
}

bool Mapa::isPositionEmpty(int x, int y)
{
    if (x < 0 || x >= getLinhas() || y < 0 || y >= getColunas())
    {
        return false;
    }
    return grelha[x][y] == '.';
}

void Mapa::fazSurpresa(int x, int y)
{
    int chance = rand() % 100;
    if (chance < 33)
    {
        int coins = 0;
        int coinChance = rand() % 100;
        if (coinChance < 50)
        {
            coins = rand() % 101 + 100;
        }
        else if (coinChance < 80)
        {
            coins = rand() % 101 + 201;
        }
        else if (coinChance < 95)
        {
            coins = rand() % 101 + 301;
        }
        else if (coinChance < 99)
        {
            coins = rand() % 101 + 401;
        }
        simulacao->adicionarMoedas(coins);
        cout << "Caixa surpresa " << coins << " moedas\n";
    }
    else if (chance < 66)
    {
        int raio = rand() % 4 + 2;
        criarTempestade(x, y, raio);
        cout << "Caixa surpresa tempestade " << x << " " << y << " " << raio << "\n";
    }
    else
    {
        cout << "Caixa surpresa vazia\n";
    }
}

int Mapa::getTurnosSimulacao() const
{
    return simulacao->getTurnos();
}

void Mapa::listCidade(char c)
{
    for (auto cidade : cidades)
    {
        if (cidade->getId() == c)
        {
            cidade->list();
        }
    }
}

void Mapa::listCaravana(int id)
{
    for (auto caravana : caravanas)
    {
        if (caravana->getId() == id)
        {
            caravana->list();
        }
    }
}

void Mapa::adicionaCargaCaravana(int id, int carga)
{
    for (auto caravana : caravanas)
    {
        if (caravana->getId() == id && isCidade(caravana->getX(), caravana->getY()))
        {
            if (carga * simulacao->getConfiguracao("preço_compra_mercadoria") <= simulacao->getConfiguracao("moedas"))
            {
                if (caravana->adicionarCarga(carga) == 1)
                {
                    simulacao->adicionarMoedas(-carga * simulacao->getConfiguracao("preço_compra_mercadoria"));
                }
            }
        }
    }
}

void Mapa::adicionarTripulantes(int id, int n)
{
    for (auto caravana : caravanas)
    {
        if (n <= simulacao->getConfiguracao("moedas"))
        {
            if (caravana->getId() == id && isCidade(caravana->getX(), caravana->getY()))
            {
                if (caravana->adicionarTripulantes(n) == 1)
                {
                    simulacao->adicionarMoedas(-n);
                }
            }
        }
    }
}

void Mapa::venderCargaCaravana(int id)
{
    for (auto caravana : caravanas)
    {
        if (caravana->getId() == id && isCidade(caravana->getX(), caravana->getY()))
        {
            simulacao->adicionarMoedas(caravana->getCarga() * simulacao->getConfiguracao("preço_venda_mercadoria"));
            caravana->setCarga(0);
        }
    }
}

void Mapa::ativarAutoGestao(int id)
{
    for (auto caravana : caravanas)
    {
        if (caravana->getId() == id)
        {
            caravana->setAuto(true);
        }
    }
}

void Mapa::desativarAutoGestao(int id)
{
    for (auto caravana : caravanas)
    {
        if (caravana->getId() == id)
        {
            caravana->setAuto(false);
        }
    }
}

void Mapa::ganharCombate()
{
    simulacao->vencerCombate();
}

void Mapa::atualizarCidadesCaravanas()
{
    for (auto cidade : cidades)
    {
        cidade->clearCaravanas();
        for (auto caravana : caravanas)
        {
            if (cidade->getX() == caravana->getX() && cidade->getY() == caravana->getY())
            {
                cidade->adicionarCaravana(caravana);
                caravana->fillAgua();
            }
        }
    }
}

void Mapa::moverCaravana(int id, string direcao)
{
    for (auto caravana : caravanas)
    {
        if (caravana->getId() == id)
        {
            caravana->mover(direcao, this);
            atualizarEstado(id);
        }
    }
}

bool Mapa::isTransitable(int x, int y)
{
    for (auto cidade : cidades)
    {
        if (cidade->getX() == x && cidade->getY() == y)
        {
            return true;
        }
    }
    for (auto montanha : montanhas)
    {
        if (montanha->getX() == x && montanha->getY() == y)
        {
            return false;
        }
    }
    for (auto caravana : caravanas)
    {
        if (caravana->getX() == x && caravana->getY() == y)
        {
            return false;
        }
    }
    for (auto barbaro : barbaros)
    {
        if (barbaro->getX() == x && barbaro->getY() == y)
        {
            return false;
        }
    }
    return true;
}

Caravana *Mapa::getCaravanaById(int id)
{
    for (auto caravana : caravanas)
    {
        if (caravana->getId() == id)
        {
            return caravana;
        }
    }
    return nullptr;
}

void Mapa::printDebug()
{
    cout << "Caravanas: " << caravanas.size() << endl;
    for (auto caravana : caravanas)
    {
        cout << "Caravana id: " << caravana->getId() << "/" << caravana->getTipo() << " na posicao (" << caravana->getX() << ", " << caravana->getY() << ")\n";
    }
    cout << "Barbaros: " << barbaros.size() << endl;
    for (auto barbaro : barbaros)
    {
        cout << "Barbaro na posicao (" << barbaro->getX() << ", " << barbaro->getY() << ")\n";
    }
    cout << "Cidades: " << cidades.size() << endl;
    for (auto cidade : cidades)
    {
        cout << "Cidade " << cidade->getId() << " na posicao (" << cidade->getX() << ", " << cidade->getY() << ")\n";
    }
    cout << "Items: " << items.size() << endl;
    for (auto item : items)
    {
        cout << "Item " << item->getNome() << " na posicao (" << item->getX() << ", " << item->getY() << ")\n";
    }
    cout << "Tempestades: " << tempestades.size() << endl;
    for (auto tempestade : tempestades)
    {
        cout << "Tempestade na posicao (" << tempestade->getX() << ", " << tempestade->getY() << ")\n";
    }
    /*cout << "Montanhas: " << montanhas.size() << endl;
    for (auto montanha : montanhas) {
        cout << "Montanha na posição (" << montanha->getX() << ", " << montanha->getY() << ")\n";
    }*/
}

int Mapa::getLinhas() const
{
    return linhas;
}

int Mapa::getColunas() const
{
    return colunas;
}