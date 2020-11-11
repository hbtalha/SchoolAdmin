#ifndef PESSOA_H_INCLUDED
#define PESSOA_H_INCLUDED

#include <sys/time.h>

class Pessoa
{
public:
    std::string nome;
    std::string apelido;
    std::string nome_completo;
    char genero;

    int dia_nascimento;
    int mes_nascimento;
    int ano_nascimento;
    std::string data_de_nascimento;
    int idade;

    std::string morada;
    std::string nacionalidade;
    std::string estado_civil;
    std::string numero_identificacao_civil;

protected:
    Pessoa() {}
    virtual ~ Pessoa() {}

    int estabelecerIdade()
    {
        time_t now = time(nullptr);
        tm* agora = localtime(&now);

        int presente_ano = 1900 + agora->tm_year;
        int presente_mes = agora->tm_mon + 1;
        int presente_dia = agora->tm_mday;

        int idade = presente_ano - ano_nascimento;

        if(presente_mes < mes_nascimento)
            --idade;
        else if(presente_mes == mes_nascimento && presente_dia < dia_nascimento)
            --idade;

        return idade;
    }
};


#endif // PESSOA_H_INCLUDED
