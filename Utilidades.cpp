#include "Diretorios.h"
#include "Utilidades.h"

#include <iostream>
#include <conio.h>
#include <dirent.h>
#include <windows.h>
#include <filesystem>


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
using std::filesystem::create_directories;

namespace util
{

// função para inserir a data de inauguracao da escola
void dataDaInauguracaoDaEscola(Manager *m, string& s_ano, string& s_mes, string& s_dia)
{
    time_t now = time(nullptr);
    tm* agora = localtime(&now);

    int presente_ano = 1900 + agora->tm_year;

    int ano_inauguracao, mes_inauguracao, dia_inauguracao;

    cout << "   \t\t" << "Ano: ";

    bool ano_inauguracao_minimo = true;

    while(ano_inauguracao_minimo == true)
    {
        s_ano = util::inputRestriction(4, presente_ano, false, m, &Manager::startMenu, ano_inauguracao);

        if(ano_inauguracao < 1904)
        {
            cout << "\b \b\b \b\b \b\b \b";
        }
        else
            ano_inauguracao_minimo = false;
    }

    cout << endl << "   \t\t" << "Mes: ";
    s_mes = inputRestriction(2, 12, true, m, &Manager::startMenu, mes_inauguracao);

    cout << endl << "   \t\t" << "Dia: ";
    s_dia = inputRestriction(2, 31, true, m, &Manager::startMenu, dia_inauguracao);

    while ( ! dataValido(ano_inauguracao, mes_inauguracao, dia_inauguracao))
    {
        cout << endl << endl << "   \t\t" <<  "Data Invalido" << endl;

        cout << "   \t\t" << "Ano: ";

        bool ano_inauguracao_minimo = true;

        while(ano_inauguracao_minimo == true)
        {
            s_ano = inputRestriction(4, presente_ano, false, m, &Manager::startMenu, ano_inauguracao);

            if(ano_inauguracao < 1904)
            {
                cout << "\b \b\b \b\b \b\b \b";
            }
            else
                ano_inauguracao_minimo = false;
        }

        cout << endl << "   \t\t" << "Mes: ";
        s_mes = inputRestriction(2, 12, true, m, &Manager::startMenu, mes_inauguracao);

        cout << endl << "   \t\t" << "Dia: ";
        s_dia = inputRestriction(2, 31, true, m, &Manager::startMenu, dia_inauguracao);
    }
}

// essa tem como objectivo regular os dados inseridos ou seja
// garantir que p/ex nao haverá datas como 31 de fevereiro
bool dataValido (const int& ano, const int& mes, const int& dia)
{
    time_t now = time(nullptr);
    tm* agora = localtime(&now);

    int presente_ano = 1900 + agora->tm_year;

    int ano_decrescente = presente_ano;

    bool leap_year = false;

    // um loop que irá do ano presente ate 1960
    // irá identificar caso o ano da inauguração é um ano bisexto
    while (ano_decrescente > 1959)
    {
        if (ano_decrescente % 4 == 0 and ano == ano_decrescente)
        {
            leap_year = true;

            break;
        }

        --ano_decrescente;
    }

    if ( leap_year)
        if (mes == 2 && dia > 29)
            return false;

    if ( ! leap_year)
        if (mes == 2 && dia > 28)
            return false;

    if ( mes ==  4 || mes == 6 || mes == 9 || mes == 11)
        if (dia > 30)
            return false;

    return true;
}

// função que irá validar nome e localização da escola
// Irá garantir que esses não sejam vazios, não tenham numeros e espaços em locais inválidos
void validarInput(string& input, Manager *m, void (Manager::*funcao_de_retorno) ())
{
    getline(cin, input);

    if (input == "0")
        (m->*funcao_de_retorno)();

    while (input.empty())
    {
        cout << endl << "   \t\t" << "Obrigatorio" << endl << "   \t\t" << " Entrar De Novo:: ";
        getline(cin, input);

        if (input == "0")
            (m->*funcao_de_retorno)();
    }

    for ( int i = 0, len = input.size(); i < len; ++i)
    {
        if  (( ! isalpha(input[i]) && ! isspace(input[i]) ) || isspace(input[0]))
        {
            cout << endl << "   \t\t" << "Invalido" << endl << "   \t\t" << " Entrar De Novo: ";
            getline(cin, input);

            if (input == "0")
                (m->*funcao_de_retorno)();

            while (input.empty())
            {
                cout << endl << "   \t\t" << "Obrigatorio" << endl << "   \t\t" << " Entrar De Novo:: ";
                getline(cin, input);

                if (input == "0")
                    (m->*funcao_de_retorno)();
            }

            i = -1;
        }
    }

    regularizarNomes(input);
}

// função onde ira garantir que o input do usuario na hora de inserir as datas seja valido
// o bool 'chck' é usado para notificar se o input eh do ano ou nao para que sejam feitas as mudanças
// o bool 'opcao_voltar' é usado para quando o usuário quer cancelar a operação, ira decidir se irá volta para 'paginaInicial' ou 'startMenu'
// necessárias, no ano o usuario tem que inserir 4 carateres, mas no mes e no dia pode ser 1 ou 2
// essa função é usada em diversas outras situações por ser bastante conveniente
string inputRestriction(int length_limit, int limite, bool chck, Manager *m, void (Manager::*funcao_de_retorno) (), int& _data )
{
    string s_in;
    char ch_;
    int length = 1, c_a;
    bool brk = false; //

    do
    {
        ch_ = _getch();

        if (ch_ == 27)
            (m->*funcao_de_retorno)();

        while (ch_ == 13 && length <= length_limit)
        {
            if (s_in.empty())
                cout << "\b  \b";

            ch_ = getch();

            // if the user wishes to go back to the menu he/she presses esc
            if (ch_ == 27)
                (m->*funcao_de_retorno)();
        }

        while (ch_ != 13)
        {
            bool bkr = false;

            if (ch_ == '0' && s_in.empty()) // condicao que proibe o input de 0 como o primeiro carater
                bkr = true;

            if (s_in.empty())
            {
                cout << "  \b";
            }

            if (ch_ == 8)
            {
                if ( ! s_in.empty())
                {
                    -- length;
                    s_in.pop_back();
                }

                cout << "\b \b";
            }
            else if (length <= length_limit && isdigit(ch_) && ! bkr)
            {
                if (s_in.empty())
                    cout << "\b \b";

                s_in.push_back(ch_);

                cout << ch_;

                ++length;
            }
            else if ( s_in.empty())
            {
                cout << "\b \b";
            }

            ch_ = _getch();

            if (ch_ == 27)
                (m->*funcao_de_retorno)();

            if (chck == false)
            {
                while (ch_ == 13 && length <= length_limit)
                {
                    if (s_in.empty())
                    {
                        cout << " \b";
                    }

                    ch_ = _getch();

                    if (ch_ == 27)
                        (m->*funcao_de_retorno)();
                }
            }

            if (chck)
            {
                if (ch_ == 13 && s_in.size() == 1 && s_in[1] != '0')
                    brk = true;

                while (ch_ == 13 && s_in.empty())
                {
                    if (s_in.empty())
                    {
                        cout << " \b";
                    }

                    ch_ = _getch();

                    if (ch_ == 27)
                        (m->*funcao_de_retorno)();
                }
            }
        }

        c_a = stoi(s_in.c_str());

        // condicao que irá garantir que caso o numero limite for de apenas um carater o while nao ira ter um break
        // caso nao for apropriado
        if (c_a > limite)
            brk = false;
    }
    while ( c_a > limite && ! brk );

    _data = c_a;

    return s_in;
}

void criarPastasPrincipais()
{
    class stat info;

    create_directories(Diretorios::ESCOLAS_INFO);

    create_directories(Diretorios::ESCOLAS);
}

void criarPastasDaEscola(std::string nome_da_escola)
{
    // criar pastas que vao conter as turmas, da 1classe ao 12ano
    for (int i = 1; i <= 12; i++)
    {
        string f_path = Diretorios::ESCOLAS + nome_da_escola + "/Classes/" + to_string( i );

        create_directories(f_path);
    }

    // criar pastas de CT, ES, H, no 11ano e 12ano
    for (int i = 11; i <= 12; i++)
    {
//        stringstream converter;
//        converter << i;
        string ano_de_escolaridade = to_string( i );
//        converter >> ano_de_escolaridade;

        string ct_path = Diretorios::ESCOLAS + nome_da_escola + "/Classes/" + ano_de_escolaridade + "/CT";
        string es_path = Diretorios::ESCOLAS + nome_da_escola + "/Classes/" + ano_de_escolaridade + "/ES";
        string h_path = Diretorios::ESCOLAS + nome_da_escola + "/Classes/" + ano_de_escolaridade + "/H";

        create_directories(ct_path);

        create_directories(es_path);

        create_directories(h_path);
    }
}

// variavel 'warn' eh usada apenas no debug
bool seFicheiroVazio(Manager *m, void (Manager::*funcao_retorno)(), const string& file_path, string warn)
{
    ifstream file(file_path);

    if (! file)
    {
        cerr << endl << endl << endl << "\a" << "*****Erro Ao Verificar Se O Ficheiro Encontra-se Vazio*****" << endl;

        Sleep(2000);

        file.close();

        (m->*funcao_retorno)();
    }

    bool ficheiro_vazio = (file.peek() == ifstream::traits_type::eof());

    file.close();

    return ficheiro_vazio;
}

void regularizarNomes(string& str)
{
    int len = str.size();
    for(int i = 0; i <len; ++i)
    {
        if(i == 0)
            for(int j = i; i < len; ++j)
            {
                if(j != 0)
                    str[ j ] = tolower(str[ j ]);

                if(isspace(str[ j ]))
                {
                    i = j;
                    break;
                }
            }

        if(isspace(str[i]))
            for(int j = i; j < len; ++j)
            {
                if(! isspace(str[j]))
                {
                    if( ! isupper(str[j]))
                        str[j] = toupper(str[j]);

                    for(int k = j + 1; k < len; ++k)
                    {
                        if( isupper(str[k]))
                            str[k] = tolower(str[k]);

                        if(isspace(str[k]))
                        {
                            i = k -1;
                            break;
                        }
                    }

                    break;
                }
            }
    }

    if (! isupper(str[0]))
        str[0] = toupper(str[0]);

    // erases all space in between the names
    str.erase(std::remove(str.begin(), str.end(), ' '), str.end());
}

void adicionarEspacos(string& str)
{
    for ( int i = 1,  len = str.size(); i < len; ++i)
        if ( isupper(str[i]))
        {
            str.insert(i, " ");
            ++i;
        }
}

void pressionarEnter()
{
    char ch = _getch();

    while (ch != 13)
    {
        ch = _getch();
    }
}

void set_a_FileAttribute_To_ReadOnly_or_Normal(string filepath, DWORD flag)
{
    const char* file_path = filepath.c_str();

    if (flag == FILE_ATTRIBUTE_READONLY)
    {
        SetFileAttributesA(file_path, FILE_ATTRIBUTE_READONLY);
    }
    else if (flag == FILE_ATTRIBUTE_NORMAL)
    {
        SetFileAttributesA(file_path, FILE_ATTRIBUTE_NORMAL);
    }
}

void ajustarTamanhoJanela(TamanhoConsole tamanho)
{
    HWND console = GetConsoleWindow();

    RECT r;

    GetWindowRect(console, &r);

    if(tamanho == TamanhoConsole::Aumentada)
    {
        MoveWindow(console, r.left, r.top, 1700, 560, TRUE);
    }
    else if(tamanho == TamanhoConsole::Normal)
    {
        MoveWindow(console, r.left, r.top, 900, 560, TRUE);
    }

}


}

