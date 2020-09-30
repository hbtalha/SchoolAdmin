#ifndef ESTUDANTE_H_INCLUDED
#define ESTUDANTE_H_INCLUDED

#include <sstream>
#include "Pessoa.h"

class Estudante : public Pessoa
{
public:
    Estudante () {}
    ~Estudante () {}

    std::string ano_escolaridade;
    std::string turma;
    std::string numero_do_aluno;
    std::string numero_da_sala_de_aula;

    void registar (std::string estu_info);

    std::string obterAnoDeEscolaridade () const;
    std::string obterTurma () const;
    std::string obterNumeroDoAluno () const;
    std::string obterNumeroDaSalaDeAula () const;

    friend std::ostream& operator << (std::ostream& os, Estudante& est);

    /*
    bool operator < (const Estudante& est) const
    {
        return (this->nome_completo < est.nome_completo);
    }

    void registarAnoDeEscolaridade (int registo);
    void registarNumeroDaSalaDeAula (int registo);
    void registarNumeroDoAluno (int registo);
    void registarTurma (std::string registo);
    */
};


#endif // ESTUDANTE_H_INCLUDED
