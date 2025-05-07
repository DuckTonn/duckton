#include "Menu.h"

//Hàm thay đổi tiêu đề cửa sổ Console
void SetTitle() {
    SetConsoleTitle(L"SNAKE HUNTING");
}

//Hàm in Menu
void PrintMenu() {
    SetConsoleOutputCP(65001);
    setDefaultConsoleFont();
    setConsoleFont(L"Cascadia Code Bold", 16);
    system("color e0");         //Hàm đổi màu nền thành màu vàng
    ReadImageFromFie("GameMenuFrame.txt", 15, 8);
    SetColor(14, 0);
    GotoXY(22, 38); cout << "        DESCRIPTIONS";
    StartGameMini(true);
    PrintTitle();
    GotoXY(70, 15);
    PrintOptions();
}

//Hàm in ra các chức năng ở Menu
void PrintOptions() {
    int Arrcolors[] = { 8, 14, 14, 14, 14, 14 };
    int counter = 1;        //Biến đếm con trỏ trong console
    showImages("Snake.txt", 64, 9, true);
    SetConsoleOutputCP(65001);

    for (;;) {

        SetColor(Arrcolors[0], 0);
        GotoXY(24, 12); cout << "┏┓┏┳┓┏┓┳┓┏┳┓  ┏┓┏┓┳┳┓┏┓";
        GotoXY(24, 13); cout << "┗┓ ┃ ┣┫┣┫ ┃   ┃┓┣┫┃┃┃┣ ";
        GotoXY(24, 14); cout << "┗┛ ┻ ┛┗┛┗ ┻   ┗┛┛┗┛ ┗┗┛";
        SetColor(Arrcolors[1], 0);
        GotoXY(24, 16);  cout << "┳┓┏┓┏┓┳┳┳┳┓┏┓  ┏┓┏┓┳┳┓┏┓";
        GotoXY(24, 17);  cout << "┣┫┣ ┗┓┃┃┃┃┃┣   ┃┓┣┫┃┃┃┣ ";
        GotoXY(24, 18);  cout << "┛┗┗┛┗┛┗┛┛ ┗┗┛  ┗┛┛┗┛ ┗┗┛";

        SetColor(Arrcolors[2], 0);
        GotoXY(24, 20);  cout << "┓┏┳┏┓┓┏┏┓┏┓┏┓┳┓┏┓┏┓";
        GotoXY(24, 21);  cout << "┣┫┃┃┓┣┫┗┓┃ ┃┃┣┫┣ ┗┓";
        GotoXY(24, 22);  cout << "┛┗┻┗┛┛┗┗┛┗┛┗┛┛┗┗┛┗┛";

        SetColor(Arrcolors[3], 0);
        GotoXY(24, 24);  cout << "┏┓┳┓┏┓┳┳┏┳┓";
        GotoXY(24, 25);  cout << "┣┫┣┫┃┃┃┃ ┃ ";
        GotoXY(24, 26);  cout << "┛┗┻┛┗┛┗┛ ┻ ";

        SetColor(Arrcolors[4], 0);
        GotoXY(24, 28);  cout << "┏┓┏┓┏┳┓┳┏┓┳┓┏┓";
        GotoXY(24, 29);  cout << "┃┃┃┃ ┃ ┃┃┃┃┃┗┓";
        GotoXY(24, 30);  cout << "┗┛┣┛ ┻ ┻┗┛┛┗┗┛";

        SetColor(Arrcolors[5], 0);
        GotoXY(24, 32);  cout << "┏┓┏┓┏┓┳┏┳┓  ┏┓┏┓┳┳┓┏┓";
        GotoXY(24, 33);  cout << "┣  ┃┃ ┃ ┃   ┃┓┣┫┃┃┃┣ ";
        GotoXY(24, 34);  cout << "┗┛┗┛┗┛┻ ┻   ┗┛┛┗┛ ┗┗┛";
        if (counter == 1) {
          
            StartGameMini(true);
            showImages("Snake.txt", 64, 9, true);
            SetConsoleOutputCP(65001);
        }
        else if (counter == 2) {

            ResumeGameMini(true);
            showImages("ResumeIcon.txt", 64, 9, true);
            SetConsoleOutputCP(65001);
        }
        else if (counter == 3) {

            HighscoresMini(true);
            showImages("Cup.txt", 64, 9, true);
            SetConsoleOutputCP(65001);
        }
        else if (counter == 4) {

            AboutMini(true);
            showImages("AmongUs.txt", 64, 9, true);
            SetConsoleOutputCP(65001);
        }
        else if (counter == 5) {
 
            OptionsMini(true);
            showImages("Options.txt", 64, 9, true);
            SetConsoleOutputCP(65001);
        }
        else if (counter == 6) {
            ExitGameMini(true);
            showImages("BYE.txt", 64, 9, true);
            SetConsoleOutputCP(65001);
        }
        char key = tolower(_getch());       //Nhận 1 ký tự bất kỳ từ bàn phím và luôn đưa nó về dạng chữ viết thường

        if (key == 119 && counter > 1) {  
            mutesoundeffect();
            playsoundeffect();  //119 là chữ 'w'
            counter--;
        }

        else if (key == 115 && counter < 6) {  

            mutesoundeffect();
            playsoundeffect();   //115 là chữ 's'
            counter++;
        }

        else if (key == '\r') {     // Kí hiệu '\r' là dấu Enter
            if (counter == 1) {
                mutesoundeffect();
                playsoundeffect();
                system("cls");
                system("color e0");
                setConsoleFont(L"Cascadia Code SemiBold", 20);
                PrintStartGameOption(77, 8);
                break;
            }
            else if (counter == 2) {
                mutesoundeffect();
                playsoundeffect();
                LoadResumeGame();
                if (STATE == 0) {
                    SetColor(14, 0);
                    GotoXY(22, 40); cout << "  There's no game to resume";
                    GotoXY(22, 41); cout << "  Please start a new game  ";
                    GotoXY(22, 42); cout << "                           ";
                    Sleep(2000);
                }
                else {
                    system("cls");
                    system("color e0");
                    StartResumeGame();
                    break;
                }
            }
            else if (counter == 3) {
                mutesoundeffect();
                playsoundeffect();
                system("cls");
                system("color e0");
                GotoXY(30, 0);
                setConsoleFont(L"Cascadia Code SemiBold", 25);
                showImages("Leaderboard.txt", 5, 0, true);
                HighScoreListOut(35, 14);
                for (int i = 33; i < 96; i++) {
                    GotoXY(i, 24);
                    cout << char(196);
                }
                GotoXY(52, 25); cout << "PRESS <ESC> TO BACK TO MENU";
                Back();
                break;
            }
            else if (counter == 4) {
                mutesoundeffect();
                playsoundeffect();
                system("cls");
                system("color e0");
                setConsoleFont(L"Cascadia Code SemiBold", 25);      //Hàm thay đổi font chữ và font size
                showImages("AboutBox.txt", 10, 0, true);
                PrintAbout();
                SetConsoleOutputCP(65001);
                Back();
                break;
            }
            else if (counter == 5) {
                mutesoundeffect();
                playsoundeffect();
                system("cls");
                system("color e0");
                setConsoleFont(L"Cascadia Code SemiBold", 20);
                OptionMenu(60, 12);
                break;
            }
            else if (counter == 6) {
                playsoundeffect();
                system("cls");
                system("color e0");
                BehindExit(true);
                break;
            }
        }

        if (counter == 1) {
            Arrcolors[0] = 8;
            Arrcolors[1] = 14;
            Arrcolors[2] = 14;
            Arrcolors[3] = 14;
            Arrcolors[4] = 14;
            Arrcolors[5] = 14;
        }

        else if (counter == 2) {
            Arrcolors[1] = 8;
            Arrcolors[0] = 14;
            Arrcolors[2] = 14;
            Arrcolors[3] = 14;
            Arrcolors[4] = 14;
            Arrcolors[5] = 14;
        }

        else if (counter == 3) {
            Arrcolors[2] = 8;
            Arrcolors[0] = 14;
            Arrcolors[1] = 14;
            Arrcolors[3] = 14;
            Arrcolors[4] = 14;
            Arrcolors[5] = 14;
        }

        else if (counter == 4) {
            Arrcolors[3] = 8;
            Arrcolors[0] = 14;
            Arrcolors[1] = 14;
            Arrcolors[2] = 14;
            Arrcolors[4] = 14;
            Arrcolors[5] = 14;
        }

        else if (counter == 5) {
            Arrcolors[4] = 8;
            Arrcolors[0] = 14;
            Arrcolors[1] = 14;
            Arrcolors[2] = 14;
            Arrcolors[3] = 14;
            Arrcolors[5] = 14;
        }

        else if (counter == 6) {
            Arrcolors[5] = 8;
            Arrcolors[0] = 14;
            Arrcolors[1] = 14;
            Arrcolors[2] = 14;
            Arrcolors[3] = 14;
            Arrcolors[4] = 14;
        }
    }
}

