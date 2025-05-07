#include <iostream>
#include "Menu.h"
#include "Graphic.h"
using namespace std;

int main() {
    DWORD style = GetWindowLong(GetConsoleWindow(), GWL_STYLE);
    style &= ~WS_CAPTION;
    SetWindowLong(GetConsoleWindow(), GWL_STYLE, style);
    SetWindowText(GetConsoleWindow(), TEXT(""));
    system("color e0");
    SetTitle();
    FixConsole();
    PrintMenu();
    return 0;
}



