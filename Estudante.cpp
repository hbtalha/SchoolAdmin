
#include "Estudante.h"

using std::string;
using std::stringstream;
using std::ostream;

void Estudante::registar (string estu_info)
{
    stringstream estud_info (estu_info);

    estud_info >> nome;
    estud_info >> apelido;
    nome_completo = nome + apelido;
    estud_info >> genero;

    estud_info >> dia_nascimento;
    estud_info >> mes_nascimento;
    estud_info >>  ano_nascimento;

    // condições if para fazer um display melhor das datas de nascimentos
    string temp_dia = dia_nascimento;
    string temp_mes = mes_nascimento;

    if (dia_nascimento.size() == 1)
        temp_dia = "0" + dia_nascimento;

    if (mes_nascimento.size() == 1)
        temp_mes =  "0" + mes_nascimento;

    data_de_nascimento = temp_dia + " - " + temp_mes + " - " + ano_nascimento;
    estud_info >> idade;

    estud_info >> morada;
    estud_info >> nacionalidade;
    estud_info >> estado_civil;
    estud_info >> numero_identificacao_civil;

    estud_info >> ano_escolaridade;
    estud_info >> turma;
    estud_info >> numero_do_aluno;
}

string Estudante::obterAnoDeEscolaridade () const
{
    return ano_escolaridade;
}

string Estudante::obterNumeroDaSalaDeAula () const
{
    return numero_da_sala_de_aula;
}

string Estudante::obterNumeroDoAluno () const
{
    return numero_do_aluno;
}

string Estudante::obterTurma () const
{
    return turma;
}

ostream& operator << (ostream& os, Estudante& est)
{
    os << est.nome  << " " << est.apelido  << " " << est.genero  << " " << est.dia_nascimento << " "
       << est.mes_nascimento << " " << est.ano_nascimento << " " << est.idade << " " << " "
       << est.morada << " " << est.nacionalidade  << " " << est.estado_civil  << " " << est.numero_identificacao_civil   << " "
       <<  est.ano_escolaridade << " " << " " << est.turma << " " << est.numero_do_aluno;

    return os;
}














/*
void Estudante::registarAnoDeEscolaridade (int registo)
{
    ano_escolaridade = registo;
}

void Estudante::registarNumeroDaSalaDeAula (int registo)
{
    numero_da_sala_de_aula = registo;
}

void Estudante::registarNumeroDoAluno (int registo)
{
    numero_da_sala_de_aula = registo;
}

void Estudante::registarTurma (string registo)
{
    turma = registo;
}


*/