//Hàm nút Back
void Back() {
    for (;;) {
        char key = tolower(_getch());       //Nhận 1 ký tự bất kỳ từ bàn phím và luôn đưa nó về dạng chữ cái thường
        if (key == 27) { 
            playsoundeffect();  // Kí hiệu '27' là nút Escape
            system("cls");
            PrintMenu();
            return;
        }
    }
}

//Hàm in ra giới thiệu về nhóm và đồ án
void PrintAbout() {
    SetColor(15, 9);
    GotoXY(34, 10); cout << "    THIS IS THE GAME PROJECT FOR PROGRAMING TECHNIQUE COURSE,";
    GotoXY(34, 11); cout << "23CLC01, HCMUS UNDER THE GUIDANCE OF TEACHER DR.TRUONG TOAN THINH.";
    GotoXY(34, 13); cout << "         OUR GROUP MEMBER IS 04, INCLUDING 4 MEMBERS:";
    GotoXY(42, 14); cout << "     - LE QUANG PHUC         ID: 23127102";
    GotoXY(42, 15); cout << "     - DOAN THANH PHAT       ID: 23127241";
    GotoXY(42, 16); cout << "     - VUONG KHAI PHONG      ID: 23127553";
    GotoXY(42, 17); cout << "     - PHAM DUC TOAN         ID: 23127540";
    SetColor(15, 0);
    for (int i = 32; i <= 101; i++) {
        GotoXY(i, 18);
        cout << char(196);
    }
    GotoXY(55, 19); cout << "PRESS <ESC> TO BACK TO MENU";
}

