#ifndef MANAGER_H_INCLUDED
#define MANAGER_H_INCLUDED

#include <vector>
#include <deque>

#include "Escola.h"
#include "Estudante.h"


class Escola;

class Manager
{
public:

    static Manager& getOnlyInstance()
    {
        static Manager es;

        return es;
    }

    void startMenu();

    Manager(Manager&) {};
    void operator=(Manager&) {};

    ~Manager() {};

private:

    Manager() {};

    Escola escola;

    void cabecalho();

    void entrarEscolas();
    void registarEscola();

    void paginaInicial();
    void sair();

    void wrteDataEscola(std::string file_path);
    void rdDataEscola(std::string file_path);

    void paginaInicialTurmas();
    void paginaInicialAlunos();

    void registarTurmas();
    void criarTurmas(int registo_de_ano_escolaridade, int num_turmas_registar, int opcao_de_area);

    void apagarTurmas();
    void apagarUmaTurma();
    void apagarTurmasDeUmaClasse();
    void apagarTurmaSelecionada(std::string path);
    void apagarTurmasDeClasseSelecionada(std::string path, std::string ano_de_escolaridade);
    void reajustarAsTurmasAposEliminacao(std::string ano_de_escolaridade);

    void verTurmas();
    void verTurmasPorClasse();
    void verTurmasDaClasse(int ano_escolaridade_, std::string file_path);
    void verTodasAsTurmas();
    void verNumeroDeTurmas();
    void verNumeroDeTurmasDeUmaClasse();
    void verNumeroDeTurmasDeTodaAEscola();

    std::vector<std::string> vecTurmas = { "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V","W", "X", "Y", "Z" };

    std::vector<std::string> vecTurmas3Ciclo = { "CT1", "CT2", "CT3", "CT4", "CT5", "CT6", "CT7", "CT8", "CT9", "ES1", "ES2",
                                                 "ES3", "ES4", "ES5", "ES6", "ES7", "ES8", "ES9", "H1", "H2", "H3", "H4", "H5", "H6", "H7", "H8", "H9"
                                               };

    std::vector<std::string> area_3_ciclo_path = {"/CT/", "/ES/", "/H/"};

    void registarAlunos();
    bool turmaExiste(std::string path, std::string turma);
    void verAlunos();
    void verAlunosPorClasse();
    void verAlunosPorTurma();
    void verTodosOsAlunosDaEscola();

    void selecionarGeneroParaVer();
    void verNumeroDeAlunos(char opcao_de_genero);
    void verNumeroDeAlunosNumaClasse(char opcao_de_genero);
    void verNumeroDeAlunosEntreClasses(char opcao_de_genero);
    void verNumeroDeAlunosEmTodaAEscola(char opcao_de_genero);
    int contarNumeroDeAlunosPeloGenero(std::deque<Estudante>& alunos, char opcao_de_genero);

    void inserirTurma(std::string& turma_path, std::string& ano_escolaridade, std::string& turma, auto (Manager::*funcao_de_retorno) ());
    void inserirAnoDeEscolaridadeEntreClasses(int& registo_de_ano_escolaridadeA, int& registo_de_ano_escolaridadeB, auto (Manager::*funcao_de_retorno) ());

    void pesquisarAlunos();

    void pesquisarMoradas();
    void pesquisarMoradasNumaClasse();
    void pesquisarMoradasEntreClasses();
    void pesquisarMoradasEmTodaAEsola();

    void pesquisarPorIdade();
    void pesquisarIdadeNumaClasse();
    void pesquisarIdadeEntreClasses();
    void pesquisarIdadeEmTodaAEsola();

    void pesquisarNomes();
    void pesquisarNomesNumaClasse();
    void pesquisarNomesEntreClasses();
    void pesquisarNomesEmTodaAEsola();

