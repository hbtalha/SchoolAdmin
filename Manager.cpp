#include <iostream>
#include <ostream>
#include <iomanip>
#include <conio.h>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <windows.h>
#include <filesystem>

#include "Manager.h"
#include "Diretorios.h"
#include "Utilidades.h"

using std::cout;
using std::endl;
using std::cin;
using std::cerr;
using std::string;
using std::to_string;
using std::stoi;
using std::stringstream;
using std::vector;
using std::deque;
using std::ifstream;
using std::ofstream;
using std::setw;
using std::left;
using std::right;
using std::swap;
using std::filesystem::exists;
using std::filesystem::create_directories;

void Manager::wrteDataEscola(string file_path)
{
    ofstream file(file_path);

    if (file.is_open())
    {
        file << escola;

        file.close();

        util::set_a_FileAttribute_To_ReadOnly_or_Normal(file_path, FILE_ATTRIBUTE_READONLY);
    }
    else
    {
        cerr  << endl << endl << "\a" << "*****Erro ao escrever para o ficheiro com as informacoes da escola*****" << endl << endl;

        Sleep(2000);

        startMenu();
    }
}

void Manager::rdDataEscola(string file_path)
{
    ifstream file(file_path);

    if (file.is_open())
    {
        string info_line;

        getline(file, info_line);

        escola.resgistar(info_line);

        file.close();
    }
    else
    {
        cerr << endl << endl << "\a" << "*****Erro no acesso ao ficherio que contem informacoes da escola*****" << endl << endl;

        Sleep(2000);

        startMenu();
    }
}

void Manager::startMenu()
{
    system("cls");

    util::criarPastasPrincipais();

    cout << "\n" << "   \t\t" << "--------------------------------------------------" << endl;
    cout << "   \t\t\t" << "SISTEMA DE GESTAO DA SUA ESCOLA"  << endl;
    cout << "   \t\t" << "--------------------------------------------------" << endl << endl;
    cout << "   \t\t" << "1. Entrar Na Sua Escola" << endl << endl;
    cout << "   \t\t" << "2. Registar A Sua Escola" << endl << endl;
    cout << "   \t\t" << "-> ";
    char opt = _getch();

    while (opt != '1' && opt != '2' && opt != '0' && opt != 27)
    {
        opt = _getch();
    }

    switch (opt)
    {
    case '1':
        entrarEscolas();
        break;

    case '2':
        registarEscola();
        break;

    case '0':
    case 27:
        cout << "\n\n\nProcess returned 0" << endl << "Press enter key to continue.";
        util::pressionarEnter();
        exit(0);
        break;

    default:
        startMenu();
        break;
    }
}

void Manager::cabecalho()
{
    system("cls");

    string nome = escola.obterNomeDaEscola();

    util::adicionarEspacos(nome);

    cout << "\n" << "   \t\t" << "--------------------------------------------------" << endl;
    cout << "   \t\t\t\t" << nome << endl;
    cout << "   \t\t" << "--------------------------------------------------" << endl << endl;
}

void Manager::registarEscola()
{
    system("cls");

    string nome_da_escola, localizacao_da_escola, data_inauguracao_da_escola;

    cout << "\n" << "   \t\t" << "--------------------------------------------------" << endl;
    cout << "   \t\t\t" << "SISTEMA DE GESTAO DA SUA ESCOLA"  << endl;
    cout << "   \t\t" << "--------------------------------------------------" << endl << endl;

    cout << "   \t\t\t\t" << "REGISTAR ESCOLA" << endl << endl;

    cout << "   \t\t" << "-> Nome da Escola: ";
    util::validarInput(nome_da_escola, this, &Manager::startMenu);

    cout << endl << "   \t\t" << "-> Localizacao da Escola: ";
    util::validarInput(localizacao_da_escola, this, &Manager::startMenu);

    string dia, mes, ano;

    cout << endl << "   \t\t" << "-> Data de Inauguracao da Escola: " << endl;
    util::dataDaInauguracaoDaEscola(this, ano, mes, dia);

    data_inauguracao_da_escola = dia + "-" + mes + "-" + ano;

    string info_escola = nome_da_escola + " " + localizacao_da_escola + " " + data_inauguracao_da_escola;

    escola.resgistar(info_escola);

    string file_path = Diretorios::ESCOLAS_INFO + nome_da_escola;

    util::criarPastasDaEscola(nome_da_escola);

    wrteDataEscola(file_path );

    system("cls");

    cout << endl << "   \t\t\t\t" << "Registo Concluido" << endl << endl;

    Sleep(1000);

    paginaInicial();
}

void Manager::entrarEscolas()
{
    system("cls");

    cout << "\n" << "   \t\t" << "--------------------------------------------------" << endl;
    cout << "   \t\t\t" << "SISTEMA DE GESTAO DA SUA ESCOLA"  << endl;
    cout << "   \t\t" << "--------------------------------------------------" << endl << endl;

    cout << "   \t\t\t\t" << "ENTRAR ESCOLA" << endl << endl;

    string nome_da_escola;

    cout << "   \t\t" << "Nome da Escola: ";
    getline(cin, nome_da_escola);

    if (nome_da_escola == "0")
        startMenu();

    // irá remover espacos e capitalizar as letras necessarias
    util::regularizarNomes(nome_da_escola);

    string file_path = Diretorios::ESCOLAS_INFO + nome_da_escola;

    ifstream file(file_path);

    // caso o ficheiro nao existir isto significa que a escola nao exite e o usuario
    // eh pronteado com o aviso de tal e a opcao de entrar de novo
    if (! file.good())
        while ( 1 )
        {
            cout << endl << endl << "   \t\t"  << "Escola nao encontrada!!" << endl;
            cout << "   \t\t" << " Entre de novo: ";
            getline(cin, nome_da_escola);

            if (nome_da_escola == "0")
                startMenu();

            // irá remover espacos e capitalizar as letras necessarias
            util::regularizarNomes(nome_da_escola);

            file_path = Diretorios::ESCOLAS_INFO + nome_da_escola;

            // tenta abrir o ficheiro com o novo nome da escola
            file.open(file_path);

            // se o ficehiro for aberto com sucesso o loop sera quebrado dando o ficheiro como
            // existente e o msm para a escola
            if (file.good())
                break;
        }

    file.close();

    // obtem as informacoes da escola que se encontram no ficheiro
    rdDataEscola(file_path);

    atualizarIdadeDosAlunosdeTodaAEscola();

    paginaInicial();
}

void Manager::paginaInicial()
{
    cabecalho();

    cout << "   \t\t" << "\t\t" << "Pagina Inicial" << endl << endl;

    cout << "   \t\t" << "1. Turmas" << endl;

    cout << "   \t\t" << "\t\t" << "0. Sair" << endl;

    cout << "   \t\t" << "2. Alunos" << endl << endl;

    cout << "   \t\t" << "->";
    char opt = _getch();

    while (opt != '1' && opt != '2' && opt != '0' && opt != 27)
    {
        opt = _getch();
    }

    switch(opt)
    {
    case '1':
        paginaInicialTurmas();
        break;

    case '2':
        paginaInicialAlunos();
        break;

    case 27:
    case '0':
        sair();
        break;


    default:
        paginaInicial();
    }
}

int Manager::contarNumeroDeTurmas(auto (Manager::*funcao_retorno)(), std::string file_path)
{
    int numero_turmas = 0;

    if(std::filesystem::exists(file_path))
    {
        for(const auto& it : std::filesystem::directory_iterator(file_path))
            if(std::filesystem::is_regular_file(it.path()))
                if(find(vecTurmas.begin(), vecTurmas.end(),  it.path().filename().string()) != vecTurmas.end() or
                        find(vecTurmas3Ciclo.begin(), vecTurmas3Ciclo.end(),  it.path().filename().string()) != vecTurmas3Ciclo.end() )
                    ++numero_turmas;
    }
    else
    {
        cerr << endl << endl << endl << "\a" << "*****Erro Ao Contar Numero De Ficheiros*****" << endl;

        Sleep(2000);

        (this->*funcao_retorno)();
    }

    return numero_turmas;
}

void Manager::paginaInicialTurmas()
{
    cabecalho();

    cout << "   \t\t" << "\t\t" << "Turmas" << endl << endl;

    cout << "   \t\t" << "1. Registar Turmas" << endl << endl;

    cout << "   \t\t" << "2. Apagar Turmas" << endl << endl;

    cout << "   \t\t" << "3. Ver Turmas" << endl << endl;

    cout << "   \t\t" << "4. Ver Numero De Turmas" << endl << endl;

    cout << "   \t\t" << "0. Voltar" << endl << endl;

    char opt;

    cout << "   \t\t" << "->";
    opt = _getch();

    // operacao que faz com que o programa apenas um conjunto de entradas validas
    // esta opercao de de restricao de entradas feitas pelo usuario sera encontrada ao longo
    // do programa
    while (opt != '1' && opt != '2' && opt != '3' && opt != '4' && opt != '0' && opt != 27 && opt != 8)
    {
        opt = _getch();
    }

    switch (opt)
    {
    case '1':
        registarTurmas();
        break;

    case '2':
        apagarTurmas();
        break;

    case '3':
        verTurmas();
        break;

    case '4':
        verNumeroDeTurmas();
        break;

    case '0':
    case 27:
    case 8:
        paginaInicial();
        break;

    default:
        paginaInicialTurmas();
        break;
    }
}

void Manager::registarTurmas()
{
    cabecalho();

    int registo_de_ano_escolaridade;
    int num_turmas_registar = 0;
    int opcao_de_area = 0;

    cout << "   \t\t" << "\t\t" << "Registar Turmas" << endl << endl;

    cout << "   \t\t" << " Registar Ano De Escolaridade (1 -12): ";
    util::inputRestriction(2, 12, true, this, &Manager::paginaInicialTurmas, registo_de_ano_escolaridade); // a funcao nao foi criada para esta situacao mas ela serve perfeitamente

    string str_registo_de_ano_escolaridade = to_string(registo_de_ano_escolaridade);

    string path = Diretorios::ESCOLAS + escola.obterNomeDaEscola() + "/Classes/" + str_registo_de_ano_escolaridade + "/";

    if (registo_de_ano_escolaridade < 11)
    {
        // contar numero de ficheiros ou seja conta o numero de turmas já registados
        int num_turmas_registados = contarNumeroDeTurmas(&paginaInicial, path);

        int num_turmas_restantante = 26 - num_turmas_registados;

        if (num_turmas_restantante <= 0)
        {
            cout << endl << endl << "   \t\t" << " *****Turma Selecionada Ja Contem O Numero Maxinmo De Turmas*****";

            Sleep(1000);

            registarTurmas();
        }

        cout << endl << endl << "   \t\t" << " Numero De Turmas A Registar (" << num_turmas_registados
             << " Registados | " << num_turmas_restantante << " Restantes): ";
        util::inputRestriction(2, num_turmas_restantante, true, this, &Manager::paginaInicialTurmas, num_turmas_registar);
    }
    else
    {
        cout << endl << endl << "   \t\t" << "1-> Ciencias e Tecnologias (CT)" ;
        cout << setw(28) << "2-> Economico-Social (ES)";
        cout  << setw(23) << "3-> Humasnitica (H)";
        cout << endl << endl << "   \t\t" << "Escolha a opcao: ";
        util::inputRestriction(1, 3, true, this, &Manager::paginaInicialTurmas, opcao_de_area);

        string srt_opcao_de_area = to_string(opcao_de_area);

        switch(opcao_de_area)
        {
        case 1:
            path = Diretorios::ESCOLAS + escola.obterNomeDaEscola() + "/Classes/" + str_registo_de_ano_escolaridade + "/CT/";
            break;

        case 2:
            path = Diretorios::ESCOLAS + escola.obterNomeDaEscola() + "/Classes/" + str_registo_de_ano_escolaridade + "/ES/";
            break;

        case 3:
            path = Diretorios::ESCOLAS + escola.obterNomeDaEscola() + "/Classes/" + str_registo_de_ano_escolaridade + "/H/";
            break;
        }

        int num_turmas_registados = contarNumeroDeTurmas(&paginaInicial, path);

        int num_turmas_restantante = 9 - num_turmas_registados;

        if (num_turmas_restantante <= 0)
        {
            cout << endl << endl <<  "\a" <<  "   \t\t" << "  ***** Turma Selecionada Ja Contem O Numero Maxinmo De Turmas*****";

            Sleep(2000);

            registarTurmas();
        }

        cout << endl << endl << "   \t\t" << " Numero De Turmas A Registar (" << num_turmas_registados
             << " Registados | " << num_turmas_restantante << " Restantes): ";
        util::inputRestriction(2, num_turmas_restantante, true, this, &Manager::paginaInicialTurmas, num_turmas_registar);
    }

    criarTurmas(registo_de_ano_escolaridade, num_turmas_registar, opcao_de_area);

    cout << endl << endl << "\a" << "Turma(s) Criadas Com Sucesso" << endl;

    Sleep(2000);

    paginaInicialTurmas();
}

// depois dos numeros de turmas ja forem introduzidas esta funcao cria as turmas
void Manager::criarTurmas(int registo_de_ano_escolaridade, int num_turmas_registar, int opcao_de_area)
{
    if (registo_de_ano_escolaridade < 11)
    {
        string str_registo_de_ano_escolaridade = to_string(registo_de_ano_escolaridade);

        string file_path = Diretorios::ESCOLAS + escola.obterNomeDaEscola() + "/Classes/" + str_registo_de_ano_escolaridade + "/" ;

        // ao saber quantas turmas ja foram registadas o offset no loop é atribuida a esse numero
        int num_turmas_registados = contarNumeroDeTurmas(&paginaInicial, file_path);

        // e o loop começa no 'num_turmas_registados' indo até o 'linite'
        int limite = num_turmas_registados + num_turmas_registar;

        for (int i = num_turmas_registados; i < limite; ++i)
        {
            file_path = Diretorios::ESCOLAS + escola.obterNomeDaEscola() + "/Classes/" + str_registo_de_ano_escolaridade + "/" + vecTurmas[i];

            ofstream file (file_path);

            util::set_a_FileAttribute_To_ReadOnly_or_Normal(file_path, FILE_ATTRIBUTE_READONLY);

            file.close();
        }
    }
    else
    {
        string str_registo_de_ano_escolaridade = to_string(registo_de_ano_escolaridade);

        string file_path_ct = Diretorios::ESCOLAS + escola.obterNomeDaEscola() + "/Classes/" + str_registo_de_ano_escolaridade + "/CT/" ;
        string file_path_es = Diretorios::ESCOLAS + escola.obterNomeDaEscola() + "/Classes/" + str_registo_de_ano_escolaridade + "/ES/" ;
        string file_path_h = Diretorios::ESCOLAS + escola.obterNomeDaEscola() + "/Classes/" + str_registo_de_ano_escolaridade + "/H/" ;

        int num_turmas_registados;

        if (opcao_de_area == 1)
            num_turmas_registados = contarNumeroDeTurmas(&paginaInicial, file_path_ct);

        if (opcao_de_area == 2)
            num_turmas_registados = contarNumeroDeTurmas(&paginaInicial, file_path_es);

        if (opcao_de_area == 3)
            num_turmas_registados = contarNumeroDeTurmas(&paginaInicial, file_path_h);

        int indice = num_turmas_registados + num_turmas_registar;

        for (int i = num_turmas_registados + 1; i <= indice; ++i)
        {
            if (opcao_de_area == 1)
            {
                string numero_de_turma = to_string(i);
                string file_path = file_path_ct + "CT" + numero_de_turma;

                ofstream file(file_path);

                // depois de cada turma ser criada o ficheiro referente a turma é atribuida o atributo de read-only
                // este serve para que o ficheiro nao seja editado pelo usuario
                util::set_a_FileAttribute_To_ReadOnly_or_Normal(file_path, FILE_ATTRIBUTE_READONLY);

                file.close();
            }

            if (opcao_de_area == 2)
            {
                string numero_de_turma = to_string(i);
                string file_path = file_path_es + "ES" + numero_de_turma;

                ofstream file(file_path);

                util::set_a_FileAttribute_To_ReadOnly_or_Normal(file_path, FILE_ATTRIBUTE_READONLY);

                file.close();
            }

            if (opcao_de_area == 3)
            {
                string numero_de_turma = to_string(i);
                string file_path = file_path_h + "H" + numero_de_turma ;

                ofstream file(file_path);

                util::set_a_FileAttribute_To_ReadOnly_or_Normal(file_path, FILE_ATTRIBUTE_READONLY);

                file.close();
            }
        }
    }
}

void Manager::apagarTurmas()
{
    cabecalho();

    cout << "   \t\t" << "\t\t" << "Apagar Turmas" << endl << endl;

    cout << "   \t\t" << "   1. Apagar Uma Turma" << endl << endl;
    cout << "   \t\t" << "   2. Apagar Todas As Turmas De Uma Classe" << endl << endl;
    cout << "   \t\t" << "   0. Cancelar" << endl << endl;
    cout << "   \t\t" << "   ->";
    char opt = _getch();

    while (opt != '1' && opt != '2' && opt != '0' && opt != 27 && opt != 8)
    {
        opt = _getch();
    }

    switch(opt)
    {
    case '1':
        apagarUmaTurma();
        break;

    case '2':
        apagarTurmasDeUmaClasse();
        break;

    case '0':
    case 8:
        paginaInicialTurmas();
        break;

    case 27:
        paginaInicial();
        break;

    default:
        verTurmas();
        break;
    }
}

// funcao para o usuario escolher qual turma que deseja apagar
void Manager::apagarUmaTurma()
{
    cabecalho();

    cout << "   \t\t" << "\t\t" << "Apagar Turma" << endl << endl;

    string file_path, ano_escolaridade, turma;

    inserirTurma(file_path, ano_escolaridade, turma, &Manager::apagarTurmas);

    cabecalho();

    cout << "   \t\t" << "\t\t" << "Apagar Turma" << endl << endl;

    cout << "   \t\t" << "   Turma " << ano_escolaridade << turma << endl << endl;

    deque<Estudante> deque_alunos;

    // se o ficheiro nao estiver vazio ou seja se a turma conter alunos, esses alunos serao extraidos para o deque
    // este tem o proposito de informar ao usuario o numero de alunos da turma selecionada
    if (! util::seFicheiroVazio(this, &paginaInicial, file_path, "apagarUmaTurma"))
        extrairAlunosDeUmaTurma(deque_alunos, file_path);

    cout << "   \t\t" << "   Turma Selecionada Contem " << deque_alunos.size() << " Alunos" << endl << endl;

    cout << "   \t\t" << "    1-> Entrar de novo" << endl;
    cout << "   \t\t" << "    2-> Apagar" << endl;
    cout << "   \t\t" << "    0-> Cancelar";
    cout << endl << endl << "   \t\t" << "   Escolha a opcao: ";
    char opt = _getch();

    while (opt != '1' && opt != '2' && opt != '0')
    {
        opt = _getch();
    }

    if (opt == '1')
    {
        apagarTurmas();
    }
    else if (opt == '2')
    {
        apagarTurmaSelecionada(file_path);

        reajustarAsTurmasAposEliminacao(ano_escolaridade);

        system("cls");

        cout << endl << "   \t\t\t\t" << "Turma Apagada Com Sucesso" << endl << endl;

        Sleep(1200);

        apagarTurmas();
    }
    else if (opt == '0')
    {
        paginaInicialTurmas();
    }
}

