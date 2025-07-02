//
// Created by adgp on 25-Dec-24.
//

#include "Combate.h"
#include "DCaravana/Caravana.h"
#include "Mapa.h"

void Combate::combate(Caravana *caravanaUser, Caravana *caravanaBarbara, Mapa *mapa)
{
    cout << "Combate entre caravana " << caravanaUser->getId() << " e caravana barbara " << caravanaBarbara->getId() << endl;

    if (caravanaUser->getTripulantes() == 0 || caravanaBarbara->getTripulantes() == 0)
    {
        cout << "Erro: Uma das caravanas não tem tripulantes para combater." << endl;
        return;
    }

    int randtripulantesUser = rand() % caravanaUser->getTripulantes();
    int randtripulantesBarbara = rand() % caravanaBarbara->getTripulantes();
    int tripulantesPerdidos = 0;

    if (randtripulantesUser >= randtripulantesBarbara)
    {
        // USER WON
        mapa->ganharCombate();
        tripulantesPerdidos = caravanaUser->getTripulantes() * 0.2;
        caravanaUser->setTripulantes(max(0, caravanaUser->getTripulantes() - tripulantesPerdidos));
        caravanaBarbara->setTripulantes(max(0, caravanaBarbara->getTripulantes() - tripulantesPerdidos * 2));
        cout << "Caravana " << caravanaUser->getId() << " ganhou o combate" << endl;
    }
    else
    {
        // BARBARA WON
        tripulantesPerdidos = caravanaBarbara->getTripulantes() * 0.2;
        caravanaBarbara->setTripulantes(max(0, caravanaBarbara->getTripulantes() - tripulantesPerdidos));
        caravanaUser->setTripulantes(max(0, caravanaUser->getTripulantes() - tripulantesPerdidos * 2));
        cout << "Caravana " << caravanaBarbara->getId() << " ganhou o combate" << endl;
    }

    if (caravanaUser->getTripulantes() <= 0)
    {
        mapa->removerCaravana(caravanaUser->getId());
        cout << "Caravana " << caravanaUser->getId() << " foi eliminada" << endl;
    }
    else if (caravanaBarbara->getTripulantes() <= 0)
    {
        caravanaUser->setAgua(caravanaUser->getAgua() + caravanaBarbara->getAgua());
        if (caravanaUser->getAgua() > caravanaUser->getMaxAgua())
        {
            caravanaUser->setAgua(caravanaUser->getMaxAgua());
        }
        mapa->removerBarbaro(caravanaBarbara->getId());
        cout << "Caravana " << caravanaBarbara->getId() << " foi eliminada" << endl;
    }

    cout << "Combate terminado" << endl;
}