//Hàm ẩn hiện dòng chữ "THANK YOU EVERYONE FOR PLAYING"
void BehindExit(bool show) {
    if (show) {
        SetColor(14, 0);
        GotoXY(88, 5);  cout << "████████╗██╗  ██╗ █████╗ ███╗   ██╗██╗  ██╗";
        GotoXY(88, 6);  cout << "╚══██╔══╝██║  ██║██╔══██╗████╗  ██║██║ ██╔╝";
        GotoXY(88, 7);  cout << "   ██║   ███████║███████║██╔██╗ ██║█████╔╝";
        GotoXY(88, 8);  cout << "   ██║   ██╔══██║██╔══██║██║╚██╗██║██╔═██╗";
        GotoXY(88, 9);  cout << "   ██║   ██║  ██║██║  ██║██║ ╚████║██║  ██╗";
        GotoXY(88, 10); cout << "   ╚═╝   ╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═══╝╚═╝  ╚═╝";

        GotoXY(95, 12); cout << " ██╗     ██╗ ██████╗ ██╗   ██╗";
        GotoXY(95, 13); cout << " ╚██╗   ██╔╝██╔═══██╗██║   ██║";
        GotoXY(95, 14); cout << "   ╚████╔╝  ██║   ██║██║   ██║";
        GotoXY(95, 15); cout << "    ╚██╔╝   ██║   ██║██║   ██║";
        GotoXY(95, 16); cout << "     ██║    ╚██████╔╝╚██████╔╝";
        GotoXY(95, 17); cout << "     ╚═╝     ╚═════╝  ╚═════╝";

        GotoXY(80, 19); cout << "███████╗██╗   ██╗███████╗██████╗ ██╗   ██╗ ██████╗ ███╗   ██╗███████╗";
        GotoXY(80, 20); cout << "██╔════╝██║   ██║██╔════╝██╔══██╗╚██╗ ██╔╝██╔═══██╗████╗  ██║██╔════╝";
        GotoXY(80, 21); cout << "█████╗  ██║   ██║█████╗  ██████╔╝ ╚████╔╝ ██║   ██║██╔██╗ ██║█████╗  ";
        GotoXY(80, 22); cout << "██╔══╝  ╚██╗ ██╔╝██╔══╝  ██╔══██╗  ╚██╔╝  ██║   ██║██║╚██╗██║██╔══╝  ";
        GotoXY(80, 23); cout << "███████╗ ╚████╔╝ ███████╗██║  ██║   ██║   ╚██████╔╝██║ ╚████║███████╗";
        GotoXY(80, 24); cout << "╚══════╝  ╚═══╝  ╚══════╝╚═╝  ╚═╝   ╚═╝    ╚═════╝ ╚═╝  ╚═══╝╚══════╝";

        GotoXY(100, 25); cout << "███████╗ ██████╗ ██████╗ ";
        GotoXY(100, 26); cout << "██╔════╝██╔═══██╗██╔══██╗";
        GotoXY(100, 27); cout << "█████╗  ██║   ██║██████╔╝";
        GotoXY(100, 28); cout << "██╔══╝  ██║   ██║██╔══██╗";
        GotoXY(100, 29); cout << "██║     ╚██████╔╝██║  ██║";
        GotoXY(100, 30); cout << "╚═╝      ╚═════╝ ╚═╝  ╚═╝";

        GotoXY(85, 32); cout << "██████╗ ██╗      █████╗ ██╗   ██╗██╗███╗   ██╗ ██████╗ ";
        GotoXY(85, 33); cout << "██╔══██╗██║     ██╔══██╗╚██╗ ██╔╝██║████╗  ██║██╔════╝ ";
        GotoXY(85, 34); cout << "██████╔╝██║     ███████║ ╚████╔╝ ██║██╔██╗ ██║██║  ███╗";
        GotoXY(85, 35); cout << "██╔═══╝ ██║     ██╔══██║  ╚██╔╝  ██║██║╚██╗██║██║   ██║";
        GotoXY(85, 36); cout << "██║     ███████╗██║  ██║   ██║   ██║██║ ╚████║╚██████╔╝";
        GotoXY(85, 37); cout << "╚═╝     ╚══════╝╚═╝  ╚═╝   ╚═╝   ╚═╝╚═╝  ╚═══╝ ╚═════╝ ";
    }
}