// funcao para o usuario escolher turmas de que classe deseja apagar
void Manager::apagarTurmasDeUmaClasse()
{
    cabecalho();

    cout << "   \t\t" << "\t\t" << "Apagar Turmas De Uma Classe" << endl << endl;

    int registo_de_ano_escolaridade;

    cout << "   \t\t" << "  Ano De Escolaridade (1 -12): ";
    string ano_escolaridade = util::inputRestriction(2, 12, true, this, &Manager::apagarTurmas, registo_de_ano_escolaridade);

    string file_path = Diretorios::ESCOLAS + escola.obterNomeDaEscola() + "/Classes/" + ano_escolaridade + "/";

    cout << endl << endl <<"   \t\t" << "   Apagar Turmas do " << ano_escolaridade << " Ano" << endl << endl;

    deque<Estudante> deque_alunos;

    extrairAlunosDeUmaClasse(deque_alunos, file_path, ano_escolaridade);

    cout << "   \t\t" << "   Classe Selecionada Contem " << contarNumeroDeTurmasDeUmaClasse(ano_escolaridade) << " Turmas";
    cout << " Com Um Todo De " << deque_alunos.size() << " Alunos" << endl << endl;

    cout << "   \t\t" << "    1-> Entrar de novo" << endl;
    cout << "   \t\t" << "    2-> Apagar" << endl;
    cout << "   \t\t" << "    0-> Cancelar";
    cout << endl << endl << "   \t\t" << "    Escolha a opcao: ";
    char opt = _getch();

    while (opt != '1' && opt != '2' && opt != '0')
    {
        opt = _getch();
    }

    if (opt == '1')
    {
        apagarTurmasDeUmaClasse();
    }
    else if (opt == '2')
    {
        apagarTurmasDeClasseSelecionada(file_path, ano_escolaridade);

        system("cls");

        cout << endl << "   \t\t\t\t" << "Turmas Apagada Com Sucesso" << endl << endl;

        Sleep(1200);

        apagarTurmas();
    }
    else if (opt == '0')
    {
        paginaInicialTurmas();
    }
}

// funcao que apaga o ficheiro referente a turma selecionada
void Manager::apagarTurmaSelecionada(string file_path)
{
    const char* file_to_delete = file_path.c_str();

    // o atributo read-only do ficheiro tem que ser removido para que possa ser apagado
    util::set_a_FileAttribute_To_ReadOnly_or_Normal(file_path, FILE_ATTRIBUTE_NORMAL);

    remove(file_to_delete);
}

// funcao que apaga um conjunto de ficheiros (caso existirem) referentes as turmas da classe selecionada
void Manager::apagarTurmasDeClasseSelecionada(string file_path, string ano_de_escolaridade)
{
    bool ocorreu_erro = false;
    try
    {
        if(stoi(ano_de_escolaridade.c_str()) < 11)
        {
            if(std:: filesystem::exists(file_path))
            {
                for(const auto& it : std::filesystem::directory_iterator(file_path))
                    if(std::filesystem::is_regular_file(it.path()))
                    {
                        util::set_a_FileAttribute_To_ReadOnly_or_Normal(it.path().string().c_str(), FILE_ATTRIBUTE_NORMAL);
                        std::filesystem::remove_all(it.path());
                    }
            }
            else
                ocorreu_erro = true;
        }
        else
        {
            for( int j = 0, len = area_3_ciclo_path.size(); j < len; ++j)
                if(std::filesystem::exists(area_3_ciclo_path[ j ]))
                {
                    for(const auto& it : std::filesystem::directory_iterator(file_path + area_3_ciclo_path[ j ]))
                        if(std::filesystem::is_regular_file(it.path()))
                        {
                            util::set_a_FileAttribute_To_ReadOnly_or_Normal(it.path().string().c_str(), FILE_ATTRIBUTE_NORMAL);
                            std::filesystem::remove_all(it.path());
                        }
                }
                else
                    ocorreu_erro = true;
        }
    }
    catch(std::exception& e)
    {
        cerr << endl << e.what() << endl;
        cin.get();
    }

    if(ocorreu_erro)
    {
        cerr << endl << endl << "\a" << "*****Erro Ao Apagar Turmas De Uma Classe*****";

        Sleep(2000);

        paginaInicial();
    }
}

// Apos uma turma ser eliminada as turmas daquela classe serao reajustadas
// p/ex Caso a classe tem turmas A, B, C, D, e a turma B for eliminada
// as turmas naquela classe ficarao A, B, C, ao em vez de A, C, D
void Manager::reajustarAsTurmasAposEliminacao(string ano_de_escolaridade)
{
    string dir_path = Diretorios::ESCOLAS + escola.obterNomeDaEscola() + "/Classes/" + ano_de_escolaridade + "/";

    int indice = 0;
    bool ocorreu_erro = false;

    try
    {
        if(stoi(ano_de_escolaridade.c_str()) < 11)
        {
            if(std::filesystem::exists(dir_path))
            {
                for(const auto& it : std::filesystem::directory_iterator(dir_path))
                    if(std::filesystem::is_regular_file(it.path()))
                    {
                        util::set_a_FileAttribute_To_ReadOnly_or_Normal(it.path().string().c_str(), FILE_ATTRIBUTE_NORMAL);

                        string new_file_name = dir_path + vecTurmas[ indice++ ];

                        if (it.path().string() != new_file_name)
                        {
                            std::filesystem::rename(it.path(), new_file_name);

                            util::set_a_FileAttribute_To_ReadOnly_or_Normal(new_file_name.c_str(), FILE_ATTRIBUTE_READONLY);
                        }
                    }
            }
            else
                ocorreu_erro = true;
        }
        else
        {
            vector<string> file_name_3_ciclo;

            file_name_3_ciclo.push_back("CT");
            file_name_3_ciclo.push_back("ES");
            file_name_3_ciclo.push_back("H");

            int ind = 1;
            for( int j = 0, len = area_3_ciclo_path.size(); j < len; ++j)
                if(std::filesystem::exists(dir_path + area_3_ciclo_path[ j ]))
                {
                    for(const auto& it : std::filesystem::directory_iterator(dir_path + area_3_ciclo_path[ j ]))
                        if(std::filesystem::is_regular_file(it.path()))
                        {
                            string new_file_name = dir_path + area_3_ciclo_path[ j ] + file_name_3_ciclo[ j ] + to_string(ind++);

                            util::set_a_FileAttribute_To_ReadOnly_or_Normal(it.path().string().c_str(), FILE_ATTRIBUTE_NORMAL);

                            if (it.path().string() != new_file_name)
                            {
                                std::filesystem::rename(it.path(), new_file_name);
                                util::set_a_FileAttribute_To_ReadOnly_or_Normal(new_file_name.c_str(), FILE_ATTRIBUTE_READONLY);
                            }
                        }
                }
                else
                    ocorreu_erro = true;
        }
    }
    catch(std::exception& e)
    {
        cerr << endl << e.what();
        cin.get();
    }

    if(ocorreu_erro)
    {
        cerr << endl << endl << "\a" << "*****Erro Ao Reajustar Turmas Apos Eliminacao*****";

        Sleep(2000);

        paginaInicial();
    }
}

void Manager::verTurmas()
{
    cabecalho();

    cout << "   \t\t" << "\t\t" << "Ver Turmas" << endl << endl;

    cout << "   \t\t" << "1. Ver Por Classes" << endl << endl;
    cout << "   \t\t" << "2. Ver Todas As Turmas" << endl << endl;
    cout << "   \t\t" << "0. Voltar" << endl << endl;
    cout << "   \t\t" << "->";
    char opt = _getch();

    while (opt != '1' && opt != '2' && opt != '0' && opt != 27 && opt != 8)
    {
        opt = _getch();
    }

    switch(opt)
    {
    case '1':
        verTurmasPorClasse();
        break;

    case '2':
        verTodasAsTurmas();
        break;

    case '0':
    case 8:
        paginaInicialTurmas();
        break;

    case 27:
        paginaInicial();
        break;

    default:
        verTurmas();
        break;
    }
}

void Manager::verTurmasPorClasse()
{
    cabecalho();

    int registo_de_ano_escolaridade;
    string ano_escolaridade;

    cout << "   \t\t" << "\t\t" << "Ver Turmas" << endl << endl;

    cout << "   \t\t" << "Ano De Escolaridade (1 -12): ";
    ano_escolaridade = util::inputRestriction(2, 12, true, this, &Manager::verTurmas, registo_de_ano_escolaridade);

    string file_path = Diretorios::ESCOLAS + escola.obterNomeDaEscola() + "/Classes/" + ano_escolaridade + "/";

    verTurmasDaClasse(registo_de_ano_escolaridade, file_path);
}


void Manager::verTurmasDaClasse(int ano_escolaridade_, string file_path)
{
    cabecalho();

    cout << "   \t\t" << "\t\t" << "Ver Turmas" << endl << endl;

    cout << "   \t\t" << "Turmas Do " << ano_escolaridade_ << " Ano" << endl;


    bool sem_turmas = true, ocorreu_erro = false;

    if (ano_escolaridade_ < 11)
    {
        if( std::filesystem::exists(file_path))
        {
            for(const auto& it : std::filesystem::directory_iterator(file_path))
                if(std::filesystem::is_regular_file(it.path()))
                {
                    string turma = it.path().filename().string();
                    if(find(vecTurmas.begin(), vecTurmas.end(), turma) != vecTurmas.end())
                    {
                        cout << "   \t\t" << turma << endl;
                        sem_turmas = false;
                    }
                }
        }
        else
            ocorreu_erro = true;
    }
    else
    {
        for ( int i = 0,  len = area_3_ciclo_path.size(); i < len; ++i)
        {
            for(const auto& it : std::filesystem::directory_iterator(file_path + area_3_ciclo_path[i]))
                if(std::filesystem::is_regular_file(it.path()))
                {
                    string turma = it.path().filename().string();
                    if(find(vecTurmas3Ciclo.begin(), vecTurmas3Ciclo.end(), turma) != vecTurmas3Ciclo.end())
                    {
                        cout << "   \t\t" << turma << endl;
                        sem_turmas = false;
                    }
                    else
                        ocorreu_erro = true;
                }
        }
    }

    if(ocorreu_erro)
    {
        cerr << endl << endl << endl << "\a" << "*****Erro No Acesso As Classes*****";

        Sleep(2000);

        paginaInicial();
    }

    if (sem_turmas)
    {
        cout << endl << endl << "   \t\t" << "Nenhuma Turma Registada" << endl << endl;
        cout << "   \t\t" << "1. Registar Agora" << endl;
        cout << "   \t\t" << "0. Voltar" << endl;
        cout << "   \t\t" << "->";
        char opt = _getch();

        while(opt != '1' && opt != '0' && opt != '9' && opt != 27)
        {
            opt = _getch();
        }

        switch(opt)
        {
        case '1':
            registarTurmas();
            break;

        case '0':
            verTurmas();
            break;

        case 27:
            paginaInicial();
            break;
        }
    }
    else
    {
        /*
        deque <Estudante> alunos;
        string _ano_escolaridade_ = to_string(ano_escolaridade_);

        cout << endl << endl << "   \t\t" << "1. Entrar " << endl;
        cout << "   \t\t" << "0. Voltar " << endl;
        cout << "   \t\t" << "->";
        char ch = _getch();

        while (ch != '1' && ch != '2' && ch != 13)
        {
            ch = _getch();
        }

        if (ch == 1)
        {
            cout << endl << endl ;
        }

        //     extrairAlunosDeUmaTurma()

        cout << "Em Construcao";
        */
    }

    util::pressionarEnter();

    verTurmas();
}

void Manager::verTodasAsTurmas()
{
    cabecalho();

    cout << "   \t\t" << "\t\t" << "Ver Turmas" << endl << endl;

    cout << "   \t\t" << "Todas as Turmas" << endl << endl;

    bool sem_turmas = true;

    string file_path = Diretorios::ESCOLAS + escola.obterNomeDaEscola() + "/Classes/";

    for(int i = 1; i <= 10; ++i)
    {
        cout << endl << endl << "   \t\t" << "-> " << i << " Ano" << endl << endl;

        if(std::filesystem::exists(file_path + to_string(i)))
        {
            for(const auto& it : std::filesystem::directory_iterator(file_path + to_string(i)))
                if(std::filesystem::is_regular_file(it.path()))
                {
                    string turma = it.path().filename().string();
                    if(find(vecTurmas.begin(), vecTurmas.end(), turma) != vecTurmas.end())
                    {
                        cout << "   \t\t" << turma << endl;
                        sem_turmas = false;
                    }
                }
        }
        else
        {
            cerr << endl << endl << endl << "\a" << "*****Erro Ao Ver Turmas*****" << endl;

            Sleep(2000);

            paginaInicial();
        }

        if (sem_turmas)
            cout << "   \t\t" << "Nenhuma Tuma Registada";

        sem_turmas = true;
    }

    sem_turmas = true;

    for(int i = 11; i <= 12; ++i)
    {
        cout << endl << endl << "   \t\t" << "-> " << i << " Ano" << endl << endl;

        for ( int j = 0, len = area_3_ciclo_path.size(); j < len; ++j)
        {
            if(std::filesystem::exists(file_path + to_string(i) + "/" + area_3_ciclo_path[ j ]))
            {
                for(const auto& it : std::filesystem::directory_iterator(file_path + to_string(i) + "/" + area_3_ciclo_path[ j ]))
                    if(std::filesystem::is_regular_file(it.path()))
                    {
                        string turma = it.path().filename().string();
                        if(find(vecTurmas3Ciclo.begin(), vecTurmas3Ciclo.end(), turma) != vecTurmas3Ciclo.end())
                        {
                            cout << "   \t\t" << turma << endl;
                            sem_turmas = false;
                        }
                    }
            }
            else
            {
                cerr << endl << endl << endl << "\a" << "*****Erro Ao Ver Turmas*****" << endl;

                Sleep(2000);

                paginaInicial();
            }
        }

        if (sem_turmas)
            cout << "   \t\t" << "Nenhuma Tuma Registada";

        sem_turmas = true;
    }

    cout << endl << endl;

    util::pressionarEnter();

    verTurmas();
}

int Manager::contarNumeroDeTurmasDeUmaClasse(string ano_escolaridade)
{
    int numero_de_turmas = 0;
    bool ocorreu_erro = false;

    string file_path = Diretorios::ESCOLAS + escola.obterNomeDaEscola() + "/Classes/";

    if (stoi(ano_escolaridade.c_str()) < 11)
    {
        if(std::filesystem::exists(file_path + ano_escolaridade))
        {
            for(const auto& it : std::filesystem::directory_iterator(file_path + ano_escolaridade))
                if(std::filesystem::is_regular_file(it.path()))
                    if(find(vecTurmas.begin(), vecTurmas.end(),  it.path().filename().string()) != vecTurmas.end())
                        ++numero_de_turmas;
        }
        else
            ocorreu_erro = true;

    }
    else
    {
        for(int i = 11; i <= 12; ++i)
        {
            for ( int j = 0, len = area_3_ciclo_path.size(); j < len; ++j)
            {
                if(std::filesystem::exists(file_path + to_string(i) + "/" + area_3_ciclo_path[ j ]))
                {
                    for(const auto& it : std::filesystem::directory_iterator(file_path + to_string(i) + "/" + area_3_ciclo_path[ j ]))
                        if(std::filesystem::is_regular_file(it.path()))
                            if(find(vecTurmas3Ciclo.begin(), vecTurmas3Ciclo.end(),  it.path().filename().string()) != vecTurmas3Ciclo.end())
                                ++numero_de_turmas;
                }
                else
                    ocorreu_erro = true;
            }
        }
    }

    if(ocorreu_erro)
    {
        cerr << endl << endl << endl << "\a" << "*****Erro Ao Contar Numero De Turmas*****" << endl;

        Sleep(2000);

        paginaInicial();
    }

    return numero_de_turmas;
}

void Manager::verNumeroDeTurmas()
{
    cabecalho();

    cout << "   \t\t" << "\t\t" << "Ver Numero de Turmas" << endl << endl;

    cout << "   \t\t" << "1. Ver De Uma Classes" << endl << endl;
    cout << "   \t\t" << "2. Ver De Toda A Escola" << endl << endl;
    cout << "   \t\t" << "0. Voltar" << endl << endl;
    cout << "   \t\t" << "->";
    char opt = _getch();

    while (opt != '1' && opt != '2' && opt != '0' && opt != 27 && opt != 8)
    {
        opt = _getch();
    }

    switch(opt)
    {
    case '1':
        verNumeroDeTurmasDeUmaClasse();
        break;

    case '2':
        verNumeroDeTurmasDeTodaAEscola();
        break;

    case '0':
    case 8:
        paginaInicialTurmas();
        break;

    case 27:
        paginaInicial();
        break;

    default:
        verNumeroDeTurmas();
        break;
    }

}

void Manager::verNumeroDeTurmasDeUmaClasse()
{
    cabecalho();

    cout << "   \t\t" << "\t\t" << "Ver Numero de Turmas De Uma Classe" << endl << endl;

    int registo_de_ano_escolaridade;

    cout << "   \t\t" << "Ano De Escolaridade (1 -12): ";
    string ano_escolaridade = util::inputRestriction(2, 12, true, this, &Manager::verNumeroDeTurmas, registo_de_ano_escolaridade);

    cout << endl << endl;

    cout << "   \t\t" << ano_escolaridade << " Ano Contem " << contarNumeroDeTurmasDeUmaClasse(ano_escolaridade) << " Turmas";

    cout << endl << endl;

    util::pressionarEnter();

    paginaInicialTurmas();
}

