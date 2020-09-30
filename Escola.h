#ifndef ESCOLA_H_INCLUDED
#define ESCOLA_H_INCLUDED

#include <string>

class Escola
{
protected:
    std::string nome_da_escola;
    std::string localizacao_da_escola;
    std::string data_inauguracao_da_escola;

    // Estudante stdnts;
public:
    Escola () {}
    ~Escola () {}

    void resgistar (std::string esc_info);

    std::string obterNomeDaEscola ();
    std::string obterLocalizacaoDaEscola ();
    std::string obterDataInauguracaoDaEscola ();

    friend std::ostream& operator << (std::ostream& os, Escola& esc);

};

#endif // ESCOLA_H_INCLUDED
