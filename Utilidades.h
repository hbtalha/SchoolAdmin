#ifndef UTILIDADES_H_INCLUDED
#define UTILIDADES_H_INCLUDED

#include "Manager.h"
#include <fstream>

namespace util
{

void pressionarEnter();

void set_a_FileAttribute_To_ReadOnly_or_Normal(std::string filepath, unsigned long flag);

bool seFicheiroVazio(Manager *m, void (Manager::*funcao_retorno)(), const std::string& file, std::string warn);

void criarPastasDaEscola(std::string nome_da_escola);
void criarPastasPrincipais();

void regularizarNomes(std::string& str);
void adicionarEspacos(std::string& str);

void dataDaInauguracaoDaEscola(Manager *m, std::string& s_ano, std::string& s_mes, std::string& s_dia);

std::string inputRestriction(int length_limit, int limite, bool chck, Manager *m, void (Manager::*funcao_de_retorno) (), int& _data);
std::string inputRestriction(int length_limit, int limite, bool chck, Manager *m, void (Manager::*funcao_de_retorno) ());
void validarInput(std::string& input, Manager *m, void (Manager::*funcao_de_retorno) ());
bool dataValido(const int& ano, const int& mes, const int& dia);

enum class TamanhoConsole {Normal, Aumentada};

void ajustarTamanhoJanela(TamanhoConsole tamanho);

}


#endif // UTILIDADES_H_INCLUDED