void Manager::verNumeroDeTurmasDeTodaAEscola()
{
    cabecalho();

    cout << "   \t\t" << "\t\t" << "Ver Numero de Turmas De Toda A Escola" << endl << endl;

    int numero_de_turmas = 0;

    for (int i = 1; i <= 12; ++i)
    {
        string ano_escolaridade;

        ano_escolaridade = to_string(i);

        numero_de_turmas += contarNumeroDeTurmasDeUmaClasse( ano_escolaridade );
    }

    cout << "   \t\t" << "A Escola Contem " << numero_de_turmas << " Turmas";

    cout << endl << endl;

    util::pressionarEnter();

    paginaInicialTurmas();
}

void Manager::paginaInicialAlunos()
{
    cabecalho();

    cout << "   \t\t" << "\t\t" << "Alunos" << endl << endl;

    cout << "   \t\t" << "1. Registar Alunos" << endl << endl;

    cout << "   \t\t" << "2. Ver Alunos" << endl << endl;

    cout << "   \t\t" << "0. Voltar" << endl << endl;

    char opt;

    cout << "   \t\t" << "->";
    opt = _getch();

    while (opt != '1' && opt != '2' && opt != '0' && opt != 27 && opt != 8)
    {
        opt = _getch();
    }

    switch(opt)
    {
    case '1':
        registarAlunos();
        break;

    case '2':
        verAlunos();
        break;


    case '0':
    case 8:
        paginaInicial();
        break;

    case 27:
        paginaInicial();
        break;

    default:
        paginaInicialAlunos();
        break;
    }
}

void Manager::inserirTurma(string& turma_path, string& ano_escolaridade, string& turma, auto (Manager::*funcao_de_retorno) ())
{
    int registo_de_ano_escolaridade;

    string turma_3_ciclo;

    cout << "   \t\t" << "  Ano De Escolaridade (1 -12): ";
    ano_escolaridade = util::inputRestriction(2, 12, true, this, funcao_de_retorno, registo_de_ano_escolaridade);

    turma_path = Diretorios::ESCOLAS + escola.obterNomeDaEscola() + "/Classes/" + ano_escolaridade + "/";

    if (registo_de_ano_escolaridade < 11)
    {
        cout << endl << endl << "   \t\t" << "  Turma (A - Z):";
        getline(cin, turma);

        if (turma == "0")
            (this->*funcao_de_retorno)();

        while (turma.empty())
        {
            cout << endl << "   \t\t" << "  Turma Obrigatorio (A - Z):";
            getline(cin, turma);

            if (turma == "0")
                (this->*funcao_de_retorno)();
        }

        if (islower(turma[0]))
            turma[0] = toupper(turma[0]);

        while (! turmaExiste(turma_path, turma))
        {
            cout << endl << "   \t\t" << "   Turma Nao Existe";
            cout << endl << endl << "   \t\t" << "    1-> Entrar de novo" ;
            cout << setw(25) << "2-> Registar Turmas";
            cout  << setw(20) << "0-> Cancelar";
            cout << endl << endl << "   \t\t" << "   Escolha a opcao: ";
            char opt = _getch();

            while (opt != '1' && opt != '2' && opt != '0')
            {
                opt = _getch();
            }

            if (opt == '1')
            {
                cout << endl << endl << "   \t\t" << "  Turma (A - Z):";
                getline(cin, turma);

                if (turma == "0")
                    (this->*funcao_de_retorno)();

                while (turma.empty())
                {
                    cout << endl << "   \t\t" << "  Turma Obrigatorio (A - Z):";
                    getline(cin, turma);

                    if (turma == "0")
                        (this->*funcao_de_retorno)();
                }

                if (islower(turma[0]))
                    turma[0] = toupper(turma[0]);
            }
            else if (opt =='2')
                registarTurmas();
            else if (opt == '0')
                (this->*funcao_de_retorno)();
        }
    }
    else
    {
        int opcao;

        cout << endl << endl << "   \t\t" << "  1-> Ciencias e Tecnologias (CT)" ;
        cout << setw(28) << "2-> Economico-Social (ES)";
        cout  << setw(23) << "3-> Humasnitica (H)";
        cout << endl << endl << "   \t\t" << "  Escolha a opcao: ";
        util::inputRestriction(1, 3, true, this, funcao_de_retorno, opcao);

        if (opcao == 1)
        {
            cout << endl << endl << "   \t\t" << "  Turma CT:";
            getline(cin, turma);

            if (turma == "0")
                (this->*funcao_de_retorno)();

            turma_path += "CT/";
        }
        else if (opcao == 2)
        {
            cout << endl << endl << "   \t\t" << "  Turma ES:";
            getline(cin, turma);

            if (turma == "0")
                (this->*funcao_de_retorno)();

            turma_path += "ES/";
        }
        else if (opcao == 3)
        {
            cout << endl << endl << "   \t\t" << "  Turma H:";
            getline(cin, turma);

            if (turma == "0")
                (this->*funcao_de_retorno)();

            turma_path += "H/";
        }

        if (opcao == 1)
            turma_3_ciclo = "CT" + turma;
        else if (opcao == 2)
            turma_3_ciclo = "ES" + turma;
        else if (opcao == 3)
            turma_3_ciclo = "H" + turma;

        while (! turmaExiste(turma_path, turma_3_ciclo))
        {
            cout << endl << "   \t\t" << "   Turma Nao Existe";
            cout << endl << endl << "   \t\t" << "    1-> Entrar de novo";
            cout << setw(25) << "2-> Registar Turmas";
            cout  << setw(20) << "0-> Cancelar";
            cout << endl << endl << "   \t\t" << "   Escolha a opcao: ";
            char opt = _getch();

            while (opt != '1' && opt != '2' && opt != '0')
            {
                opt = _getch();
            }

            if (opt == '1')
            {
                cout << endl << endl << "   \t\t" << "  Turma ";

                if (opcao == 1)
                    cout << "CT:";
                else if (opcao == 2)
                    cout << "ES:";
                else if (opcao == 3)
                    cout << "H:";

                getline(cin, turma);

                if (opcao == 1)
                    turma_3_ciclo = "CT" + turma;
                else if (opcao == 2)
                    turma_3_ciclo = "ES" + turma;
                else if (opcao == 3)
                    turma_3_ciclo = "H" + turma;

            }
            else if (opt =='2')
                registarTurmas();
            else if (opt == '0')
                (this->*funcao_de_retorno)();
        }
    }

    if (registo_de_ano_escolaridade < 11)
        turma_path += turma;
    else
    {
        turma = turma_3_ciclo;
        turma_path += turma_3_ciclo;
    }
}

void Manager::inserirAnoDeEscolaridadeEntreClasses(int& registo_de_ano_escolaridadeA, int& registo_de_ano_escolaridadeB,
        auto (Manager::*funcao_de_retorno) ( ))
{
    cout << "   \t\t" << "Ano De Escolaridade Inicial (1 -12): ";
    util::inputRestriction(2, 12, true, this, funcao_de_retorno, registo_de_ano_escolaridadeA);

    cout << endl << endl;

    cout << "   \t\t" << "Ano De Escolaridade Final (1 -12): ";
    util::inputRestriction(2, 12, true, this, funcao_de_retorno, registo_de_ano_escolaridadeB);

    while ((registo_de_ano_escolaridadeA > registo_de_ano_escolaridadeB) || (registo_de_ano_escolaridadeA == registo_de_ano_escolaridadeB))
    {
        cout << endl << endl << "   \t\t" << " Invalido!! Entre De Novo" << endl;

        cout << "   \t\t" << "Ano De Escolaridade Inicial (1 -12): ";
        util::inputRestriction(2, 12, true, this, funcao_de_retorno, registo_de_ano_escolaridadeA);

        cout << endl << endl;

        cout << "   \t\t" << "Ano De Escolaridade Final (1 -12): ";
        util::inputRestriction(2, 12, true, this, funcao_de_retorno, registo_de_ano_escolaridadeB);
    }
}

void Manager::registarAlunos()
{
    cabecalho();

    time_t now = time(nullptr);
    tm* agora = localtime(&now);

    int presente_ano = 1900 + agora->tm_year;

    string nome, apelido, genero, idade, morada, nacionalidade, estado_civil, numero_identificacao_civil;

    string ano_escolaridade_, turma_;
    string turma_3_ciclo;

    int ano_nascimento, mes_nascimento, dia_nascimento;

    cout << "   \t\t" << "\t\t" << "Registar Alunos" << endl << endl;

    cout << "   \t\t" << "-> Informacoes Pessoais" << endl << endl;

    cout << "   \t\t" << "  Nome: ";
    util::validarInput(nome, this, &Manager::paginaInicialAlunos);

    cout << endl << "   \t\t" <<  "  Apelido: ";
    util::validarInput(apelido, this, &Manager::paginaInicialAlunos);


    cout << endl << "   \t\t" << "  Data De Nascimento: " << endl;
    cout << "   \t\t" << "   Ano: ";

    bool ano_nascimento_minimo = true;

    while(ano_nascimento_minimo == true)
    {
        util::inputRestriction(4, presente_ano, false, this, &Manager::paginaInicialAlunos, ano_nascimento);

        if(ano_nascimento < 1904)
        {
            cout << "\b \b\b \b\b \b\b \b";;
        }
        else
            ano_nascimento_minimo = false;
    }

    cout << endl << "   \t\t" << "   Mes: ";
    util::inputRestriction(2, 12, true, this, &Manager::paginaInicialAlunos, mes_nascimento);

    cout << endl << "   \t\t" << "   Dia: ";
    util::inputRestriction(2, 31, true, this, &Manager::paginaInicialAlunos, dia_nascimento);

    while (! util::dataValido(ano_nascimento, mes_nascimento, dia_nascimento))
    {
        cout << endl << endl << "   \t\t" <<  "  Data Invalido" << endl;
        cout << "   \t\t" << "   Ano: ";

        bool ano_nascimento_minimo = true;

        while(ano_nascimento_minimo == true)
        {
            util::inputRestriction(4, presente_ano, false, this, &Manager::paginaInicialAlunos, ano_nascimento);

            if(ano_nascimento < 1904)
            {
                cout << "\b \b\b \b\b \b\b \b";
            }
            else
                ano_nascimento_minimo = false;
        }

        util::inputRestriction(4, presente_ano, false, this, &Manager::paginaInicialAlunos, ano_nascimento);

        cout << endl << "   \t\t" << "   Mes: ";
        util::inputRestriction(2, 12, true, this, &Manager::paginaInicialAlunos, mes_nascimento);

        cout << endl << "   \t\t" << "   Dia: ";
        util::inputRestriction(2, 31, true, this, &Manager::paginaInicialAlunos, dia_nascimento);
    }

    cout << endl << endl << "   \t\t" << "  Genero:" << endl;
    cout << "   \t\t" << "   1 -> Masculino" << endl;
    cout << "   \t\t" << "   2 -> Feminino" << endl;
    cout << "   \t\t" << "   ->";
    int opt;
    util::inputRestriction(1, 2, true, this, &Manager::paginaInicialAlunos, opt);

    if ( opt == 1)
        genero = "M";
    if (opt == 2)
        genero = "F";

    cout << endl << endl << "   \t\t" << "  Morada: ";
    util::validarInput(morada, this, &Manager::paginaInicialAlunos);

    // cout << endl << "   \t\t" << "  Nacionalidade: ";
    // validarInput(nacionalidade, &Manager::paginaInicialAlunos);

    nacionalidade = "Caboverdiana";

    cout << endl << "   \t\t" << "  Estado Civil:" << endl;
    cout << "   \t\t" << "   1 -> Solteiro" << endl;
    cout << "   \t\t" << "   2 -> Casado" << endl;
    cout << "   \t\t" << "   ->";
    util::inputRestriction(1, 2, true, this, &Manager::paginaInicialAlunos, opt);

    if ( opt == 1)
        estado_civil = "Solteiro";
    if (opt == 2)
        estado_civil = "Casado";

    cout << endl << endl << "   \t\t" << "  Numero de Identificacao Civil: ";
    numero_identificacao_civil = util::inputRestriction(6, 1000000, false, this, &Manager::paginaInicialAlunos, opt); // esse int "opt' é passado como parametro apenas para completar a funcao, nao será usada

    cout << endl << endl << "   \t\t" << "-> Informacoes Como Estudante" << endl << endl;

    string file_path;

    inserirTurma(file_path, ano_escolaridade_, turma_, &Manager::paginaInicialAlunos);

    util::regularizarNomes(nome);

    string info_estudante = nome + " " +
                            apelido + " " +
                            genero + " " +
                            to_string(dia_nascimento) + " " +
                            to_string(mes_nascimento) + " " +
                            to_string(ano_nascimento) + " " +
                            morada + " " +
                            nacionalidade + " "
                            + estado_civil + " " +
                            numero_identificacao_civil + " " +
                            ano_escolaridade_ + " " +
                            turma_ + " " + "1";


    if (util::seFicheiroVazio(this, &paginaInicial, file_path, "registarAlunos"))
    {
        util::set_a_FileAttribute_To_ReadOnly_or_Normal(file_path, FILE_ATTRIBUTE_NORMAL);

        ofstream file_out(file_path);

        Estudante est;
        est.registar(info_estudante);

        file_out << est << endl;

        file_out.close();

        util::set_a_FileAttribute_To_ReadOnly_or_Normal(file_path, FILE_ATTRIBUTE_READONLY);
    }
    else
    {
        Estudante est;
        est.registar(info_estudante);

        atribuirNumeroAosAlunos_e_OrganizarPorOdemAlfabetica(file_path, est);
    }

    system("cls");

    cout << endl << "   \t\t\t\t" << "Registo Concluido" << endl << endl;

    Sleep(1000);

    paginaInicialAlunos();
}

// essa funcao será chamada toda vez que o programa for executado e serve para atualizar as idades dos alunos
// essa vai chamar a funcao 'atualizarTurmaAposEdicao()' que embora foi criada para um toutro serve perfeitamente nessa situacao
void Manager::atualizarIdadeDosAlunosdeTodaAEscola()
{
    string file_path = Diretorios::ESCOLAS + escola.obterNomeDaEscola() + "/Classes/";

    for (int i = 1; i < 11; ++i)
    {
        string ano_esc = to_string(i);

        if(std::filesystem::exists(file_path + ano_esc))
        {
            for(const auto& it : std::filesystem::directory_iterator(file_path + ano_esc))
                if(std::filesystem::is_regular_file(it.path()))
                    if(find(vecTurmas.begin(), vecTurmas.end(),  it.path().filename().string()) != vecTurmas.end())
                    {
                        deque<Estudante> deque_alunos;

                        if(! util::seFicheiroVazio(this, &paginaInicial, it.path().string(), "atualizarIdadeDosAlunosdeTodaAEscola"))
                        {
                            extrairAlunosDeUmaTurma(deque_alunos, it.path().string());

                            atualizarTurmaAposEdicao(it.path().string(), deque_alunos);
                        }
                    }
        }

    }

    for (int i = 11; i <= 12; ++i)
    {
        string ano_esc = to_string(i);
        file_path += ano_esc + "/";

        for( int j = 0, len = area_3_ciclo_path.size(); j < len; ++j)
        {
            string _path = file_path + area_3_ciclo_path[j];

            if(std::filesystem::exists(_path))
            {
                for(const auto& it : std::filesystem::directory_iterator(_path))
                    if(std::filesystem::is_regular_file(it.path()))
                        if(find(vecTurmas3Ciclo.begin(), vecTurmas3Ciclo.end(),  it.path().filename().string()) != vecTurmas3Ciclo.end())
                        {
                            deque<Estudante> deque_alunos;

                            if(! util::seFicheiroVazio(this, &paginaInicial, it.path().string(), "atualizarIdadeDosAlunosdeTodaAEscola"))
                            {
                                extrairAlunosDeUmaTurma(deque_alunos, it.path().string());

                                atualizarTurmaAposEdicao(it.path().string(), deque_alunos);
                            }
                        }
            }

        }
    }

}

void Manager::atribuirNumeroAosAlunos_e_OrganizarPorOdemAlfabetica(std::string file_path, Estudante estud )
{
    deque<Estudante> dequeEstdnt;

    dequeEstdnt.push_back(estud); // adicionar o aluno recem-registado

    extrairAlunosDeUmaTurma(dequeEstdnt, file_path);

    sort(dequeEstdnt.begin(), dequeEstdnt.end(), ordenarAlunosPorNomeCrescente);

    for ( int i = 0, len = dequeEstdnt.size(); i < len; ++i)
    {
        string str_numero_do_aluno = to_string(i+1);
        dequeEstdnt[i].numero_do_aluno = str_numero_do_aluno;
    }

    util::set_a_FileAttribute_To_ReadOnly_or_Normal(file_path, FILE_ATTRIBUTE_NORMAL);

    ofstream file(file_path);

    for (auto e : dequeEstdnt)
        file << e << endl;

    file.close();

    util::set_a_FileAttribute_To_ReadOnly_or_Normal(file_path, FILE_ATTRIBUTE_READONLY);
}

