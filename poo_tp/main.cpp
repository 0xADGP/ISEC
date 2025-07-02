#include "Simulacao.h"
#include "Interface.h"

int main()
{
    Simulacao simulacao;
    Interface interface(&simulacao);
    interface.iniciar();
    return 0;
}
