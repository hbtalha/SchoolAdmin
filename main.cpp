#include <iostream>
#include <windows.h>
#include "Manager.h"

using std::cout;
using std::endl;

void displayAuthor()
{
    cout << endl << endl << endl << endl;

    cout << "\t\t\t\t" << "Helder Batalha" << endl << endl;

    Sleep(800);
}

void changeConsoleTextColor()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    SetConsoleTextAttribute(hConsole, 2);
}

void ajustarTamanhoJanelaOriginal()
{
    HWND console = GetConsoleWindow();

    RECT r;

    GetWindowRect(console, &r);

    MoveWindow(console, r.left, r.top, 900, 560, TRUE);
}

int main()
{
    changeConsoleTextColor();

    ajustarTamanhoJanelaOriginal();

    Manager& es = Manager::getOnlyInstance();

    displayAuthor();

    es.startMenu();

    return 0;
}
