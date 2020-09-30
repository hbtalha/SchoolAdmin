#include <string>
#include <vector>
#include <sstream>

#include "Escola.h"

using std::string;
using std::stringstream;
using std::ostream;

void Escola::resgistar(string esc_info )
{
    stringstream info (esc_info);

    info >> nome_da_escola >> localizacao_da_escola >> data_inauguracao_da_escola;
}

string Escola::obterNomeDaEscola ()
{
    return nome_da_escola;
}

string Escola::obterLocalizacaoDaEscola ()
{
    return localizacao_da_escola;
}

string Escola::obterDataInauguracaoDaEscola ()
{
    return data_inauguracao_da_escola;
}

ostream& operator << (std::ostream& os, Escola& esc)
{
    os << esc.obterNomeDaEscola() << " " <<  esc.obterLocalizacaoDaEscola() << " " << esc.obterDataInauguracaoDaEscola();

    return os;
}