// funcao para ver informacoes dos alunos que serao passao num container deque
// o bool é usado para sinalizar caso o usuario tem tiver 2 opcoes de vista
void Manager::displayAlunos(deque<Estudante> deque_alunos_por_turma, bool opcao_vista)
{
    int len = deque_alunos_por_turma.size();

    int d = 0;

    for ( int i = 0; i < len; ++i)
    {
        int temp = deque_alunos_por_turma[i].nome_completo.size();

        if(temp > d)
            d = temp;
    }

    d +=10;

    char ch;
    if (opcao_vista == true)
    {
        cout << "   \t\t" << "1. Ver Basico" << endl;
        cout << "   \t\t" << "2. Ver Completo" << endl;
        cout << "   \t\t"  << "->";
        ch = _getch();

        while (ch != '1' && ch != '2')
        {
            ch = _getch();
        }

        cout << ch << endl << endl;

        if (ch == '1')
        {
            cout <<  left << "   \t\t" << setw(d) << "Nome" << setw(5) << "N." << setw(10) << "Turma";
            cout << setw(10) << "Idade" << setw(30) << "Morada"<< endl;

            for ( int i = 0; i < len; ++i)
            {
                util::adicionarEspacos(deque_alunos_por_turma[i].nome_completo);
                util::adicionarEspacos(deque_alunos_por_turma[i].morada);

                cout << "   \t\t" << setw(d) << deque_alunos_por_turma[i].nome_completo;
                cout << setw(5) << deque_alunos_por_turma[i].numero_do_aluno;
                cout << setw(10) << deque_alunos_por_turma[i].ano_escolaridade + deque_alunos_por_turma[i].turma;
                cout << setw(10) << deque_alunos_por_turma[i].idade;
                cout << setw(30) << deque_alunos_por_turma[i].morada << endl;
            }
        }
    }

    if (ch == '2' || ! opcao_vista)
    {
        util::ajustarTamanhoJanela(util::TamanhoConsole::Aumentada);

        cout <<  left << setw(d) << "Nome" << setw(5) << "N." << setw(10) << "Turma";
        cout << setw(10) << "Genero" << setw(22) << "Data De Nascimento" <<  setw(10) << "Idade";
        cout << setw(18) << "Estado Civil" << setw(15) << "N. BI" << setw(30) << "Morada" << endl;

        for ( int i = 0; i < len; ++i)
        {
            util::adicionarEspacos(deque_alunos_por_turma[i].nome_completo);
            util::adicionarEspacos( deque_alunos_por_turma[i].morada);

            cout << setw(d) << deque_alunos_por_turma[i].nome_completo;
            cout << setw(5) << deque_alunos_por_turma[i].numero_do_aluno;
            cout << setw(10) << deque_alunos_por_turma[i].ano_escolaridade + deque_alunos_por_turma[i].turma;
            cout << setw(10) << deque_alunos_por_turma[i].genero;
            cout << setw(22) << deque_alunos_por_turma[i].data_de_nascimento;
            cout << setw(10) << deque_alunos_por_turma[i].idade;
            cout << setw(18) << deque_alunos_por_turma[i].estado_civil;
            cout << setw(15) << deque_alunos_por_turma[i].numero_identificacao_civil;
            cout << setw(30) << deque_alunos_por_turma[i].morada << endl;
        }
    }

    if( opcao_vista )
    {
        util::pressionarEnter();

        util::ajustarTamanhoJanela(util::TamanhoConsole::Normal);
    }

    cout << right;
}

void Manager::extrairAlunosDeUmaTurma(deque<Estudante>& dequeEstdnt, string file_path)
{
    ifstream file_in(file_path);

    if (! file_in.good())
    {
        cerr << endl << endl << "\a" << "*****Erro Ao Extrair Alunos De Uma Turma*****";

        Sleep(2000);

        paginaInicial();
    }

    if( util::seFicheiroVazio(this, &paginaInicial, file_path, "extrairAlunosDeUmaTurma") )
    {
        cerr << endl << endl << "\a" << "*****Erro: Ficheiro Vazio Ao Extrair Alunos De Uma Turma*****";

        Sleep(2000);

        paginaInicial();
    }

    string line;

    while (getline(file_in, line))
    {
        Estudante est;

        est.registar(line);

        dequeEstdnt.push_back(est);
    }

    file_in.close();
}

void Manager::extrairAlunosDeUmaClasse(deque<Estudante>& deque_alunos_por_classe, string file_path, string ano_escolaridade)
{
    bool ocorreu_erro = false;

    int ano_escolaridade_ = stoi(ano_escolaridade.c_str());

    if (ano_escolaridade_ < 11)
    {
        if(std::filesystem::exists(file_path))
        {
            for(const auto& it : std::filesystem::directory_iterator(file_path))
            {
                if(find(vecTurmas.begin(), vecTurmas.end(), it.path().filename().string()) != vecTurmas.end())
                {
                    if(! util::seFicheiroVazio(this, &paginaInicial, it.path().string(), "extrairAlunosDeUmaClasse"))
                        extrairAlunosDeUmaTurma(deque_alunos_por_classe, it.path().string());
                }
            }
        }
        else
            ocorreu_erro = true;
    }
    else
    {
        for( int j = 0, len =  area_3_ciclo_path.size(); j < len; ++j)
        {
            if(std::filesystem::exists(file_path + area_3_ciclo_path[ j ]))
            {
                for(const auto& it : std::filesystem::directory_iterator(file_path + area_3_ciclo_path[ j ]))
                    if(find(vecTurmas3Ciclo.begin(), vecTurmas3Ciclo.end(), it.path().filename().string()) != vecTurmas3Ciclo.end())
                    {
                        if(! util::seFicheiroVazio(this, &paginaInicial, it.path().string(), "extrairAlunosDeUmaClasse"))
                            extrairAlunosDeUmaTurma(deque_alunos_por_classe, it.path().string());
                    }
            }
            else
                ocorreu_erro = true;
        }
    }

    if(ocorreu_erro)
    {
        cerr << endl << endl << "\a" << "*****Erro Ao Extrair Alunos De Uma Classe*****";

        Sleep(2000);

        paginaInicial();
    }
}

void Manager::extrairAlunosDeTodaAEscola(deque <Estudante>& alunos)
{
    for(int i = 1; i <= 12; ++i)
    {
        string classe_ = to_string(i);

        string path = Diretorios::ESCOLAS + escola.obterNomeDaEscola() + "/Classes/" + classe_ + "/";

        extrairAlunosDeUmaClasse(alunos, path, classe_);
    }
}

void Manager::verAlunos()
{
    cabecalho();

    cout << "   \t\t" << "\t\t" << "Ver Alunos" << endl << endl;

    cout << "   \t\t" << "1. Ver Por Classe" << endl << endl;
    cout << "   \t\t" << "2. Ver Por Turma" << endl << endl;
    cout << "   \t\t" << "3. Ver Todos Os Alunos Da Escola" << endl << endl;
    cout << "   \t\t" << "4. Ver Numero De Alunos" << endl << endl;
    cout << "   \t\t" << "5. Pesquisar" << endl << endl;
    cout << "   \t\t" << "6. Editar" << endl << endl;
    cout << "   \t\t" << "0. Voltar" << endl << endl;
    cout << "   \t\t" << "->";
    char opt = _getch();

    while (opt != '1' && opt != '2' && opt != '3' && opt != '4' && opt != '5' && opt != '6' && opt != '0' && opt != 8 && opt != 27 )
    {
        opt = _getch();
    }

    switch(opt)
    {
    case '1':
        verAlunosPorClasse();
        break;

    case '2':
        verAlunosPorTurma();
        break;

    case '3':
        verTodosOsAlunosDaEscola();
        break;

    case '4':
        selecionarGeneroParaVer();
        break;

    case '5':
        pesquisarAlunos();
        break;

    case '6':
        editarAlunos();
        break;

    case '0':
    case 8:
        paginaInicialAlunos();
        break;

    case 27:
        paginaInicial();
        break;

    default:
        verAlunos();
        break;
    }
}

void Manager::verAlunosPorClasse()
{
    cabecalho();

    string ano_escolaridade_;
    deque<Estudante> deque_alunos_por_turma;
    int registo_de_ano_escolaridade;

    cout <<"   \t\t" << "\t" << "Ver Alunos Numa Classe" << endl << endl;

    cout << "   \t\t" << "  Ano De Escolaridade (1 -12): ";
    ano_escolaridade_ = util::inputRestriction(2, 12, true, this, &Manager::verAlunos, registo_de_ano_escolaridade);

    string file_path = Diretorios::ESCOLAS + escola.obterNomeDaEscola() + "/Classes/" + ano_escolaridade_ + "/";

    extrairAlunosDeUmaClasse(deque_alunos_por_turma, file_path, ano_escolaridade_);

    if (deque_alunos_por_turma.empty())
    {
        cout << endl << endl << "   \t\t" << "Nenhum Aluno Registado" << endl << endl;
        cout << "   \t\t" << " 1. Registar Alunos" << endl;
        cout << "   \t\t" << " 2. Entrar De Novo" << endl;
        cout << "   \t\t" << " 0. Cancelar" << endl;
        cout << "   \t\t" << " ->";
        char ch = _getch();

        while (ch != '1' && ch != '2' && ch != '0')
        {
            ch = _getch();
        }

        switch(ch)
        {
        case '1':
            registarAlunos();
            break;

        case '2':
            verAlunosPorClasse();
            break;

        case '0':
            verAlunos();
            break;

        default:
            verAlunos();
            break;
        }
    }

    ordenar(deque_alunos_por_turma);

    cabecalho();

    cout << "   \t\t" << "Alunos Do " << ano_escolaridade_ + " Ano" << endl << endl;

    displayAlunos(deque_alunos_por_turma, true);

    verAlunos();
}

void Manager::verAlunosPorTurma()
{
    cabecalho();

    cout << "   \t\t" << "\t" << "Ver Alunos Numa Turma" << endl << endl;

    string file_path, ano_escolaridade_, turma_;

    inserirTurma(file_path, ano_escolaridade_, turma_, &Manager::verAlunos);

    deque<Estudante> deque_alunos_por_turma;

    bool se_ficheiro_vazio = util::seFicheiroVazio(this, &paginaInicial, file_path, "verAlunosPorTurma");

    if( ! se_ficheiro_vazio)
        extrairAlunosDeUmaTurma(deque_alunos_por_turma, file_path);

    cabecalho();

    cout << "   \t\t" << "Alunos Da Turma " << ano_escolaridade_ + turma_;

    if (se_ficheiro_vazio)
    {
        cout << "\n\n   \t\t" << " *** Turma Selecionada Nao Contem Alunos ***" << endl << endl;
        cout << "   \t\t" << " 1. Registar Alunos" << endl;
        cout << "   \t\t" << " 2. Entrar De Novo" << endl;
        cout << "   \t\t" << " 0. Cancelar" << endl;
        cout << "   \t\t" << " ->";
        char ch = _getch();

        while (ch != '1' && ch != '2' && ch != '0')
        {
            ch = _getch();
        }

        switch(ch)
        {
        case '1':
            registarAlunos();
            break;

        case '2':
            verAlunosPorTurma();
            break;

        case '0':
            verAlunos();
            break;

        default:
            verAlunos();
            break;
        }
    }
    else
    {
        ordenar(deque_alunos_por_turma);

        cabecalho();

        cout << "   \t\t" << "Alunos Da Turma " << ano_escolaridade_ + turma_ << endl << endl;

        displayAlunos(deque_alunos_por_turma, true);

        verAlunos();
    }

}

void Manager::verTodosOsAlunosDaEscola()
{
    cabecalho();

    cout << "   \t\t" << "\t" << "Todos Os Alunos Da Escola";

    deque<Estudante> deque_alunos;

    extrairAlunosDeTodaAEscola(deque_alunos);

    if(deque_alunos.empty())
    {
        cout << endl << endl << "Vazio..." << endl;

        util::pressionarEnter();
        verAlunos();
    }

    ordenar(deque_alunos);

    cabecalho();

    cout << "   \t\t" << "\t" << "Todos Os Alunos Da Escola";

    cout << endl << endl;

    displayAlunos(deque_alunos, true);

    verAlunos();
}

void Manager::selecionarGeneroParaVer()
{
    cabecalho();

    cout << "   \t\t" << "\t" << "Ver Numero De Alunos" << endl << endl;

    cout << "   \t\t" << "1. Masculino" << endl << endl;
    cout << "   \t\t" << "2. Feminino" << endl << endl;
    cout << "   \t\t" << "3. Todos" << endl << endl;
    cout << "   \t\t" << "0. Voltar" << endl << endl;
    cout << "   \t\t" << "->";
    char opt = _getch();

    while (opt != '1' && opt != '2' && opt != '3' && opt != '0' && opt != 8 && opt != 27 )
    {
        opt = _getch();
    }

    if (opt == '0' || opt == 8)
        verAlunos();
    else if (opt == 27)
        paginaInicial();

    verNumeroDeAlunos(opt);
}

void Manager::verNumeroDeAlunos(char opcao_de_genero)
{
    cabecalho();

    cout << "   \t\t" << "\t" << "Ver Numero De Alunos" << endl << endl;

    cout << "   \t\t" << "1. Numa Classe" << endl << endl;
    cout << "   \t\t" << "2. Entre Classes" << endl << endl;
    cout << "   \t\t" << "3. Em Toda A Escola" << endl << endl;
    cout << "   \t\t" << "0. Voltar" << endl << endl;
    cout << "   \t\t" << "->";
    char opt = _getch();

    while (opt != '1' && opt != '2' && opt != '3' && opt != '0' && opt != 8 && opt != 27 )
    {
        opt = _getch();
    }

    switch(opt)
    {
    case '1':
        verNumeroDeAlunosNumaClasse(opcao_de_genero);
        break;

    case '2':
        verNumeroDeAlunosEntreClasses(opcao_de_genero);
        break;

    case '3':
        verNumeroDeAlunosEmTodaAEscola(opcao_de_genero);
        break;

    case '0':
    case 8:
        selecionarGeneroParaVer();
        break;

    case 27:
        paginaInicial();
        break;

    default:
        verNumeroDeAlunos(opt);
        break;
    }

}

void Manager::verNumeroDeAlunosNumaClasse(char opcao_de_genero)
{
    cabecalho();

    if (opcao_de_genero == '1')
        cout << "   \t\t" << "Ver Numero De Alunos Do Sexo Masculino Numa Classe" << endl << endl;
    else if (opcao_de_genero == '2')
        cout << "   \t\t" << "Ver Numero De Alunos Do Sexo Feminino Numa Classe" << endl << endl;
    else if (opcao_de_genero == '3')
        cout << "   \t\t" << "Ver Numero De Alunos Numa Classe" << endl << endl;


    int registo_de_ano_escolaridade;
    deque<Estudante> deque_alunos;

    cout << "   \t\t" << "Ano De Escolaridade (1 -12): ";
    string ano_escolaridade_ = util::inputRestriction(2, 12, true, this, &Manager::selecionarGeneroParaVer, registo_de_ano_escolaridade);

    string file_path = Diretorios::ESCOLAS + escola.obterNomeDaEscola() + "/Classes/" + ano_escolaridade_ + "/";

    extrairAlunosDeUmaClasse(deque_alunos, file_path, ano_escolaridade_);

    int numero_de_alunos = contarNumeroDeAlunosPeloGenero(deque_alunos, opcao_de_genero);

    cout << endl << endl << "   \t\t" << " Numero De Alunos: " << numero_de_alunos << endl << endl;

    util::pressionarEnter();

    verAlunos();

}

void Manager::verNumeroDeAlunosEntreClasses(char opcao_de_genero)
{
    cabecalho();

    if (opcao_de_genero == '1')
        cout << "   \t\t" << "Ver Numero De Alunos Do Sexo Masculino Entre Classes" << endl << endl;
    else if (opcao_de_genero == '2')
        cout << "   \t\t" << "Ver Numero De Alunos Do Sexo Feminino Entre Classes" << endl << endl;
    else if (opcao_de_genero == '3')
        cout << "   \t\t" << "Ver Numero De Alunos Entre Classes" << endl << endl;

    deque<Estudante> deque_alunos;
    int registo_de_ano_escolaridadeA, registo_de_ano_escolaridadeB;

    inserirAnoDeEscolaridadeEntreClasses(registo_de_ano_escolaridadeA, registo_de_ano_escolaridadeB, &Manager::selecionarGeneroParaVer);

    for(int i = registo_de_ano_escolaridadeA; i <= registo_de_ano_escolaridadeB; ++i)
    {
        string classe_ = to_string(i);

        string file_path = Diretorios::ESCOLAS + escola.obterNomeDaEscola() + "/Classes/" + classe_ + "/";

        extrairAlunosDeUmaClasse(deque_alunos, file_path, classe_);
    }

    int numero_de_alunos = contarNumeroDeAlunosPeloGenero(deque_alunos, opcao_de_genero);

    cout << endl << endl << "   \t\t" << " Numero De Alunos: " << numero_de_alunos << endl << endl;

    util::pressionarEnter();

    verAlunos();
}

void Manager::verNumeroDeAlunosEmTodaAEscola(char opcao_de_genero)
{
    cabecalho();

    if (opcao_de_genero == '1')
        cout << "   \t\t" << "Ver Numero De Alunos Do Sexo Masculino Em Toda A Escola" << endl << endl;
    else if (opcao_de_genero == '2')
        cout << "   \t\t" << "Ver Numero De Alunos Do Sexo Feminino Em Toda A Escola" << endl << endl;
    else if (opcao_de_genero == '3')
        cout << "   \t\t" << "Ver Numero De Alunos Em Toda A Escola" << endl << endl;

    deque<Estudante> deque_alunos;

    extrairAlunosDeTodaAEscola(deque_alunos);

    int numero_de_alunos = contarNumeroDeAlunosPeloGenero(deque_alunos, opcao_de_genero);

    cout << "   \t\t" << " Numero De Alunos: " << numero_de_alunos << endl << endl;

    util::pressionarEnter();

    verAlunos();
}

void Manager::pesquisarAlunos()
{
    cabecalho();

    cout << "   \t\t" << "\t" << "Pesquisar Alunos" << endl << endl;

    cout << "   \t\t" << "1. Pesquisar Nomes" << endl << endl;
    cout << "   \t\t" << "2. Pesquisar Por Idades" << endl << endl;
    cout << "   \t\t" << "3. Pesquisar Por Moradas" << endl << endl;
    cout << "   \t\t" << "4. Pesquisar Por Datas" << endl << endl;
    cout << "   \t\t" << "5. Pesquisar Pelo Numero De BI" << endl << endl;
    cout << "   \t\t" << "0. Voltar" << endl << endl;
    cout << "   \t\t" << "->";
    char opt = _getch();

    while (opt != '1' && opt != '2' && opt != '3' && opt != '4' && opt != '5' && opt != '0' && opt != 8 && opt != 27 )
    {
        opt = _getch();
    }

    switch(opt)
    {
    case '1':
        pesquisarNomes();
        break;

    case '2':
        pesquisarPorIdade();
        break;

    case '3':
        pesquisarMoradas();
        break;

    case '4':
        pesquisarDatas();
        break;

    case '5':
        pesquisarN_BI();
        break;

    case '0':
    case 8:
        verAlunos();
        break;

    case 27:
        paginaInicial();
        break;

    default:
        pesquisarAlunos();
        break;
    }
}

