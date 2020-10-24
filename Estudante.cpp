
#include "Estudante.h"

using std::string;
using std::to_string;
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
    estud_info >> ano_nascimento;

    // condições if para fazer um display melhor das datas de nascimentos
    string temp_dia = std::to_string(dia_nascimento);
    string temp_mes = std::to_string(mes_nascimento);

    if (dia_nascimento < 10)
        temp_dia = "0" + to_string(dia_nascimento);

    if (mes_nascimento < 10)
        temp_mes =  "0" + to_string(mes_nascimento);

    data_de_nascimento = temp_dia + " - " + temp_mes + " - " + std::to_string(ano_nascimento);

    idade = estabelecerIdade();

    estud_info >> morada;
    estud_info >> nacionalidade;
    estud_info >> estado_civil;
    estud_info >> numero_identificacao_civil;

    estud_info >> ano_escolaridade;
    estud_info >> turma;

    ano_e_turma = to_string(ano_escolaridade) + turma;

    estud_info >> numero_do_aluno;
}

ostream& operator << (ostream& os, Estudante& est)
{
    os << est.nome << " "
       << est.apelido << " "
       << est.genero << " "
       << est.dia_nascimento << " "
       << est.mes_nascimento << " "
       << est.ano_nascimento << " "
       << est.morada << " "
       << est.nacionalidade << " "
       << est.estado_civil << " "
       << est.numero_identificacao_civil << " "
       <<  est.ano_escolaridade << " "
       << est.turma << " "
       << est.numero_do_aluno;

    return os;
}
