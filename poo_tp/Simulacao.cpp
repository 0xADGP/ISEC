//
// Created by adgp on 18-Dec-24.
//

#include "Simulacao.h"
#include "Interface.h"
#include "Mapa.h"
#include "Buffer.h"
#include "DCaravana/Comercio.h"
#include "DCaravana/Militar.h"
#include "DCaravana/Secreta.h"
#include "DZona/Cidade.h"
#include "DZona/Montanha.h"

Simulacao::Simulacao() : interface(new Interface(this)), turnos(0), vencidos(0), mapa(nullptr), buffer(nullptr)
{
    srand(static_cast<unsigned>(time(0)));
}

Simulacao::~Simulacao()
{
    delete mapa;
    delete interface;
    delete buffer;
}

void Simulacao::print()
{
    if (mapa && buffer)
    {
        fflush(stdout);
        cout << "Estado Atual:" << endl
             << "Turno: " << turnos << " - Vencidos: " << vencidos << " - Moedas: " << configuracao["moedas"] << endl;
        mapa->atualizarMapa();
        buffer->print();
    }
}

Buffer *Simulacao::getBuffer()
{
    return buffer;
}

void Simulacao::updateBuffer(char **grelha)
{
    buffer->updateBuffer(grelha);
}

void Simulacao::atualizarMapa()
{
    mapa->atualizarMapa();
}

void Simulacao::criarMapa(int linhas, int colunas)
{
    if (mapa)
    {
        delete mapa;
    }
    if (buffer)
    {
        delete buffer;
    }
    buffer = new Buffer(linhas, colunas, this);
    mapa = new Mapa(linhas, colunas, this);
}

int Simulacao::carregarConfiguracao(const string &ficheiro)
{
    ifstream file(ficheiro);
    if (!file)
    {
        cout << "Erro: Nao foi possivel abrir o ficheiro " << ficheiro << endl;
        return 0;
    }

    string line;
    int linhas, colunas;

    map<string, int> defaultConfig = {
        {"moedas", 1000},
        {"instantes_entre_novos_itens", 10},
        {"duração_item", 20},
        {"max_itens", 5},
        {"preço_venda_mercadoria", 2},
        {"preço_compra_mercadoria", 1},
        {"preço_caravana", 100},
        {"instantes_entre_novos_barbaros", 40},
        {"duração_barbaros", 60}};

    if (!(file >> linhas >> colunas))
    {
        cout << "Erro: Linhas ou colunas invalidas" << endl;
        return 0;
    }
    file.ignore();

    vector<string> mapLines(linhas);
    for (int i = 0; i < linhas; ++i)
    {
        if (!getline(file, mapLines[i]))
        {
            cout << "Erro: Nao foi possivel ler a linha " << i + 1 << " do mapa" << endl;
            return 0;
        }
    }

    string key;
    int value;
    while (file >> key >> value)
    {
        configuracao[key] = value;
    }

    for (const auto &pair : defaultConfig)
    {
        if (configuracao.find(pair.first) == configuracao.end())
        {
            configuracao[pair.first] = pair.second;
            cout << "Configuracao padrao adicionada: " << pair.first << " = " << pair.second << endl;
        }
    }

    cout << "Configuracao carregada:" << endl;
    for (const auto &pair : configuracao)
    {
        cout << pair.first << " = " << pair.second << endl;
    }

    criarMapa(linhas, colunas);

    for (int i = 0; i < linhas; ++i)
    {
        const string &mapLine = mapLines[i];
        if (mapLine.size() != static_cast<size_t>(colunas))
        {
            cout << "Erro: Numero de colunas na linha " << i + 1 << " nao corresponde a " << colunas << endl;
            return 0;
        }

        for (int j = 0; j < colunas; ++j)
        {
            char cell = mapLine[j];
            handleCell(cell, i, j);
        }
    }

    turnos = 0;
    return 1;
}