void Manager::pesquisarNomes()
{
    cabecalho();

    cout << "   \t\t" << "\t" << "Pesquisar Alunos Pelo Nome" << endl << endl;

    cout << "   \t\t" << "1. Pesquisar Numa Classe" << endl << endl;
    cout << "   \t\t" << "2. Pesquisar Entre Classes" << endl << endl;
    cout << "   \t\t" << "3. Pesquisar Em Toda A Escola" << endl << endl;
    cout << "   \t\t" << "0. Voltar" << endl << endl;
    cout << "   \t\t" << "->";
    char opt = _getch();

    while (opt != '1' && opt != '2' && opt != '3' && opt != '0' && opt != 8 && opt != 27 )
    {
        opt = _getch();
    }

    switch(opt)
    {
    case '1':
        pesquisarNomesNumaClasse();
        break;

    case '2':
        pesquisarNomesEntreClasses();
        break;

    case '3':
        pesquisarNomesEmTodaAEsola();
        break;

    case '0':
    case 8:
        pesquisarAlunos();
        break;

    case 27:
        paginaInicial();
        break;

    default:
        pesquisarNomes();
        break;
    }
}

void Manager::pesquisarNomesNumaClasse()
{
    cabecalho();

    string ano_escolaridade_, nome_de_pesquisa;

    deque<Estudante> deque_alunos;
    deque<Estudante> alunos_encontrados;

    int registo_de_ano_escolaridade;

    cout << "   \t\t" << "\t" << "Pesquisar Alunos Pelo Nome Numa Classe" << endl << endl;

    cout << "   \t\t" << "Ano De Escolaridade (1 -12): ";
    ano_escolaridade_ = util::inputRestriction(2, 12, true, this, &Manager::pesquisarNomes, registo_de_ano_escolaridade);

    string file_path = Diretorios::ESCOLAS + escola.obterNomeDaEscola() + "/Classes/" + ano_escolaridade_ + "/";

    cout << endl << endl << "   \t\t" << "Pesquisar Nome: ";
    util::validarInput(nome_de_pesquisa, this, &Manager::pesquisarNomes);

    extrairAlunosDeUmaClasse(deque_alunos, file_path, ano_escolaridade_);

    encontrarAlunosPeloNome(deque_alunos, alunos_encontrados, nome_de_pesquisa);

    if (alunos_encontrados.empty())
    {
        cout << endl << endl << "Sem Resusltados" << endl;

        util::pressionarEnter();

        pesquisarNomes();
    }
    else
    {
        sort(alunos_encontrados.begin(), alunos_encontrados.end(), ordenarAlunosPorNomeCrescente);

        cout << endl << endl;
        displayAlunos(alunos_encontrados, true);

        pesquisarNomes();
    }
}

void Manager::pesquisarNomesEntreClasses()
{
    cabecalho();

    cout << "   \t\t" << "\t" << "Pesquisar Alunos Alunos Pelo Nome Entre Classes" << endl << endl;

    deque<Estudante> deque_alunos;
    deque<Estudante> alunos_encontrados;

    string nome_de_pesquisa;

    int registo_de_ano_escolaridadeA, registo_de_ano_escolaridadeB;

    inserirAnoDeEscolaridadeEntreClasses(registo_de_ano_escolaridadeA, registo_de_ano_escolaridadeB,
                                         &Manager::pesquisarNomes);

    cout << endl << endl << "   \t\t" << "Pesquisar Nome: ";
    util::validarInput(nome_de_pesquisa, this, &Manager::pesquisarNomes);

    string file_path = Diretorios::ESCOLAS + escola.obterNomeDaEscola() + "/Classes/";

    for(int i = registo_de_ano_escolaridadeA; i <= registo_de_ano_escolaridadeB; ++i)
    {
        string classe_ = to_string(i);

        extrairAlunosDeUmaClasse(deque_alunos, file_path  + classe_ + "/", classe_);
    }

    encontrarAlunosPeloNome(deque_alunos, alunos_encontrados, nome_de_pesquisa);

    if (alunos_encontrados.empty())
    {
        cout << endl << endl << "Sem Resusltados" << endl;

        util::pressionarEnter();

        pesquisarNomes();
    }
    else
    {
        sort(alunos_encontrados.begin(), alunos_encontrados.end(), ordenarAlunosPorNomeCrescente);

        cout << endl;
        displayAlunos(alunos_encontrados, true);

        pesquisarNomes();
    }
}

void Manager::pesquisarNomesEmTodaAEsola()
{
    cabecalho();

    deque<Estudante> deque_alunos;
    deque<Estudante> alunos_encontrados;

    string nome_de_pesquisa;

    cout << "   \t\t" << "\t" << "Pesquisar Alunos Alunos Pelo Nome Em Toda A Escola" << endl << endl;

    cout << "   \t\t" << "Pesquisar Nome: ";
    util::validarInput(nome_de_pesquisa, this, &Manager::pesquisarNomes);

    extrairAlunosDeTodaAEscola(deque_alunos);

    encontrarAlunosPeloNome(deque_alunos, alunos_encontrados, nome_de_pesquisa);

    if (alunos_encontrados.empty())
    {
        cout << endl << endl << "Sem Resusltados" << endl;

        util::pressionarEnter();

        pesquisarNomes();
    }
    else
    {
        sort(alunos_encontrados.begin(), alunos_encontrados.end(), ordenarAlunosPorNomeCrescente);

        cout << endl;
        displayAlunos(alunos_encontrados, true);

        pesquisarNomes();
    }
}

void Manager::pesquisarPorIdade()
{
    cabecalho();

    cout << "   \t\t" << "\t" << "Pesquisar Alunos Por Idade" << endl << endl;

    cout << "   \t\t" << "1. Pesquisar Numa Classe" << endl << endl;
    cout << "   \t\t" << "2. Pesquisar Entre Classes" << endl << endl;
    cout << "   \t\t" << "3. Pesquisar Em Toda A Escola" << endl << endl;
    cout << "   \t\t" << "0. Voltar" << endl << endl;
    cout << "   \t\t" << "->";
    char opt = _getch();

    while (opt != '1' && opt != '2' && opt != '3' && opt != '0' && opt != 8 && opt != 27 )
    {
        opt = _getch();
    }

    switch(opt)
    {
    case '1':
        pesquisarIdadeNumaClasse();
        break;

    case '2':
        pesquisarIdadeEntreClasses();
        break;

    case '3':
        pesquisarIdadeEmTodaAEsola();
        break;

    case '0':
    case 8:
        pesquisarAlunos();
        break;

    case 27:
        paginaInicial();
        break;

    default:
        pesquisarPorIdade();
        break;
    }
}

void Manager::pesquisarIdadeNumaClasse()
{
    cabecalho();

    string ano_escolaridade_;
    int idade_do_aluno;

    deque<Estudante> deque_alunos;
    deque<Estudante> alunos_encontrados;

    int registo_de_ano_escolaridade;

    cout << "   \t\t" << "\t" << "Pesquisar Alunos Pela Idade Numa Classe" << endl << endl;

    cout << "   \t\t" << "Ano De Escolaridade (1 -12): ";
    ano_escolaridade_ = util::inputRestriction(2, 12, true, this, &Manager::pesquisarPorIdade, registo_de_ano_escolaridade);

    string file_path = Diretorios::ESCOLAS + escola.obterNomeDaEscola() + "/Classes/" + ano_escolaridade_ + "/";

    cout << endl << endl << "   \t\t" << "Inserir Idade Para Pesquisa: ";
    util::inputRestriction(2, 25, true, this, &Manager::pesquisarPorIdade, idade_do_aluno);

    extrairAlunosDeUmaClasse(deque_alunos, file_path, ano_escolaridade_);

    encontrarAlunosPelaIdade(deque_alunos, alunos_encontrados, idade_do_aluno);

    if (alunos_encontrados.empty())
    {
        cout << endl << endl << "Sem Resusltados" << endl;

        util::pressionarEnter();

        pesquisarPorIdade();
    }
    else
    {
        sort(alunos_encontrados.begin(), alunos_encontrados.end(), ordenarAlunosPorNomeCrescente);

        cout << endl << endl;
        displayAlunos(alunos_encontrados, true);

        pesquisarPorIdade();
    }
}

void Manager::pesquisarIdadeEntreClasses()
{
    cabecalho();

    cout << "   \t\t" << "\t" << "Pesquisar Alunos Pela Idade Entre Classes" << endl << endl;

    deque<Estudante> deque_alunos;
    deque<Estudante> alunos_encontrados;

    int idade_do_aluno;

    int registo_de_ano_escolaridadeA, registo_de_ano_escolaridadeB;

    inserirAnoDeEscolaridadeEntreClasses(registo_de_ano_escolaridadeA, registo_de_ano_escolaridadeB, &Manager::pesquisarPorIdade);

    cout << endl << endl << "   \t\t" << "Inserir Idade Para Pesquisa: ";
    util::inputRestriction(2, 25, true, this, &Manager::pesquisarPorIdade, idade_do_aluno );

    string file_path = Diretorios::ESCOLAS + escola.obterNomeDaEscola() + "/Classes/";

    for(int i = registo_de_ano_escolaridadeA; i <= registo_de_ano_escolaridadeB; ++i)
    {
        string classe_ = to_string(i);

        extrairAlunosDeUmaClasse(deque_alunos, file_path + classe_ + "/", classe_);
    }

    encontrarAlunosPelaIdade(deque_alunos, alunos_encontrados, idade_do_aluno);

    if (alunos_encontrados.empty())
    {
        cout << endl << endl << "Sem Resusltados" << endl;

        util::pressionarEnter();

        pesquisarPorIdade();
    }
    else
    {
        sort(alunos_encontrados.begin(), alunos_encontrados.end(), ordenarAlunosPorNomeCrescente);

        cout << endl << endl;
        displayAlunos(alunos_encontrados, true);

        pesquisarPorIdade();
    }
}

void Manager::pesquisarIdadeEmTodaAEsola()
{
    cabecalho();

    deque<Estudante> deque_alunos;
    deque<Estudante> alunos_encontrados;

    int idade_do_aluno;

    cout << "   \t\t" << "\t" << "Pesquisar Alunos Em Toda A Escola" << endl << endl;

    cout << "   \t\t" << "Inserir Idade Para Pesquisa: ";
    util::inputRestriction(2, 25, true, this, &Manager::pesquisarPorIdade, idade_do_aluno );

    extrairAlunosDeTodaAEscola(deque_alunos);

    encontrarAlunosPelaIdade(deque_alunos, alunos_encontrados, idade_do_aluno);

    if (alunos_encontrados.empty())
    {
        cout << endl << endl << "Sem Resusltados" << endl;

        util::pressionarEnter();

        pesquisarPorIdade();
    }
    else
    {
        sort(alunos_encontrados.begin(), alunos_encontrados.end(), ordenarAlunosPorNomeCrescente);

        cout << endl << endl;
        displayAlunos(alunos_encontrados, true);

        pesquisarPorIdade();
    }
}

void Manager::pesquisarMoradas()
{
    cabecalho();

    cout << "   \t\t" << "\t" << "Pesquisar Alunos Pela Morada" << endl << endl;

    cout << "   \t\t" << "1. Pesquisar Numa Classe" << endl << endl;
    cout << "   \t\t" << "2. Pesquisar Entre Classes" << endl << endl;
    cout << "   \t\t" << "3. Pesquisar Em Toda A Escola" << endl << endl;
    cout << "   \t\t" << "0. Voltar" << endl << endl;
    cout << "   \t\t" << "->";
    char opt = _getch();

    while (opt != '1' && opt != '2' && opt != '3' && opt != '0' && opt != 8 && opt != 27 )
    {
        opt = _getch();
    }

    switch(opt)
    {
    case '1':
        pesquisarMoradasNumaClasse();
        break;

    case '2':
        pesquisarMoradasEntreClasses();
        break;

    case '3':
        pesquisarMoradasEmTodaAEsola();
        break;

    case '0':
    case 8:
        pesquisarAlunos();
        break;

    case 27:
        paginaInicial();
        break;

    default:
        pesquisarMoradas();
        break;
    }
}

void Manager::pesquisarMoradasNumaClasse()
{
    cabecalho();

    string  morada_de_pesquisa;

    deque<Estudante> deque_alunos;
    deque<Estudante> alunos_encontrados;

    int ano_escolaridade_;

    cout << "   \t\t" << "\t" << "Pesquisar Alunos Numa Classe Pela Morada" << endl << endl;

    cout << "   \t\t" << "Ano De Escolaridade (1 -12): ";
    util::inputRestriction(2, 12, true, this, &Manager::pesquisarMoradas, ano_escolaridade_);

    string file_path = Diretorios::ESCOLAS + escola.obterNomeDaEscola() + "/Classes/" + to_string(ano_escolaridade_) + "/";

    cout << endl << endl << "   \t\t" << "Pesquisar Morada: ";
    util::validarInput(morada_de_pesquisa, this, &Manager::pesquisarMoradas);

    extrairAlunosDeUmaClasse(deque_alunos, file_path, to_string(ano_escolaridade_));

    encontrarAlunosPelaMorada(deque_alunos, alunos_encontrados, morada_de_pesquisa);

    if (alunos_encontrados.empty())
    {
        cout << endl << endl << "Sem Resusltados" << endl;

        util::pressionarEnter();

        pesquisarMoradas();
    }
    else
    {
        sort(alunos_encontrados.begin(), alunos_encontrados.end(), ordenarAlunosPorNomeCrescente);

        cout << endl << endl;
        displayAlunos(alunos_encontrados, true);

        pesquisarMoradas();
    }
}

void Manager::pesquisarMoradasEntreClasses()
{
    cabecalho();

    cout << "   \t\t" << "\t" << "Pesquisar Alunos Entre Classes Pela Morada" << endl << endl;

    deque<Estudante> deque_alunos;
    deque<Estudante> alunos_encontrados;

    string morada_de_pesquisa;

    int registo_de_ano_escolaridadeA, registo_de_ano_escolaridadeB;

    inserirAnoDeEscolaridadeEntreClasses(registo_de_ano_escolaridadeA, registo_de_ano_escolaridadeB, &Manager::pesquisarMoradas);

    cout << endl << endl << "   \t\t" << "Pesquisar Morada: ";
    util::validarInput(morada_de_pesquisa, this, &Manager::pesquisarMoradas);

    string file_path = Diretorios::ESCOLAS + escola.obterNomeDaEscola() + "/Classes/";

    for(int i = registo_de_ano_escolaridadeA; i <= registo_de_ano_escolaridadeB; ++i)
    {
        string classe_ = to_string(i);

        extrairAlunosDeUmaClasse(deque_alunos, file_path + classe_ + "/", classe_);
    }

    encontrarAlunosPelaMorada(deque_alunos, alunos_encontrados, morada_de_pesquisa);

    if (alunos_encontrados.empty())
    {
        cout << endl << endl << "Sem Resusltados" << endl;

        util::pressionarEnter();

        pesquisarMoradas();
    }
    else
    {
        sort(alunos_encontrados.begin(), alunos_encontrados.end(), ordenarAlunosPorNomeCrescente);

        cout << endl;
        displayAlunos(alunos_encontrados, true);

        pesquisarMoradas();
    }
}

void Manager::pesquisarMoradasEmTodaAEsola()
{
    cabecalho();

    deque<Estudante> deque_alunos;
    deque<Estudante> alunos_encontrados;

    string morada_de_pesquisa;

    cout << "   \t\t" << "\t" << "Pesquisar Alunos Em Toda A Escola Pela Morada" << endl << endl;

    cout << "   \t\t" << "Pesquisar Morada: ";
    util::validarInput(morada_de_pesquisa, this, &Manager::pesquisarMoradas);

    extrairAlunosDeTodaAEscola(deque_alunos);

    encontrarAlunosPelaMorada(deque_alunos, alunos_encontrados, morada_de_pesquisa);

    if (alunos_encontrados.empty())
    {
        cout << endl << endl << "Sem Resusltados" << endl;

        util::pressionarEnter();

        pesquisarMoradas();
    }
    else
    {
        sort(alunos_encontrados.begin(), alunos_encontrados.end(), ordenarAlunosPorNomeCrescente);

        cout << endl;
        displayAlunos(alunos_encontrados, true);

        pesquisarMoradas();
    }
}

void Manager::pesquisarDatas()
{
    cabecalho();

    cout << "   \t\t" << "\t" << "Pesquisar Alunos Por Datas" << endl << endl;

    cout << "   \t\t" << "1. Pesquisar Data De Nascimento" << endl << endl;
    cout << "   \t\t" << "2. Pesquisar Ano De Nascimento" << endl << endl;
    cout << "   \t\t" << "3. Pesquisar Mes De Nascimento" << endl << endl;
    cout << "   \t\t" << "4. Pesquisar Mes/Ano De Nascimento" << endl << endl;
    cout << "   \t\t" << "0. Voltar" << endl << endl;
    cout << "   \t\t" << "->";
    char opt = _getch();

    while (opt != '1' && opt != '2' && opt != '3' && opt != '4' && opt != '0' && opt != 8 && opt != 27 )
    {
        opt = _getch();
    }

    switch(opt)
    {
    case '1':
        pesquisarDataNascimento();
        break;

    case '2':
        pesquisarAnoNascimento();
        break;

    case '3':
        pesquisarMesNascimento();
        break;

    case '4':
        pesquisar_Mes_Ano_Nascimento();
        break;

    case '0':
    case 8:
        pesquisarAlunos();
        break;

    case 27:
        paginaInicial();
        break;
    }
}

void Manager::pesquisarDataNascimento()
{
    cabecalho();

    cout << "   \t\t" << "\t" << "Pesquisar Data De Nascimento" << endl << endl;

    cout << "   \t\t" << "1. Pesquisar Numa Classe" << endl << endl;
    cout << "   \t\t" << "2. Pesquisar Entre Classes" << endl << endl;
    cout << "   \t\t" << "3. Pesquisar Em Toda A Escola" << endl << endl;
    cout << "   \t\t" << "0. Voltar" << endl << endl;
    cout << "   \t\t" << "->";
    char opt = _getch();

    while (opt != '1' && opt != '2' && opt != '3' && opt != '0' && opt != 8 && opt != 27 )
    {
        opt = _getch();
    }

    switch(opt)
    {
    case '1':
        pesquisarDataNascimentoNumaClasse();
        break;

    case '2':
        pesquisarDataNascimentoEntreClasses();
        break;

    case '3':
        pesquisarDataNascimentoEmTodaAEscola();
        break;

    case '0':
    case 8:
        pesquisarDatas();
        break;

    case 27:
        paginaInicial();
        break;
    }
}