    void pesquisarDatas();
    void pesquisarDataNascimento();
    void pesquisarDataNascimentoNumaClasse();
    void pesquisarDataNascimentoEntreClasses();
    void pesquisarDataNascimentoEmTodaAEscola();
    void pesquisarAnoNascimento();
    void pesquisarAnoNascimentoNumaClasse();
    void pesquisarAnoNascimentoEntreClasses();
    void pesquisarAnoNascimentoEmTodaAEscola();
    void pesquisarMesNascimento();
    void pesquisarMesNascimentoNumaClasse();
    void pesquisarMesNascimentoEntreClasses();
    void pesquisarMesNascimentoEmTodaAEscola();
    void pesquisar_Mes_Ano_Nascimento();
    void pesquisar_Mes_Ano_NascimentoNumaClasse();
    void pesquisar_Mes_Ano_NascimentoEntreClasses();
    void pesquisar_Mes_Ano_NascimentoEmTodaAEscola();

    void pesquisarN_BI();
    void pesquisarN_BI_NumaClasse();
    void pesquisarN_BI_EntreClasses();
    void pesquisarN_BI_EmTodaAEsola();

    void editarAlunos();
    void editarInformacoesAluno();
    void editarInformacoesDoAluno(bool editar_tudo, int opcao, std::deque<Estudante>& aluno_a_edtitar);
    void apagarRegistoAluno();
    void transferirAlunoDeTurma();

    bool procurarAluno(std::deque<Estudante>& deque_alunos, int& indice_aluno, const std::string& numero_do_aluno_);
    void atualizarTurmaAposEdicao(std::string file_path, std::deque<Estudante>& deque_alunos);
    void transferirUmAlunoDeTurma(std::deque<Estudante>& deque_alunos);

    void encontrarAlunosPeloNome(std::deque<Estudante>& alunos, std::deque<Estudante>& alunos_encontrados, std::string nome_de_pesquisa);
    void encontrarAlunosPelaIdade(std::deque<Estudante>& alunos, std::deque<Estudante>& alunos_encontrados, std::string idade);
    void encontrarAlunosPelaMorada(std::deque<Estudante>& alunos, std::deque<Estudante>& alunos_encontrados, std::string morada);
    void encontrarAlunosPelaDataNascimento(std::deque<Estudante>& alunos, std::deque<Estudante>& alunos_encontrados, std::string data_nascimento);
    void encontrarAlunosPeloAnoNascimento(std::deque<Estudante>& alunos, std::deque<Estudante>& alunos_encontrados, std::string ano_nascimento_);
    void encontrarAlunosPeloMesNascimento(std::deque<Estudante>& alunos, std::deque<Estudante>& alunos_encontrados, std::string mes_nascimento_);
    void encontrarAlunosPelo_Mes_Ano_Nascimento(std::deque<Estudante>& alunos, std::deque<Estudante>& alunos_encontrados, std::string mes_ano);
    void encontrarAlunosPeloNumeroBI(std::deque<Estudante>& alunos, std::deque<Estudante>& alunos_encontrados, std::string n_bi);

    void atribuirNumeroAosAlunos_e_OrganizarPorOdemAlfabetica(std::string file_path, Estudante estud);
    void estabelecerIdadeDosAlunos(std::string ano, std::string mes, std::string dia, std::string& idade);
    void atualizarIdadeDosAlunosdeTodaAEscola();

    void ordenar(std::deque<Estudante>& alunos);

    static bool ordenarAlunosPorNomeCrescente(const Estudante& es1, const Estudante& es2);
    static bool ordenarAlunosPorNomeDecrescente(const Estudante& es1, const Estudante& es2);
    static bool ordenarAlunosPorIdadeCrescente(const Estudante& es1, const Estudante& es2);
    static bool ordenarAlunosPorIdadeDecrescente(const Estudante& es1, const Estudante& es2);
    static bool ordenarAlunosPorMorada(const Estudante& es1, const Estudante& es2);

    int contarNumeroDeTurmasDeUmaClasse(std::string ano_escolaridade);
    int contarNumeroDeTurmas(auto (Manager::*funcao_retorno)(), std::string file_path);

    void extrairAlunosDeUmaTurma(std::deque<Estudante>& dequeEstdnt, std::string file_path);
    void extrairAlunosDeUmaClasse(std::deque <Estudante>& deque_alunos_por_classe, std::string path, std::string ano_escolaridade);
    void extrairAlunosDeTodaAEscola(std::deque <Estudante>& alunos);

    void displayAlunos(std::deque <Estudante> deque_alunos_por_turma, bool opcao_vista);

};


#endif // MANAGER_H_INCLUDED
