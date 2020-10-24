#ifndef ESTUDANTE_H_INCLUDED
#define ESTUDANTE_H_INCLUDED

#include <sstream>
#include "Pessoa.h"

class Estudante : public Pessoa
{
public:
    Estudante () {}
    ~Estudante () {}

    int ano_escolaridade;
    std::string turma;
    std::string ano_e_turma;
    int numero_do_aluno;

    void registar (std::string estu_info);

    friend std::ostream& operator << (std::ostream& os, Estudante& est);
};


#endif // ESTUDANTE_H_INCLUDED