void Manager::pesquisarDataNascimentoNumaClasse()
{
    cabecalho();

    string ano_escolaridade_;

    deque<Estudante> deque_alunos;
    deque<Estudante> alunos_encontrados;

    int registo_de_ano_escolaridade;

    cout << "   \t\t" << "Pesquisar Alunos Pela Data De Nascimento Numa Classe" << endl << endl;

    cout << "   \t\t" << "Ano De Escolaridade (1 -12): ";
    ano_escolaridade_ = util::inputRestriction(2, 12, true, this, &Manager::pesquisarDataNascimento, registo_de_ano_escolaridade);

    time_t now = time(nullptr);
    tm* agora = localtime(&now);

    int presente_ano = 1900 + agora->tm_year;

    int ano_nascimento_, mes_nascimento_, dia_nascimento_;

    cout << endl << endl << "   \t\t" << "  Data De Nascimento: " << endl;
    cout << "   \t\t" << "   Ano: ";
    util::inputRestriction(4, presente_ano, false, this, &Manager::pesquisarDataNascimento, ano_nascimento_);

    cout << endl << "   \t\t" << "   Mes: ";
    util::inputRestriction(2, 12, true, this, &Manager::pesquisarDataNascimento, mes_nascimento_);

    cout << endl << "   \t\t" << "   Dia: ";
    util::inputRestriction(2, 31, true, this, &Manager::pesquisarDataNascimento, dia_nascimento_);

    while (! util::dataValido(ano_nascimento_, mes_nascimento_, dia_nascimento_))
    {
        cout << endl << endl << "   \t\t" <<  "  Data Invalido" << endl;
        cout << "   \t\t" << "   Ano: ";
        util::inputRestriction(4, presente_ano, false, this, &Manager::pesquisarDataNascimento, ano_nascimento_);

        cout << endl << "   \t\t" << "   Mes: ";
        util::inputRestriction(2, 12, true, this, &Manager::pesquisarDataNascimento, mes_nascimento_);

        cout << endl << "   \t\t" << "   Dia: ";
        util::inputRestriction(2, 31, true, this, &Manager::pesquisarDataNascimento, dia_nascimento_);
    }

    string file_path = Diretorios::ESCOLAS + escola.obterNomeDaEscola() + "/Classes/" + ano_escolaridade_ + "/";

    string temp_dia = to_string(dia_nascimento_);
    string temp_mes = to_string(mes_nascimento_);

    if (temp_dia.size() == 1)
        temp_dia = "0" + temp_dia;

    if (temp_mes.size() == 1)
        temp_mes =  "0" + temp_mes;

    string data_nascimento = temp_dia + " - " + temp_mes + " - " + to_string(ano_nascimento_);

    extrairAlunosDeUmaClasse(deque_alunos, file_path, ano_escolaridade_);

    encontrarAlunosPelaDataNascimento(deque_alunos, alunos_encontrados, data_nascimento);

    if (alunos_encontrados.empty())
    {
        cout << endl << endl << "Sem Resusltados" << endl;

        util::pressionarEnter();

        pesquisarDataNascimento();
    }
    else
    {
        sort(alunos_encontrados.begin(), alunos_encontrados.end(), ordenarAlunosPorNomeCrescente);

        cout << endl << endl;
        displayAlunos(alunos_encontrados, true);

        pesquisarDataNascimento();
    }
}

void Manager::pesquisarDataNascimentoEntreClasses()
{
    cabecalho();

    string ano_escolaridade_;

    deque<Estudante> deque_alunos;
    deque<Estudante> alunos_encontrados;

    cout << "   \t\t" << "Pesquisar Alunos Pela Data De Nascimento Entre Classes" << endl << endl;

    int registo_de_ano_escolaridadeA, registo_de_ano_escolaridadeB;

    inserirAnoDeEscolaridadeEntreClasses(registo_de_ano_escolaridadeA, registo_de_ano_escolaridadeB, &Manager::pesquisarDataNascimento);

    time_t now = time(nullptr);
    tm* agora = localtime(&now);

    int presente_ano = 1900 + agora->tm_year;

    int ano_nascimento_, mes_nascimento_, dia_nascimento_;

    cout << endl << endl << "   \t\t" << "  Data De Nascimento: " << endl;
    cout << "   \t\t" << "   Ano: ";
    util::inputRestriction(4, presente_ano, false, this, &Manager::pesquisarDataNascimento, ano_nascimento_);

    cout << endl << "   \t\t" << "   Mes: ";
    util::inputRestriction(2, 12, true, this, &Manager::pesquisarDataNascimento, mes_nascimento_);

    cout << endl << "   \t\t" << "   Dia: ";
    util::inputRestriction(2, 31, true, this, &Manager::pesquisarDataNascimento, dia_nascimento_);

    while (! util::dataValido(ano_nascimento_, mes_nascimento_, dia_nascimento_))
    {
        cout << endl << endl << "   \t\t" <<  "  Data Invalido" << endl;
        cout << "   \t\t" << "   Ano: ";
        util::inputRestriction(4, presente_ano, false, this, &Manager::pesquisarDataNascimento, ano_nascimento_);

        cout << endl << "   \t\t" << "   Mes: ";
        util::inputRestriction(2, 12, true, this, &Manager::pesquisarDataNascimento, mes_nascimento_);

        cout << endl << "   \t\t" << "   Dia: ";
        util::inputRestriction(2, 31, true, this, &Manager::pesquisarDataNascimento, dia_nascimento_);
    }

    string file_path = Diretorios::ESCOLAS + escola.obterNomeDaEscola() + "/Classes/";

    string temp_dia = to_string(dia_nascimento_);
    string temp_mes = to_string(mes_nascimento_);

    if (temp_dia.size() == 1)
        temp_dia = "0" + temp_dia;

    if (temp_mes.size() == 1)
        temp_mes =  "0" + temp_mes;

    string data_nascimento = temp_dia + " - " + temp_mes + " - " + to_string(ano_nascimento_);

    for(int i = registo_de_ano_escolaridadeA; i <= registo_de_ano_escolaridadeB; ++i)
    {
        string classe_ = to_string(i);

        extrairAlunosDeUmaClasse(deque_alunos, file_path + classe_, classe_);
    }

    encontrarAlunosPelaDataNascimento(deque_alunos, alunos_encontrados, data_nascimento);

    if (alunos_encontrados.empty())
    {
        cout << endl << endl << "Sem Resusltados" << endl;

        util::pressionarEnter();

        pesquisarDataNascimento();
    }
    else
    {
        sort(alunos_encontrados.begin(), alunos_encontrados.end(), ordenarAlunosPorNomeCrescente);

        cout << endl << endl;
        displayAlunos(alunos_encontrados, true);

        pesquisarDataNascimento();
    }
}

void Manager::pesquisarDataNascimentoEmTodaAEscola()
{
    cabecalho();

    string ano_escolaridade_;

    deque<Estudante> deque_alunos;
    deque<Estudante> alunos_encontrados;

    cout << "   \t\t" << "Pesquisar Alunos Pela Data De Nascimento Em Toda A Escola" << endl << endl;

    time_t now = time(nullptr);
    tm* agora = localtime(&now);

    int presente_ano = 1900 + agora->tm_year;

    int ano_nascimento_, mes_nascimento_, dia_nascimento_;

    cout << "   \t\t" << "  Data De Nascimento: " << endl;
    cout << "   \t\t" << "   Ano: ";
    util::inputRestriction(4, presente_ano, false, this, &Manager::pesquisarDataNascimento, ano_nascimento_);

    cout << endl << "   \t\t" << "   Mes: ";
    util::inputRestriction(2, 12, true, this, &Manager::pesquisarDataNascimento, mes_nascimento_);

    cout << endl << "   \t\t" << "   Dia: ";
    util::inputRestriction(2, 31, true, this, &Manager::pesquisarDataNascimento, dia_nascimento_);

    while (! util::dataValido(ano_nascimento_, mes_nascimento_, dia_nascimento_))
    {
        cout << endl << endl << "   \t\t" <<  "  Data Invalido" << endl;
        cout << "   \t\t" << "   Ano: ";
        util::inputRestriction(4, presente_ano, false, this, &Manager::pesquisarDataNascimento, ano_nascimento_);

        cout << endl << "   \t\t" << "   Mes: ";
        util::inputRestriction(2, 12, true, this, &Manager::pesquisarDataNascimento, mes_nascimento_);

        cout << endl << "   \t\t" << "   Dia: ";
        util::inputRestriction(2, 31, true, this, &Manager::pesquisarDataNascimento, dia_nascimento_);
    }

    string path = Diretorios::ESCOLAS + escola.obterNomeDaEscola() + "/Classes/" + ano_escolaridade_ + "/";

    string temp_dia = to_string(dia_nascimento_);
    string temp_mes = to_string(mes_nascimento_);

    if (temp_dia.size() == 1)
        temp_dia = "0" + temp_dia;

    if (temp_mes.size() == 1)
        temp_mes =  "0" + temp_mes;

    string data_nascimento = temp_dia + " - " + temp_mes + " - " + to_string(ano_nascimento_);

    extrairAlunosDeTodaAEscola(deque_alunos);

    encontrarAlunosPelaDataNascimento(deque_alunos, alunos_encontrados, data_nascimento);

    if (alunos_encontrados.empty())
    {
        cout << endl << endl << "Sem Resusltados" << endl;

        util::pressionarEnter();

        pesquisarDataNascimento();
    }
    else
    {
        sort(alunos_encontrados.begin(), alunos_encontrados.end(), ordenarAlunosPorNomeCrescente);

        cout << endl << endl;
        displayAlunos(alunos_encontrados, true);

        pesquisarDataNascimento();
    }

}

void Manager::pesquisarAnoNascimento()
{
    cabecalho();

    cout << "   \t\t" << "\t" << "Pesquisar Ano De Nascimento" << endl << endl;

    cout << "   \t\t" << "1. Pesquisar Numa Classe" << endl << endl;
    cout << "   \t\t" << "2. Pesquisar Entre Classes" << endl << endl;
    cout << "   \t\t" << "3. Pesquisar Em Toda A Escola" << endl << endl;
    cout << "   \t\t" << "0. Voltar" << endl << endl;
    cout << "   \t\t" << "->";
    char opt = _getch();

    while (opt != '1' && opt != '2' && opt != '3' && opt != '0' && opt != 8 && opt != 27 )
    {
        opt = _getch();
    }

    switch(opt)
    {
    case '1':
        pesquisarAnoNascimentoNumaClasse();
        break;

    case '2':
        pesquisarAnoNascimentoEntreClasses();
        break;

    case '3':
        pesquisarAnoNascimentoEmTodaAEscola();
        break;

    case '0':
    case 8:
        pesquisarDatas();
        break;

    case 27:
        paginaInicial();
        break;
    }
}

void Manager::pesquisarAnoNascimentoNumaClasse()
{
    cabecalho();

    deque<Estudante> deque_alunos;
    deque<Estudante> alunos_encontrados;

    int ano_escolaridade_;

    cout << "   \t\t" << "Pesquisar Alunos Pelo Ano De Nascimento Numa Classe" << endl << endl;

    cout << "   \t\t" << "Ano De Escolaridade (1 -12): ";
    util::inputRestriction(2, 12, true, this, &Manager::pesquisarAnoNascimento, ano_escolaridade_);

    time_t now = time(nullptr);
    tm* agora = localtime(&now);

    int presente_ano = 1900 + agora->tm_year;

    int ano_nascimento;

    cout << endl << endl << "   \t\t" << "  Data De Nascimento: " << endl;
    cout << endl << "   \t\t" << "   Mes: ";
    util::inputRestriction(4, presente_ano, false, this, &Manager::pesquisarAnoNascimento, ano_nascimento);

    string file_path = Diretorios::ESCOLAS + escola.obterNomeDaEscola() + "/Classes/" + to_string(ano_escolaridade_) + "/";

    extrairAlunosDeUmaClasse(deque_alunos, file_path, to_string(ano_escolaridade_));

    encontrarAlunosPeloAnoNascimento(deque_alunos, alunos_encontrados, ano_nascimento);

    if (alunos_encontrados.empty())
    {
        cout << endl << endl << "Sem Resusltados" << endl;

        util::pressionarEnter();

        pesquisarAnoNascimento();
    }
    else
    {
        sort(alunos_encontrados.begin(), alunos_encontrados.end(), ordenarAlunosPorNomeCrescente);

        cout << endl << endl;
        displayAlunos(alunos_encontrados, true);

        pesquisarAnoNascimento();
    }
}

void Manager::pesquisarAnoNascimentoEntreClasses()
{
    cabecalho();

    string ano_escolaridade_;

    deque<Estudante> deque_alunos;
    deque<Estudante> alunos_encontrados;

    cout << "   \t\t" << "Pesquisar Alunos Pelo Ano De Nascimento Entre Classes" << endl << endl;

    int registo_de_ano_escolaridadeA, registo_de_ano_escolaridadeB;

    inserirAnoDeEscolaridadeEntreClasses(registo_de_ano_escolaridadeA, registo_de_ano_escolaridadeB, &Manager::pesquisarAnoNascimento);

    time_t now = time(nullptr);
    tm* agora = localtime(&now);

    int presente_ano = 1900 + agora->tm_year;

    int ano_nascimento;

    cout << endl << endl << "   \t\t" << "  Data De Nascimento: " << endl;
    cout << "   \t\t" << "   Ano: ";
    util::inputRestriction(4, presente_ano, false, this, &Manager::pesquisarAnoNascimento, ano_nascimento);

    string file_path = Diretorios::ESCOLAS + escola.obterNomeDaEscola() + "/Classes/";

    for(int i = registo_de_ano_escolaridadeA; i <= registo_de_ano_escolaridadeB; ++i)
    {
        string classe_ = to_string(i);

        extrairAlunosDeUmaClasse(deque_alunos, file_path + classe_ + "/", classe_);
    }

    encontrarAlunosPeloAnoNascimento(deque_alunos, alunos_encontrados, ano_nascimento);

    if (alunos_encontrados.empty())
    {
        cout << endl << endl << "Sem Resusltados" << endl;

        util::pressionarEnter();

        pesquisarAnoNascimento();
    }
    else
    {
        sort(alunos_encontrados.begin(), alunos_encontrados.end(), ordenarAlunosPorNomeCrescente);

        cout << endl << endl;
        displayAlunos(alunos_encontrados, true);

        pesquisarAnoNascimento();
    }
}

void Manager::pesquisarAnoNascimentoEmTodaAEscola()
{
    cabecalho();

    string ano_escolaridade_;

    deque<Estudante> deque_alunos;
    deque<Estudante> alunos_encontrados;

    cout << "   \t\t" << "Pesquisar Alunos Pelo Ano De Nascimento Em Toda A Escola" << endl << endl;

    time_t now = time(nullptr);
    tm* agora = localtime(&now);

    int presente_ano = 1900 + agora->tm_year;

    int ano_nascimento;

    cout << "   \t\t" << "  Data De Nascimento: " << endl;
    cout << "   \t\t" << "   Ano: ";
    util::inputRestriction(4, presente_ano, false, this, &Manager::pesquisarAnoNascimento, ano_nascimento);

    extrairAlunosDeTodaAEscola(deque_alunos);

    encontrarAlunosPeloAnoNascimento(deque_alunos, alunos_encontrados, ano_nascimento);

    if (alunos_encontrados.empty())
    {
        cout << endl << endl << "Sem Resusltados" << endl;

        util::pressionarEnter();

        pesquisarAnoNascimento();
    }
    else
    {
        sort(alunos_encontrados.begin(), alunos_encontrados.end(), ordenarAlunosPorNomeCrescente);

        cout << endl << endl;
        displayAlunos(alunos_encontrados, true);

        pesquisarAnoNascimento();
    }
}

void Manager::pesquisarMesNascimento()
{
    cabecalho();

    cout << "   \t\t" << "\t" << "Pesquisar Mes De Nascimento" << endl << endl;

    cout << "   \t\t" << "1. Pesquisar Numa Classe" << endl << endl;
    cout << "   \t\t" << "2. Pesquisar Entre Classes" << endl << endl;
    cout << "   \t\t" << "3. Pesquisar Em Toda A Escola" << endl << endl;
    cout << "   \t\t" << "0. Voltar" << endl << endl;
    cout << "   \t\t" << "->";
    char opt = _getch();

    while (opt != '1' && opt != '2' && opt != '3' && opt != '0' && opt != 8 && opt != 27 )
    {
        opt = _getch();
    }

    switch(opt)
    {
    case '1':
        pesquisarMesNascimentoNumaClasse();
        break;

    case '2':
        pesquisarMesNascimentoEntreClasses();
        break;

    case '3':
        pesquisarMesNascimentoEmTodaAEscola();
        break;

    case '0':
    case 8:
        pesquisarDatas();
        break;

    case 27:
        paginaInicial();
        break;
    }
}

void Manager::pesquisarMesNascimentoNumaClasse()
{
    cabecalho();

    string ano_escolaridade_;

    deque<Estudante> deque_alunos;
    deque<Estudante> alunos_encontrados;

    int registo_de_ano_escolaridade;

    cout << "   \t\t" << "Pesquisar Alunos Pelo Mes De Nascimento Numa Classe" << endl << endl;

    cout << "   \t\t" << "Ano De Escolaridade (1 -12): ";
    ano_escolaridade_ = util::inputRestriction(2, 12, true, this, &Manager::pesquisarMesNascimento, registo_de_ano_escolaridade);

    int mes_nascimento;

    cout << endl << endl << "   \t\t" << "  Data De Nascimento: " << endl;
    cout << endl << "   \t\t" << "   Mes: ";
    util::inputRestriction(2, 12, true, this, &Manager::pesquisarMesNascimento, mes_nascimento);

    string file_path = Diretorios::ESCOLAS + escola.obterNomeDaEscola() + "/Classes/" + ano_escolaridade_ + "/";

    extrairAlunosDeUmaClasse(deque_alunos, file_path, ano_escolaridade_);

    encontrarAlunosPeloMesNascimento(deque_alunos, alunos_encontrados, mes_nascimento);

    if (alunos_encontrados.empty())
    {
        cout << endl << endl << "Sem Resusltados" << endl;

        util::pressionarEnter();

        pesquisarMesNascimento();
    }
    else
    {
        sort(alunos_encontrados.begin(), alunos_encontrados.end(), ordenarAlunosPorNomeCrescente);

        cout << endl << endl;
        displayAlunos(alunos_encontrados, true);

        pesquisarMesNascimento();
    }
}