void Simulacao::handleCell(char cell, int x, int y)
{
    if (cell == '+')
    {
        // cout << "Montanha em (" << x << ", " << y << ")" << endl;
        Montanha *montanha = new Montanha(x, y);
        mapa->adicionarZona(montanha, 'M');
    }
    else if (isalpha(cell))
    {
        // cout << "Cidade " << cell << " em (" << x << ", " << y << ")" << endl;
        Cidade *cidade = new Cidade(cell, x, y);
        mapa->adicionarZona(cidade, 'C');
    }
    else if (isdigit(cell))
    {
        // cout << "Caravana " << cell << " em (" << x << ", " << y << ")" << endl;
        int random = 1 + (rand() % 3);
        Caravana *caravana = nullptr;

        switch (random)
        {
        case 1:
            caravana = new Comercio(mapa->getAvailableCaravanaId(), x, y);
            break;
        case 2:
            caravana = new Militar(mapa->getAvailableCaravanaId(), x, y);
            break;
        case 3:
            caravana = new Secreta(mapa->getAvailableCaravanaId(), x, y);
            break;
        }
        if (caravana)
        {
            mapa->adicionarCaravana(caravana);
        }
    }
    else if (cell == '!')
    {
        // cout << "Barbara em (" << x << ", " << y << ")" << endl;
        mapa->adicionarBarbaro(x, y);
    }
    else if (cell != '.')
    {
        cout << "Aviso: Caracter desconhecido '" << cell << "' em (" << x << ", " << y << ")" << endl;
    }
}

void Simulacao::executarComandosDoFicheiro(const string &filename)
{
    ifstream file(filename);

    if (!file.is_open())
    {
        cout << "Erro: Nao foi possivel abrir o ficheiro " << filename << endl;
        return;
    }

    interface->atualizarFase();

    string linhaComando;
    while (getline(file, linhaComando))
    {
        if (linhaComando.empty())
            continue;

        cout << "> " << linhaComando << endl;

        if (interface->getFaseAtual() == 2)
        {
            interface->lerComandos(linhaComando);
        }
    }

    file.close();
}

void Simulacao::adicionarMoedas(int n)
{
    configuracao["moedas"] += n;
    if (configuracao["moedas"] < 0)
    {
        configuracao["moedas"] = 0;
    }
}

int Simulacao::getConfiguracao(const string &key) const
{
    auto it = configuracao.find(key);
    if (it != configuracao.end())
    {
        return it->second;
    }
    return -1;
}

void Simulacao::avancarInstantes(int n)
{
    for (int i = 0; i < n; ++i)
    {
        turnos++;
        mapa->atualizarEstado();
    }
}

void Simulacao::adicionarTripulantes(int id, int n)
{
    mapa->adicionarTripulantes(id, n);
}

int Simulacao::getTurnos()
{
    return turnos;
}

void Simulacao::comprarCaravana(char idCidade, char tipo)
{
    mapa->comprarCaravana(idCidade, tipo);
}

void Simulacao::removerCaravana(int id)
{
    mapa->removerCaravana(id);
}

void Simulacao::adicionarBarbaro(int x, int y)
{
    mapa->adicionarBarbaro(x, y);
}

void Simulacao::listCidade(char c)
{
    mapa->listCidade(c);
}

void Simulacao::listCaravana(int id)
{
    mapa->listCaravana(id);
}

void Simulacao::adicionarTempestade(int x, int y, int raio)
{
    mapa->criarTempestade(x, y, raio);
}

void Simulacao::adicionarAoBuffer(Caravana *caravana)
{
    //*buffer << *caravana;
    buffer->operator<<(*caravana);
}

void Simulacao::adicionarAoBuffer(Zona *zona)
{
    buffer->operator<<(*zona);
}

void Simulacao::adicionaCargaCaravana(int id, int carga)
{
    mapa->adicionaCargaCaravana(id, carga);
}

void Simulacao::venderCargaCaravana(int id)
{
    mapa->venderCargaCaravana(id);
}

void Simulacao::moverCaravana(int id, string direcao)
{
    mapa->moverCaravana(id, direcao);
    turnos++;
}

void Simulacao::ativarAutoGestao(int id)
{
    mapa->ativarAutoGestao(id);
}

void Simulacao::desativarAutoGestao(int id)
{
    mapa->desativarAutoGestao(id);
}

void Simulacao::terminar()
{
    cout << "Simulacao terminada." << endl;
    cout << "Turnos: " << turnos << endl;
    cout << "Combates vencidos: " << vencidos << endl;
    cout << "Moedas: " << configuracao["moedas"] << endl;
}

void Simulacao::printDebug()
{
    cout << "Turnos: " << turnos << endl;
    cout << "Configuracao:" << endl;
    for (const auto &pair : configuracao)
    {
        cout << pair.first << " = " << pair.second << endl;
    }
    mapa->printDebug();
}

void Simulacao::vencerCombate()
{
    vencidos++;
}