#include <iostream>
#include <vector>

#include "Pessoa.h"

using namespace std;

/*
#include <string>

#include "Pessoa.h"

using namespace std;

void Pessoa::registarNome (string registo )
{
    nome = registo;
}

void Pessoa::registarApelido (string registo )
{
    apelido = registo;
}

void Pessoa::registarDataDeNascimento (string registo )
{
    data_de_nascimento = registo;
}

void Pessoa::registarEndereco (string registo )
{
    endereco = registo;
}

void Pessoa::registaNacionalidade (string registo )
{
    nacionalidade = registo;
}

void Pessoa::registarEstadoCivil (string registo )
{
    estado_civil = registo;
}

void Pessoa::registarNumeroIdentificacaoCivl (string registo )
{
    numero_identificacao_civil = registo;
}

*/


string Pessoa::obterNome ()
{
    return nome;
}

string Pessoa::obterApelido ()
{
    return apelido;
}

string Pessoa::obterNomeCompleto ()
{
    return nome + apelido;
}

string Pessoa::obterDataDeNascimento ()
{
    return data_de_nascimento;
}

string Pessoa::obterMorada()
{
    return morada;
}

string Pessoa::obterNacionalida ()
{
    return nacionalidade;
}

string Pessoa::obterEstadoCivil ()
{
    return estado_civil;
}

string Pessoa::obterNumeroDeIdentificacaoCivil ()
{
    return numero_identificacao_civil;
}