void Manager::pesquisarMesNascimentoEntreClasses()
{
    cabecalho();

    string ano_escolaridade_;

    deque<Estudante> deque_alunos;
    deque<Estudante> alunos_encontrados;

    cout << "   \t\t" << "Pesquisar Alunos Pelo Mes De Nascimento Entre Classes" << endl << endl;

    int registo_de_ano_escolaridadeA, registo_de_ano_escolaridadeB;

    inserirAnoDeEscolaridadeEntreClasses(registo_de_ano_escolaridadeA, registo_de_ano_escolaridadeB, &Manager::pesquisarMesNascimento);

    int  mes_nascimento;

    cout << endl << endl << "   \t\t" << "  Data De Nascimento: " << endl;
    cout << endl << "   \t\t" << "   Mes: ";
    util::inputRestriction(2, 12, true, this, &Manager::pesquisarMesNascimento, mes_nascimento);

    string file_path = Diretorios::ESCOLAS + escola.obterNomeDaEscola() + "/Classes/";

    for(int i = registo_de_ano_escolaridadeA; i <= registo_de_ano_escolaridadeB; ++i)
    {
        string classe_ = to_string(i);

        extrairAlunosDeUmaClasse(deque_alunos, file_path + classe_ + "/", classe_);
    }

    encontrarAlunosPeloMesNascimento(deque_alunos, alunos_encontrados, mes_nascimento);

    if (alunos_encontrados.empty())
    {
        cout << endl << endl << "Sem Resusltados" << endl;

        util::pressionarEnter();

        pesquisarMesNascimento();
    }
    else
    {
        sort(alunos_encontrados.begin(), alunos_encontrados.end(), ordenarAlunosPorNomeCrescente);

        cout << endl << endl;
        displayAlunos(alunos_encontrados, true);

        pesquisarMesNascimento();
    }
}

void Manager::pesquisarMesNascimentoEmTodaAEscola()
{
    cabecalho();

    string ano_escolaridade_;

    deque<Estudante> deque_alunos;
    deque<Estudante> alunos_encontrados;

    cout << "   \t\t" << "Pesquisar Alunos Pelo Mes De Nascimento Em Toda A Escola" << endl << endl;

    int mes_nascimento;

    cout << "   \t\t" << "  Data De Nascimento: " << endl;
    cout << endl << "   \t\t" << "   Mes: ";
    util::inputRestriction(2, 12, true, this, &Manager::pesquisarMesNascimento, mes_nascimento);

    extrairAlunosDeTodaAEscola(deque_alunos);

    encontrarAlunosPeloMesNascimento(deque_alunos, alunos_encontrados, mes_nascimento);

    if (alunos_encontrados.empty())
    {
        cout << endl << endl << "Sem Resusltados" << endl;

        util::pressionarEnter();

        pesquisarMesNascimento();
    }
    else
    {
        sort(alunos_encontrados.begin(), alunos_encontrados.end(), ordenarAlunosPorNomeCrescente);

        cout << endl << endl;
        displayAlunos(alunos_encontrados, true);

        pesquisarMesNascimento();
    }
}

void Manager::pesquisar_Mes_Ano_Nascimento()
{
    cabecalho();

    cout << "   \t\t" << "\t" << "Pesquisar Mes/Ano De Nascimento" << endl << endl;

    cout << "   \t\t" << "1. Pesquisar Numa Classe" << endl << endl;
    cout << "   \t\t" << "2. Pesquisar Entre Classes" << endl << endl;
    cout << "   \t\t" << "3. Pesquisar Em Toda A Escola" << endl << endl;
    cout << "   \t\t" << "0. Voltar" << endl << endl;
    cout << "   \t\t" << "->";
    char opt = _getch();

    while (opt != '1' && opt != '2' && opt != '3' && opt != '0' && opt != 8 && opt != 27 )
    {
        opt = _getch();
    }

    switch(opt)
    {
    case '1':
        pesquisar_Mes_Ano_NascimentoNumaClasse();
        break;

    case '2':
        pesquisar_Mes_Ano_NascimentoEntreClasses();
        break;

    case '3':
        pesquisar_Mes_Ano_NascimentoEmTodaAEscola();
        break;

    case '0':
    case 8:
        pesquisarDatas();
        break;

    case 27:
        paginaInicial();
        break;
    }
}

void Manager::pesquisar_Mes_Ano_NascimentoNumaClasse()
{
    cabecalho();

    string ano_escolaridade_;

    deque<Estudante> deque_alunos;
    deque<Estudante> alunos_encontrados;

    int registo_de_ano_escolaridade;

    cout << "   \t\t" << "Pesquisar Alunos Pelo Mes/Ano De Nascimento Numa Classe" << endl << endl;

    cout << "   \t\t" << "Ano De Escolaridade (1 -12): ";
    ano_escolaridade_ = util::inputRestriction(2, 12, true, this, &Manager::pesquisar_Mes_Ano_Nascimento, registo_de_ano_escolaridade);

    time_t now = time(nullptr);
    tm* agora = localtime(&now);

    int presente_ano = 1900 + agora->tm_year;

    int ano_nascimento_, mes_nascimento_;

    cout << endl << endl << "   \t\t" << "  Data De Nascimento: " << endl;
    cout << "   \t\t" << "   Ano: ";
    util::inputRestriction(4, presente_ano, false, this, &Manager::pesquisar_Mes_Ano_Nascimento, ano_nascimento_);

    cout << endl << "   \t\t" << "   Mes: ";
    util::inputRestriction(2, 12, true, this, &Manager::pesquisar_Mes_Ano_Nascimento, mes_nascimento_);

    string path = Diretorios::ESCOLAS + escola.obterNomeDaEscola() + "/Classes/" + ano_escolaridade_ + "/";

    string mes_ano_nascimento = to_string(mes_nascimento_) + "-" + to_string(ano_nascimento_);

    extrairAlunosDeUmaClasse(deque_alunos, path, ano_escolaridade_);

    encontrarAlunosPelo_Mes_Ano_Nascimento(deque_alunos, alunos_encontrados, mes_ano_nascimento);

    if (alunos_encontrados.empty())
    {
        cout << endl << endl << "Sem Resusltados" << endl;

        util::pressionarEnter();

        pesquisar_Mes_Ano_Nascimento();
    }
    else
    {
        sort(alunos_encontrados.begin(), alunos_encontrados.end(), ordenarAlunosPorNomeCrescente);

        cout << endl << endl;
        displayAlunos(alunos_encontrados, true);

        pesquisar_Mes_Ano_Nascimento();
    }
}

void Manager::pesquisar_Mes_Ano_NascimentoEntreClasses()
{
    cabecalho();

    string ano_escolaridade_;

    deque<Estudante> deque_alunos;
    deque<Estudante> alunos_encontrados;

    cout << "   \t\t" << "Pesquisar Alunos Pelo Mes/Ano De Nascimento Entre Classes" << endl << endl;

    int registo_de_ano_escolaridadeA, registo_de_ano_escolaridadeB;

    inserirAnoDeEscolaridadeEntreClasses(registo_de_ano_escolaridadeA, registo_de_ano_escolaridadeB, &Manager::pesquisar_Mes_Ano_Nascimento);

    time_t now = time(nullptr);
    tm* agora = localtime(&now);

    int presente_ano = 1900 + agora->tm_year;

    int ano_nascimento_, mes_nascimento_;

    cout << endl << endl << "   \t\t" << "  Data De Nascimento: " << endl;
    cout << "   \t\t" << "   Ano: ";
    util::inputRestriction(4, presente_ano, false, this, &Manager::pesquisar_Mes_Ano_Nascimento, ano_nascimento_);

    cout << endl << "   \t\t" << "   Mes: ";
    util::inputRestriction(2, 12, true, this, &Manager::pesquisar_Mes_Ano_Nascimento, mes_nascimento_);

    string file_path = Diretorios::ESCOLAS + escola.obterNomeDaEscola() + "/Classes/";

    for(int i = registo_de_ano_escolaridadeA; i <= registo_de_ano_escolaridadeB; ++i)
    {
        string classe_ = to_string(i);

        extrairAlunosDeUmaClasse(deque_alunos, file_path + classe_ + "/", classe_);
    }

    string mes_ano_nascimento = to_string(mes_nascimento_) + "-" + to_string(ano_nascimento_);

    encontrarAlunosPelo_Mes_Ano_Nascimento(deque_alunos, alunos_encontrados, mes_ano_nascimento);

    if (alunos_encontrados.empty())
    {
        cout << endl << endl << "Sem Resusltados" << endl;

        util::pressionarEnter();

        pesquisar_Mes_Ano_Nascimento();
    }
    else
    {
        sort(alunos_encontrados.begin(), alunos_encontrados.end(), ordenarAlunosPorNomeCrescente);

        cout << endl << endl;
        displayAlunos(alunos_encontrados, true);

        pesquisar_Mes_Ano_Nascimento();
    }
}

void Manager::pesquisar_Mes_Ano_NascimentoEmTodaAEscola()
{
    cabecalho();

    string ano_escolaridade_;

    deque<Estudante> deque_alunos;
    deque<Estudante> alunos_encontrados;

    cout << "   \t\t" << "Pesquisar Alunos Pelo Mes/Ano De Nascimento Em Toda A Escola" << endl << endl;

    time_t now = time(nullptr);
    tm* agora = localtime(&now);

    int presente_ano = 1900 + agora->tm_year;

    int ano_nascimento_, mes_nascimento_;

    cout << "   \t\t" << "  Data De Nascimento: " << endl;
    cout << "   \t\t" << "   Ano: ";
    util::inputRestriction(4, presente_ano, false, this,  &Manager::pesquisar_Mes_Ano_Nascimento, ano_nascimento_);

    cout << endl << "   \t\t" << "   Mes: ";
    util::inputRestriction(2, 12, true, this, &Manager::pesquisar_Mes_Ano_Nascimento, mes_nascimento_);

    extrairAlunosDeTodaAEscola(deque_alunos);

    string mes_ano_nascimento = to_string(mes_nascimento_) + "-" + to_string(ano_nascimento_);

    encontrarAlunosPelo_Mes_Ano_Nascimento(deque_alunos, alunos_encontrados, mes_ano_nascimento);

    if (alunos_encontrados.empty())
    {
        cout << endl << endl << "Sem Resusltados" << endl;

        util::pressionarEnter();

        pesquisar_Mes_Ano_Nascimento();
    }
    else
    {
        sort(alunos_encontrados.begin(), alunos_encontrados.end(), ordenarAlunosPorNomeCrescente);

        cout << endl << endl;
        displayAlunos(alunos_encontrados, true);

        pesquisar_Mes_Ano_Nascimento();
    }
}

void Manager::pesquisarN_BI()
{
    cabecalho();

    deque<Estudante> deque_alunos;
    deque<Estudante> alunos_encontrados;

    cout << "   \t\t" << "\t" << "Pesquisar Alunos Pelo N. BI" << endl << endl;

    int n_bi;
    cout << "   \t\t" << "Pesquisar N. BI: ";
    util::inputRestriction(6, 1000000, false, this, &Manager::pesquisarAlunos, n_bi);

    extrairAlunosDeTodaAEscola(deque_alunos);

    encontrarAlunosPeloNumeroBI(deque_alunos, alunos_encontrados, to_string(n_bi));

    if (alunos_encontrados.empty())
    {
        cout << endl << endl << "Sem Resusltados" << endl;

        util::pressionarEnter();

        pesquisarAlunos();
    }
    else
    {
        sort(alunos_encontrados.begin(), alunos_encontrados.end(), ordenarAlunosPorNomeCrescente);

        cout << endl << endl;
        displayAlunos(alunos_encontrados, true);

        pesquisarAlunos();
    }
}

void Manager::editarAlunos()
{
    cabecalho();

    cout << "   \t\t" << "\t" << "Editar Alunos" << endl << endl;

    cout << "   \t\t" << "1. Editar Informacoes De Um Aluno" << endl << endl;
    cout << "   \t\t" << "2. Transferir Aluno De Turma" << endl << endl;
    cout << "   \t\t" << "3. Apagar Registo De Um Aluno" << endl << endl;
    cout << "   \t\t" << "0. Voltar" << endl << endl;
    cout << "   \t\t" << "->";
    char opt = _getch();

    while (opt != '1' && opt != '2' && opt != '3' && opt != '0' && opt != 8 && opt != 27 )
    {
        opt = _getch();
    }

    switch(opt)
    {
    case '1':
        editarInformacoesAluno();
        break;

    case '2':
        transferirAlunoDeTurma();
        break;

    case '3':
        apagarRegistoAluno();
        break;

    case '0':
    case 8:
        verAlunos();
        break;

    case 27:
        paginaInicial();
        break;

    default:
        editarAlunos();
        break;
    }
}

void Manager::editarInformacoesAluno()
{
    cabecalho();

    cout << "   \t\t" << "\t" << "Editar Informacoes De Um Aluno" << endl << endl;

    string file_path, ano_escolaridade_, turma_;
    int numero_do_aluno;
    deque <Estudante> deque_alunos;
    deque <Estudante> deque_aluno_a_editar;

    inserirTurma(file_path, ano_escolaridade_, turma_, &Manager::editarAlunos);

    cout << endl << "   \t\t" << "  Inserir Numero Do Aluno: ";
    util::inputRestriction(2, 100, true, this, &Manager::editarAlunos, numero_do_aluno);

    if (util::seFicheiroVazio(this, &paginaInicial, file_path, "editarInformacoesAluno"))
    {
        cout << endl << endl << "   \t\t" << "--Turma Selecionada Nao Contem Alunos--" << endl << endl;

        cout << "   \t\t" << " 1. Entrar de novo" << endl;
        cout << "   \t\t" << " 0. Cancelar" << endl;
        cout << "   \t\t" << " -> ";
        char opt = _getch();

        while (opt != '1' && opt != '0')
        {
            opt = _getch();
        }

        switch(opt)
        {
        case '1':
            editarInformacoesAluno();
            break;

        case '0':
            editarAlunos();
            break;
        }
    }

    extrairAlunosDeUmaTurma(deque_alunos, file_path);

    int indice_aluno;

    bool aluno_encontrado = procurarAluno(deque_alunos, indice_aluno, to_string(numero_do_aluno));

    if (! aluno_encontrado)
    {
        cout << endl << endl << "   \t\t" << "--Aluno Nao Encontrado--" << endl << endl;

        cout << "   \t\t" << " 1. Entrar de novo" << endl;
        cout << "   \t\t" << " 0. Cancelar" << endl;
        cout << "   \t\t" << " -> ";
        char opt = _getch();

        while (opt != '1' && opt != '0')
        {
            opt = _getch();
        }

        switch(opt)
        {
        case '1':
            editarInformacoesAluno();
            break;

        case '0':
            editarAlunos();
            break;
        }
    }
    else
    {
        cabecalho();

        cout << "   \t\t" << "\t" << "Editar Informacoes Do Aluno" << endl << endl;

        deque_aluno_a_editar.push_back(deque_alunos[indice_aluno]);

        util::ajustarTamanhoJanela(util::TamanhoConsole::Aumentada);

        displayAlunos(deque_aluno_a_editar, false);

        cout << endl << " 1. Entrar de novo" << endl;
        cout << " 2. Editar" << endl;
        cout << " 0. Cancelar" << endl;
        cout << " -> ";
        char opt = _getch();

        while (opt != '1' && opt != '2' && opt != '0')
        {
            opt = _getch();
        }

        util::ajustarTamanhoJanela(util::TamanhoConsole::Normal);

        if (opt == '1')
            editarInformacoesAluno();

        if (opt == '2')
        {
            cabecalho();

            cout << "   \t\t" << "\t" << "Editar Informacoes Do Aluno" << endl << endl;

            util::ajustarTamanhoJanela(util::TamanhoConsole::Aumentada);

            displayAlunos(deque_aluno_a_editar, false);

            bool editar_tudo = false;

            int opcao;

            cout << endl;

            cout << " 1. Editar Nome" << endl;
            cout << " 2. Editar Data De Nascimento" << endl;
            cout << " 3. Editar Morada" << endl;
            cout << " 4. Genero" << endl;
            cout << " 5. Editar Estado Civil" << endl;
            cout << " 6. Editar Numero De Identificacao Civil" << endl;
            cout << " 7. Editar Tudo" << endl;
            cout << " -> ";
            util::inputRestriction(1, 7, true, this, &Manager::editarAlunos, opcao);

            if(opcao == 7)
                editar_tudo = true;

            editarInformacoesDoAluno(editar_tudo, opcao, deque_aluno_a_editar);

            swap(deque_alunos[0], deque_alunos[indice_aluno]);

            deque_alunos.pop_front();
            deque_alunos.push_back(deque_aluno_a_editar[0]);

            atualizarTurmaAposEdicao(file_path, deque_alunos);

            system("cls");

            cout << "   \t\t" << "\t" << "Informacoes Do Aluno Editadas Com Sucesso" << endl;

            Sleep(1500);

            util::ajustarTamanhoJanela(util::TamanhoConsole::Normal);

            editarAlunos();
        }

        if (opt == '0')
            editarAlunos();
    }
}