//Hàm in tiêu đề MENU GAME
void PrintTitle() {
    SetColor(14, 0);
    GotoXY(75, 1); cout << "███╗   ███╗███████╗███╗   ██╗██╗   ██╗     ██████╗  █████╗ ███╗   ███╗███████╗";
    GotoXY(75, 2); cout << "████╗ ████║██╔════╝████╗  ██║██║   ██║    ██╔════╝ ██╔══██╗████╗ ████║██╔════╝";
    GotoXY(75, 3); cout << "██╔████╔██║█████╗  ██╔██╗ ██║██║   ██║    ██║  ███╗███████║██╔████╔██║█████╗";
    GotoXY(75, 4); cout << "██║╚██╔╝██║██╔══╝  ██║╚██╗██║██║   ██║    ██║   ██║██╔══██║██║╚██╔╝██║██╔══╝";
    GotoXY(75, 5); cout << "██║ ╚═╝ ██║███████╗██║ ╚████║╚██████╔╝    ╚██████╔╝██║  ██║██║ ╚═╝ ██║███████╗";
    GotoXY(75, 6); cout << "╚═╝     ╚═╝╚══════╝╚═╝  ╚═══╝ ╚═════╝      ╚═════╝ ╚═╝  ╚═╝╚═╝     ╚═╝╚══════╝";
    ReadImageFromFie("MenuGameTitle.txt", 75, 1);
}

void StartGameMini(bool show) {
    if (show) {
        SetColor(14, 0);
        GotoXY(22, 40); cout << "  Start a new game!        ";
        GotoXY(22, 41); cout << "  All the previous progress";
        GotoXY(22, 42); cout << "  will be deleted          ";
    }
    else {
        SetColor(14, 14);
        GotoXY(28, 42); cout << "                 ";
        GotoXY(28, 43); cout << "                         ";
        GotoXY(28, 44); cout << "               ";
    }
}

void ResumeGameMini(bool show) {
    if (show) {
        SetColor(14, 0);
        GotoXY(22, 40); cout << "  Continue the game from   ";
        GotoXY(22, 41); cout << "  where you left of        ";
        GotoXY(22, 42); cout << "  Let's go, cheer up!      ";
    }
    else {
        SetColor(14, 14);
        GotoXY(28, 42); cout << "                      ";
        GotoXY(28, 43); cout << "                 ";
        GotoXY(28, 44); cout << "                   ";
    }
}

void HighscoresMini(bool show) {
    if (show) {
        SetColor(14, 0);
        GotoXY(22, 40); cout << "  Ranking of players with ";
        GotoXY(22, 41); cout << "  the highest score       ";
        GotoXY(22, 42); cout << "  Let's keep trying!      ";
    }
    else {
        SetColor(14, 14);
        GotoXY(22, 42); cout << "                       ";
        GotoXY(22, 43); cout << "                 ";
        GotoXY(22, 44); cout << "                  ";
    }
}

void AboutMini(bool show) {
    if (show) {
        SetColor(14, 0);
        GotoXY(22, 40); cout << "  Learn more about the    ";
        GotoXY(22, 41); cout << "  publisher here!         ";
    }
    else {
        SetColor(14, 14);
        GotoXY(28, 42); cout << "                    ";
        GotoXY(28, 43); cout << "               ";
    }
}

void OptionsMini(bool show) {
    if (show) {
        SetColor(14, 0);
        GotoXY(22, 40); cout << "  Setting more about the  ";
        GotoXY(22, 41); cout << "  game as you want!       ";
        GotoXY(22, 42); cout << "                          ";
    }
    else {
        SetColor(14, 14);
        GotoXY(28, 42); cout << "                      ";
        GotoXY(28, 43); cout << "                 ";
    }
}

void ExitGameMini(bool show) {
    if (show) {
        SetColor(14, 0);
        GotoXY(22, 40); cout << "  Dont't worry, auto save  ";
        GotoXY(22, 41); cout << "  feature is on            ";
        GotoXY(22, 42); cout << "  Thank you!               ";
    }
    else {
        SetColor(14, 14);
        GotoXY(28, 42); cout << "                       ";
        GotoXY(28, 43); cout << "             ";
        GotoXY(28, 44); cout << "          ";
    }
}
