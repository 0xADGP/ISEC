//
// Created by adgp on 18-Dec-24.
//

#include "Interface.h"

#include "Buffer.h"
#include "Simulacao.h"

Interface::Interface(Simulacao *sim) : simulacao(sim), faseAtual(1) {}

void Interface::iniciar()
{
    while (true)
    {
        lerComandos();
        if (faseAtual == 2)
        {
            simulacao->atualizarMapa();
            // simulacao->print();
        }
    }
}

int Interface::getFaseAtual() const
{
    return faseAtual;
}

void Interface::atualizarFase()
{
    faseAtual = 2;
}

void Interface::lerComandos()
{
    string linhaComando;
    cout << "> ";
    getline(cin, linhaComando);

    if (!linhaComando.empty())
    {
        lerComandos(linhaComando);
    }
}

void Interface::lerComandos(const string &comando)
{
    istringstream iss(comando);
    string comandoPrincipal;
    iss >> comandoPrincipal;

    if (faseAtual == 1)
    {
        if (comandoPrincipal == "config")
        {
            string ficheiro;
            iss >> ficheiro;
            if (ficheiro.empty())
            {
                cout << "Erro: Nome do ficheiro nao especificado\n";
            }
            else
            {
                if (simulacao->carregarConfiguracao(ficheiro) == 1)
                {
                    faseAtual = 2;
                    simulacao->print();
                }
            }
        }
        else if (comandoPrincipal == "sair")
        {
            cout << "Saindo do programa\n";
            exit(0);
        }
        else
        {
            cout << "Erro: Comando invalido na Fase 1\n";
        }
    }
    else if (faseAtual == 2)
    {
        if (comandoPrincipal == "exec")
        {
            string ficheiro;
            iss >> ficheiro;
            if (ficheiro.empty())
            {
                cout << "Erro: Nome do ficheiro nao especificado" << endl;
            }
            else
            {
                if (simulacao)
                {
                    simulacao->executarComandosDoFicheiro(ficheiro);
                }
                else
                {
                    cout << "Erro: 'simulacao' não foi inicializada" << endl;
                }
            }
        }
        else if (comandoPrincipal == "prox")
        {
            int n = 1;
            iss >> n;
            if (n <= 0)
            {
                cout << "Erro: Numero de instantes invalido\n";
            }
            else
            {
                simulacao->avancarInstantes(n);
            }
        }
        else if (comandoPrincipal == "comprac")
        {
            char cidade, tipoCaravana;
            iss >> cidade >> tipoCaravana;
            if (!isalpha(cidade) || (tipoCaravana != 'C' && tipoCaravana != 'M' && tipoCaravana != 'S'))
            {
                cout << "Erro: Sintaxe invalida. Uso: comprac <C> <T>\n";
            }
            else
            {
                simulacao->comprarCaravana(cidade, tipoCaravana);
            }
        }
        else if (comandoPrincipal == "precos")
        {
            cout << "Preço venda mercadoria: " << simulacao->getConfiguracao("preço_venda_mercadoria") << endl;
            cout << "Preço compra mercadoria: " << simulacao->getConfiguracao("preço_compra_mercadoria") << endl;
        }
        else if (comandoPrincipal == "cidade")
        {
            char cidade;
            iss >> cidade;
            if (!iss || !isalpha(cidade))
            {
                cout << "Erro: Sintaxe invalida. Uso: cidade <C>\n";
            }
            else
            {
                simulacao->listCidade(cidade);
            }
        }
        else if (comandoPrincipal == "caravana")
        {
            int id;
            iss >> id;
            if (!iss || id < 0)
            {
                cout << "Erro: Sintaxe invalida. Uso: caravana <C>\n";
            }
            else
            {
                simulacao->listCaravana(id);
            }
        }
        else if (comandoPrincipal == "compra")
        {
            int id, quantidade;
            iss >> id >> quantidade;
            if (!iss || id < 0 || quantidade <= 0)
            {
                cout << "Erro: Sintaxe invalida. Uso: compra <C> <Q>\n";
            }
            else
            {
                simulacao->adicionaCargaCaravana(id, quantidade);
            }
        }
        else if (comandoPrincipal == "vende")
        {
            int id;
            iss >> id;
            if (!iss || id < 0)
            {
                cout << "Erro: Sintaxe invalida. Uso: vende <C>\n";
            }
            else
            {
                simulacao->venderCargaCaravana(id);
            }
        }
        else if (comandoPrincipal == "move")
        {
            int id;
            string direcao;
            iss >> id >> direcao;
            if (!iss || id < 0 || (direcao != "D" && direcao != "E" && direcao != "C" && direcao != "B" && direcao != "CE" && direcao != "CD" && direcao != "BE" && direcao != "BD"))
            {
                cout << "Erro: Sintaxe invalida. Uso: move <C> <D>\n";
            }
            else
            {
                simulacao->moverCaravana(id, direcao);
            }
        }
        else if (comandoPrincipal == "auto")
        {
            int id;
            iss >> id;
            if (!iss || id < 0)
            {
                cout << "Erro: Sintaxe invalida. Uso: auto <C>\n";
            }
            else
            {
                simulacao->ativarAutoGestao(id);
            }
        }
        else if (comandoPrincipal == "stop")
        {
            int id;
            iss >> id;
            if (!iss || id < 0)
            {
                cout << "Erro: Sintaxe invalida. Uso: stop <C>\n";
            }
            else
            {
                simulacao->desativarAutoGestao(id);
            }
        }
        else if (comandoPrincipal == "barbaro")
        {
            int l, c;
            iss >> l >> c;
            if (!iss || l < 0 || c < 0)
            {
                cout << "Erro: Sintaxe invalida. Uso: barbaro <L> <C>\n";
            }
            else
            {
                simulacao->adicionarBarbaro(l, c);
            }
        }
        else if (comandoPrincipal == "areia")
        {
            int l, c, r;
            iss >> l >> c >> r;
            if (!iss || l < 0 || c < 0 || r <= 0)
            {
                cout << "Erro: Sintaxe invalida. Uso: areia <L> <C> <R>\n";
            }
            else
            {
                simulacao->adicionarTempestade(l, c, r);
            }
        }
        else if (comandoPrincipal == "moedas")
        {
            int n;
            iss >> n;
            if (!iss)
            {
                cout << "Erro: Sintaxe invalida. Uso: moedas <N>\n";
            }
            else
            {
                simulacao->adicionarMoedas(n);
            }
        }
        else if (comandoPrincipal == "tripul")
        {
            int id, n;
            iss >> id >> n;
            if (!iss || id < 0 || n <= 0)
            {
                cout << "Erro: Sintaxe invalida. Uso: tripul <C> <T>\n";
            }
            else
            {
                simulacao->adicionarTripulantes(id, n);
            }
        }
        else if (comandoPrincipal == "saves")
        {
            string nome;
            iss >> nome;
            if (nome.empty())
            {
                cout << "Erro: Nome nao especificado\n";
            }
            else
            {
                simulacao->getBuffer()->saveBuffer(nome);
            }
        }
        else if (comandoPrincipal == "loads")
        {
            string nome;
            iss >> nome;
            if (nome.empty())
            {
                cout << "Erro: Nome nao especificado\n";
            }
            else
            {
                simulacao->getBuffer()->showBuffer(nome);
            }
        }
        else if (comandoPrincipal == "lists")
        {
            simulacao->getBuffer()->listBufferCopies();
        }
        else if (comandoPrincipal == "dels")
        {
            string nome;
            iss >> nome;
            if (nome.empty())
            {
                cout << "Erro: Nome nao especificado\n";
            }
            else
            {
                simulacao->getBuffer()->deleteBuffer(nome);
            }
        }
        else if (comandoPrincipal == "terminar")
        {
            simulacao->terminar();
            faseAtual = 1;
        }
        else if (comandoPrincipal == "debug")
        {
            simulacao->printDebug();
        }
        else
        {
            cout << "Erro: Comando invalido na Fase 2\n";
        }
    }
}