void Manager::editarInformacoesDoAluno(bool editar_tudo, int opcao, deque<Estudante>& aluno_a_edtitar)
{
    cabecalho();

    cout << "   \t\t" << "\t" << "Editar Informacoes Do Aluno" << endl << endl;

    displayAlunos(aluno_a_edtitar, false);

    if (editar_tudo || opcao == 1)
    {
        cout << endl << endl << "  Novo Nome: ";
        util::validarInput(aluno_a_edtitar[0].nome, this, &Manager::editarAlunos);

        cout << endl <<  "  Novo Apelido: ";
        util::validarInput(aluno_a_edtitar[0].apelido, this, &Manager::editarAlunos);
    }

    if (editar_tudo || opcao == 2)
    {
        time_t now = time(nullptr);
        tm* agora = localtime(&now);

        int presente_ano = 1900 + agora->tm_year;

        int ano_nascimento_, mes_nascimento_, dia_nascimento_;

        cout << endl << "  Nova Data De Nascimento: " << endl;
        cout << "   Ano: ";
        util::inputRestriction(4, presente_ano, false, this, &Manager::editarAlunos, aluno_a_edtitar[0].ano_nascimento);

        cout << endl << "   Mes: ";
        util::inputRestriction(2, 12, true, this, &Manager::editarAlunos, aluno_a_edtitar[0].mes_nascimento);

        cout << endl << "   Dia: ";
        util::inputRestriction(2, 31, true, this, &Manager::editarAlunos, aluno_a_edtitar[0].dia_nascimento);

        while (! util::dataValido(ano_nascimento_, mes_nascimento_, dia_nascimento_))
        {
            cout << endl << endl <<  "  Data Invalido" << endl;
            cout << "   Ano: ";
            util::inputRestriction(4, presente_ano, false, this, &Manager::editarAlunos, aluno_a_edtitar[0].ano_nascimento);

            cout << endl << "   Mes: ";
            util::inputRestriction(2, 12, true, this, &Manager::editarAlunos, aluno_a_edtitar[0].mes_nascimento);

            cout << endl << "   Dia: ";
            util::inputRestriction(2, 31, true, this, &Manager::editarAlunos, aluno_a_edtitar[0].dia_nascimento);
        }
    }

    if (editar_tudo || opcao == 3)
    {
        cout << endl << endl << "  Nova Morada: ";
        util::validarInput(aluno_a_edtitar[0].morada, this, &Manager::editarAlunos);
    }

    if (editar_tudo || opcao == 4)
    {
        cout << endl << "  Novo Genero:" << endl;
        cout << "   1 -> Masculino" << endl;
        cout << "   2 -> Feminino" << endl;
        cout << "   ->";
        int opt;
        util::inputRestriction(1, 2, true, this, &Manager::editarAlunos, opt);

        if ( opt == 1)
            aluno_a_edtitar[0].genero = "M";
        if (opt == 2)
            aluno_a_edtitar[0].genero  = "F";
    }

    if (editar_tudo || opcao == 5)
    {
        int opt;
        cout << endl << endl << "  Novo Estado Civil:" << endl;
        cout << "   1 -> Solteiro" << endl;
        cout << "   2 -> Casado" << endl;
        cout << "   ->";
        util::inputRestriction(1, 2, true, this, &Manager::editarAlunos, opt);

        if ( opt == 1)
            aluno_a_edtitar[0].estado_civil = "Solteiro";
        if (opt == 2)
            aluno_a_edtitar[0].estado_civil = "Casado";
    }

    if (editar_tudo || opcao == 6)
    {
        int opt;
        cout << endl << endl << "  Novo Numero de Identificacao Civil: ";
        aluno_a_edtitar[0].numero_identificacao_civil = util::inputRestriction(6, 1000000, false, this, &Manager::editarAlunos, opt);
    }
}

void Manager::transferirAlunoDeTurma()
{
    cabecalho();

    cout << "   \t\t" << "\t" << "Transferir Aluno De Turma" << endl << endl;

    string file_path, ano_escolaridade_, turma_;
    int numero_do_aluno;
    deque <Estudante> deque_alunos;
    deque <Estudante> deque_aluno_a_editar;

    inserirTurma(file_path, ano_escolaridade_, turma_, &Manager::editarAlunos);

    cout << endl << "   \t\t" << "  Inserir Numero Do Aluno: ";
    util::inputRestriction(2, 100, true, this, &Manager::editarAlunos, numero_do_aluno);

    if (util::seFicheiroVazio(this, &paginaInicial, file_path, "transferirAlunoDeTurma"))
    {
        cout << endl << endl << "   \t\t" << "--Turma Selecionada Nao Contem Alunos--" << endl << endl;

        cout << "   \t\t" << " 1. Entrar de novo" << endl;
        cout << "   \t\t" << " 0. Cancelar" << endl;
        cout << "   \t\t" << " -> ";
        char opt = _getch();

        while (opt != '1' && opt != '0')
        {
            opt = _getch();
        }

        switch(opt)
        {
        case '1':
            transferirAlunoDeTurma();
            break;

        case '0':
            editarAlunos();
            break;
        }
    }

    extrairAlunosDeUmaTurma(deque_alunos, file_path);

    int indice_aluno;

    bool aluno_encontrado = procurarAluno(deque_alunos, indice_aluno, to_string(numero_do_aluno));

    if (! aluno_encontrado)
    {
        cout << endl << endl << "   \t\t" << "--Aluno Nao Encontrado--" << endl << endl;

        cout << "   \t\t" << " 1. Entrar de novo" << endl;
        cout << "   \t\t" << " 0. Cancelar" << endl;
        cout << "   \t\t" << " -> ";
        char opt = _getch();

        while (opt != '1' && opt != '0')
        {
            opt = _getch();
        }

        switch(opt)
        {
        case '1':
            transferirAlunoDeTurma();
            break;

        case '0':
            editarAlunos();
            break;
        }
    }
    else
    {
        cabecalho();

        cout << "   \t\t" << "\t" << "Transferir Aluno" << endl << endl;

        deque_aluno_a_editar.push_back(deque_alunos[indice_aluno]);

        displayAlunos(deque_aluno_a_editar, false);

        cout << endl;
        cout  << " 1. Entrar de novo" << endl;
        cout << " 2. Transferir" << endl;
        cout << " 0. Cancelar" << endl;
        cout << " -> ";
        char opt = _getch();

        while (opt != '1' && opt != '2' && opt != '0')
        {
            opt = _getch();
        }

        if (opt == '1')
        {
            util::ajustarTamanhoJanela(util::TamanhoConsole::Normal);
            transferirAlunoDeTurma();
        }

        if (opt == '2')
        {
            swap(deque_alunos[0], deque_alunos[indice_aluno]);

            deque_alunos.pop_front();

            atualizarTurmaAposEdicao(file_path, deque_alunos);

            transferirUmAlunoDeTurma(deque_aluno_a_editar);

            system("cls");

            cout << "   \t\t" << "\t" << "Aluno Transferido Com Sucesso" << endl;

            Sleep(1500);

            editarAlunos();
        }

        if (opt == '0')
        {
            util::ajustarTamanhoJanela(util::TamanhoConsole::Normal);
            editarAlunos();
        }
    }
}

void Manager::transferirUmAlunoDeTurma(deque<Estudante>& deque_alunos)
{
    cabecalho();

    cout << "   \t\t" << "\t" << "Transferir Aluno" << endl << endl;

    displayAlunos(deque_alunos, false);

    cout << endl << "Para Turma:" <<endl << endl;

    string file_path, ano_escolaridade_, turma_, numero_do_aluno_;
    deque <Estudante> deque_alunos_;
    deque <Estudante> deque_aluno_a_editar;

    inserirTurma(file_path, ano_escolaridade_, turma_, &Manager::editarAlunos);

    // mudar a turma e o ano da escolaridade antes de escrever as informacoes do aluno no novo ficheiro
    deque_alunos[0].ano_escolaridade = ano_escolaridade_;
    deque_alunos[0].turma = turma_;

    if ( ! util::seFicheiroVazio(this, &paginaInicial, file_path, "transferirUmAlunoDeTurma"))
        extrairAlunosDeUmaTurma(deque_alunos_, file_path);

    deque_alunos_.push_back(deque_alunos[0]);

    atualizarTurmaAposEdicao(file_path, deque_alunos_);
}

void Manager::apagarRegistoAluno()
{
    cabecalho();

    cout << "   \t\t" << "\t" << "Apagar Registo De Um Aluno" << endl << endl;

    string file_path, ano_escolaridade_, turma_;
    int numero_do_aluno;
    deque <Estudante> deque_alunos;
    deque <Estudante> deque_aluno_a_editar;

    inserirTurma(file_path, ano_escolaridade_, turma_, &Manager::editarAlunos);

    cout << endl << "   \t\t" << "  Inserir Numero Do Aluno: ";
    util::inputRestriction(2, 100, true, this, &Manager::editarAlunos, numero_do_aluno);

    if (util::seFicheiroVazio(this, &paginaInicial, file_path, "apagarRegistoAluno"))
    {
        cout << endl << endl << "   \t\t" << "--Turma Selecionada Nao Contem Alunos--" << endl << endl;

        cout << "   \t\t" << " 1. Entrar de novo" << endl;
        cout << "   \t\t" << " 0. Cancelar" << endl;
        cout << "   \t\t" << " -> ";
        char opt = _getch();

        while (opt != '1' && opt != '0')
        {
            opt = _getch();
        }

        switch(opt)
        {
        case '1':
            apagarRegistoAluno();
            break;

        case '0':
            editarAlunos();
            break;
        }
    }

    extrairAlunosDeUmaTurma(deque_alunos, file_path);

    int indice_aluno;

    bool aluno_encontrado = procurarAluno(deque_alunos, indice_aluno, to_string(numero_do_aluno));

    if (! aluno_encontrado)
    {
        cout << endl << endl << "   \t\t" << "--Aluno Nao Encontrado--" << endl << endl;

        cout << "   \t\t" << " 1. Entrar de novo" << endl;
        cout << "   \t\t" << " 0. Cancelar" << endl;
        cout << "   \t\t" << " -> ";
        char opt = _getch();

        while (opt != '1' && opt != '0')
        {
            opt = _getch();
        }

        switch(opt)
        {
        case '1':
            apagarRegistoAluno();
            break;

        case '0':
            editarAlunos();
            break;
        }
    }
    else
    {
        cabecalho();

        cout << "   \t\t" << "\t" << "Apagar Registo Do Aluno" << endl << endl;

        deque_aluno_a_editar.push_back(deque_alunos[indice_aluno]);

        util::ajustarTamanhoJanela(util::TamanhoConsole::Aumentada);

        displayAlunos(deque_aluno_a_editar, false);

        cout << " 1. Entrar de novo" << endl;
        cout<< " 2. Apagar" << endl;
        cout << " 0. Cancelar" << endl;
        cout << " -> ";
        char opt = _getch();

        while (opt != '1' && opt != '2' && opt != '0' && opt != 27)
        {
            opt = _getch();
        }

        util::ajustarTamanhoJanela(util::TamanhoConsole::Normal);

        if (opt == '1')
        {
            apagarRegistoAluno();
        }
        else if (opt == '2')
        {
            swap(deque_alunos[0], deque_alunos[indice_aluno]);

            deque_alunos.pop_front();

            atualizarTurmaAposEdicao(file_path, deque_alunos);

            system("cls");

            cout << "   \t\t" << "\t" << "Registo Do Aluno Apagado Com Sucesso" << endl;

            Sleep(1500);

            editarAlunos();
        }
        else if (opt == '3')
        {
            editarAlunos();
        }
        else if(opt == 27)
        {
            paginaInicial();
        }
    }
}

bool Manager::procurarAluno(deque<Estudante>& deque_alunos, int& indice_aluno, const string& numero_do_aluno_)
{
    for ( int i = 0, len = deque_alunos.size(); i < len; ++i)
        if (deque_alunos[i].numero_do_aluno == numero_do_aluno_)
        {
            indice_aluno = i;
            return true;
        }

    return false;
}

void Manager::atualizarTurmaAposEdicao(string file_path, deque<Estudante>& deque_alunos)
{
    sort(deque_alunos.begin(), deque_alunos.end(), ordenarAlunosPorNomeCrescente);

    for ( int i = 0, len = deque_alunos.size(); i < len; ++i)
    {
        deque_alunos[i].numero_do_aluno = to_string(i+1);
    }

    util::set_a_FileAttribute_To_ReadOnly_or_Normal(file_path, FILE_ATTRIBUTE_NORMAL);

    ofstream file(file_path);

    for ( int i = 0, len = deque_alunos.size(); i < len; ++i)
        file << deque_alunos[i] << endl;

    util::set_a_FileAttribute_To_ReadOnly_or_Normal(file_path, FILE_ATTRIBUTE_READONLY);
}

void Manager::ordenar(deque<Estudante>& alunos)
{
    cout << endl << endl << "   \t\t" << "Ordenar Por" << endl << endl;

    cout << "   \t\t" << "1. Nome Crescente" << endl;
    cout << "   \t\t" << "2. Nome Decrescente" << endl;
    cout << "   \t\t" << "3. Idade Crescente" << endl;
    cout << "   \t\t" << "4. Idade Decrescente" << endl;
    cout << "   \t\t" << "5. Morada" << endl;

    cout << "   \t\t" << "->";
    char ch = _getch();

    while (ch != '1' && ch != '2' && ch != '3' && ch != '4' && ch != '1' && ch != '5' && ch != 27)
    {
        ch = _getch();
    }

    switch(ch)
    {
    case '1':
        sort(alunos.begin(), alunos.end(), ordenarAlunosPorNomeCrescente);
        break;

    case '2':
        sort(alunos.begin(), alunos.end(), ordenarAlunosPorNomeDecrescente);
        break;

    case '3':
        sort(alunos.begin(), alunos.end(), ordenarAlunosPorIdadeCrescente);
        break;

    case '4':
        sort(alunos.begin(), alunos.end(), ordenarAlunosPorIdadeDecrescente);
        break;

    case '5':
        sort(alunos.begin(), alunos.end(), ordenarAlunosPorMorada);
        break;

    case 27:
        paginaInicial();
        break;
    }
}

void Manager::encontrarAlunosPeloNome(deque<Estudante>& alunos, deque<Estudante>& alunos_encontrados, string nome_de_pesquisa)
{
    for ( int i = 0, len = alunos.size(); i < len; ++i)
    {
        auto aluno_encontrado = alunos[i].nome_completo.find(nome_de_pesquisa);

        if (aluno_encontrado != string::npos)
            alunos_encontrados.push_back(alunos[i]);
    }
}

void Manager::encontrarAlunosPelaIdade(deque<Estudante>& alunos, deque<Estudante>& alunos_encontrados, int idade)
{
    for ( int i = 0, len = alunos.size(); i < len; ++i)
    {
        if (alunos[i].idade == idade)
            alunos_encontrados.push_back(alunos[i]);
    }
}

void Manager::encontrarAlunosPelaMorada(deque<Estudante>& alunos, deque<Estudante>& alunos_encontrados, string morada)
{
    for (int i = 0, len = alunos.size(); i < len; ++i)
    {
        auto aluno_encontrado = alunos[i].morada.find(morada);

        if (aluno_encontrado != string::npos)
            alunos_encontrados.push_back(alunos[i]);
    }
}

void Manager::encontrarAlunosPelaDataNascimento(deque<Estudante>& alunos, deque<Estudante>& alunos_encontrados, string data_nascimento)
{
    for ( int i = 0, len = alunos.size(); i < len; ++i)
    {
        if (alunos[i].data_de_nascimento == data_nascimento)
            alunos_encontrados.push_back(alunos[i]);
    }
}

void Manager::encontrarAlunosPeloAnoNascimento(deque<Estudante>& alunos, deque<Estudante>& alunos_encontrados, int ano_nascimento_)
{
    for (int i = 0, len = alunos.size(); i < len; ++i)
    {
        if (alunos[i].ano_nascimento == ano_nascimento_)
            alunos_encontrados.push_back(alunos[i]);
    }
}

void Manager::encontrarAlunosPeloMesNascimento(deque<Estudante>& alunos, deque<Estudante>& alunos_encontrados, int mes_nascimento_)
{
    for ( int i = 0, len = alunos.size(); i < len; ++i)
    {
        if (alunos[i].mes_nascimento == mes_nascimento_)
            alunos_encontrados.push_back(alunos[i]);
    }
}

void Manager::encontrarAlunosPelo_Mes_Ano_Nascimento(deque<Estudante>& alunos, deque<Estudante>& alunos_encontrados, string mes_ano)
{
    for (int i = 0, len = alunos.size(); i < len; ++i)
    {
        string mes_ano_aluno = alunos[i].mes_nascimento + "-" + alunos[i].ano_nascimento;

        if (mes_ano_aluno == mes_ano)
            alunos_encontrados.push_back(alunos[i]);
    }
}

void Manager::encontrarAlunosPeloNumeroBI(deque<Estudante>& alunos, deque<Estudante>& alunos_encontrados, string n_bi)
{
    for ( int i = 0, len = alunos.size(); i < len; ++i)
    {
        if (alunos[i].numero_identificacao_civil == n_bi)
        {
            alunos_encontrados.push_back(alunos[i]);
            break;
        }
    }
}

int  Manager::contarNumeroDeAlunosPeloGenero(deque<Estudante>& alunos, char opcao_de_genero)
{
    int numero_de_alunos = 0;

    for ( int i = 0, len = alunos.size(); i < len; ++i)
    {
        if (opcao_de_genero == '1')
        {
            if (alunos[i].genero == "M")
                ++numero_de_alunos;
        }
        else if (opcao_de_genero == '2')
        {
            if (alunos[i].genero == "F")
                ++numero_de_alunos;
        }
        else if (opcao_de_genero == '3')
        {
            numero_de_alunos = len;
            break;
        }
    }

    return numero_de_alunos;
}

bool Manager::ordenarAlunosPorMorada(const Estudante& es1, const Estudante& es2)
{
    string str1 = es1.morada, str2 = es2.morada;
    transform(str1.begin(), str1.end(), str1.begin(), ::tolower);
    transform(str2.begin(), str2.end(), str2.begin(), ::tolower);

    return (str1 < str2);
}

bool Manager::ordenarAlunosPorNomeCrescente(const Estudante& es1, const Estudante& es2)
{
    string str1 = es1.nome, str2 = es2.nome;
    transform(str1.begin(), str1.end(), str1.begin(), ::tolower);
    transform(str2.begin(), str2.end(), str2.begin(), ::tolower);

    return (str1 < str2);
}

bool Manager::ordenarAlunosPorNomeDecrescente(const Estudante& es1, const Estudante& es2)
{
    string str1 = es1.nome, str2 = es2.nome;
    transform(str1.begin(), str1.end(), str1.begin(), ::tolower);
    transform(str2.begin(), str2.end(), str2.begin(), ::tolower);

    return (str1 > str2);
}

bool Manager::ordenarAlunosPorIdadeCrescente(const Estudante& es1, const Estudante& es2)
{
    return (es1.idade < es2.idade);
}

bool Manager::ordenarAlunosPorIdadeDecrescente(const Estudante& es1, const Estudante& es2)
{
    return (es1.idade > es2.idade);
}

bool Manager::turmaExiste(string path, string turma)
{
    return std::filesystem::exists(path + "/" + turma);
}

void Manager::sair()
{
    system("cls");

    char option;

    cout << "\n\n\n\n\n\n \t\t\t\t\t" << "   Sair" << endl << endl;
    cout << " \t\t\t\t  " << "1 -> Sim" << "     " << "2 -> Nao" << endl << endl;
    cout << " \t\t\t\t\t " << " -> ";
    option = _getch();

    while (option != '1' && option != '2')
    {
        option = _getch();
    }

    switch (option)
    {
    case '1':
        startMenu();
        break;

    case '2':
        paginaInicial();
        break;

    default:
        sair();
        break;
    }
}




















