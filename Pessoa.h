#ifndef PESSOA_H_INCLUDED
#define PESSOA_H_INCLUDED

class Pessoa
{
public:
    std::string nome;
    std::string apelido;
    std::string nome_completo;
    std::string genero;

    std::string dia_nascimento;
    std::string mes_nascimento;
    std::string ano_nascimento;
    std::string data_de_nascimento;
    std::string idade;

    std::string morada;
    std::string nacionalidade;
    std::string estado_civil;
    std::string numero_identificacao_civil;

protected:
    Pessoa() {}
   virtual ~ Pessoa() {}



    /*
    void registarDadosPessoais (std::string nom, std::string apelid, std::string nome_complet,
            std::string gener, std::string data_de_nasciment, std::string enderec, std::string nacionalidad,
            std::string estado_civi, std::string numero_identificacao_civ)

    {
        nome = nom;
        apelido = apelid;
        nome_completo = nome_complet;
        gener = genero;
        data_de_nascimento = data_de_nasciment;
        morada = enderec;
        nacionalidade = nacionalidad;
        estado_civil = estado_civi;
        numero_identificacao_civil = numero_identificacao_civ;
    }


    void registarNome (std::string registo );

    void registarApelido (std::string registo );

    void registarDataDeNascimento (std::string registo );

    void registarEndereco (std::string registo );

    void registaNacionalidade (std::string registo );

    void registarEstadoCivil (std::string registo );

    void registarNumeroIdentificacaoCivl (std::string registo );
    */
};


#endif // PESSOA_H_INCLUDED
