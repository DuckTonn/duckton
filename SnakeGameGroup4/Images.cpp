#include "Images.h"
Point image[20000];
int sizeImage;

void showImages(const char* filename, int pos_x, int pos_y, bool show) {
    ifstream file;
    file.open(filename, istream::in);
    if (!file.is_open()) {
        cout << "Không th? m? file.\n";
        return;
    }
    string line;
    while (getline(file, line)) {
        GotoXY(pos_x, pos_y);
        DrawImages(line, show);
        pos_y++;
    }
    file.close();
}

void DrawImages(const string& temp, bool show) {
    SetConsoleOutputCP(437);
    if (show) {
        for (char c : temp) {
            switch (c)
            {
            case'0':
                SetColor(0, 0);
                break;
            case'1':
                SetColor(1, 1);
                break;
            case'2':
                SetColor(2, 2);
                break;
            case'3':
                SetColor(3, 3);
                break;
            case'4':
                SetColor(4, 4);
                break;
            case'5':
                SetColor(5, 5);
                break;
            case'6':
                SetColor(6, 6);
                break;
            case'7':
                SetColor(7, 7);
                break;
            case'8':
                SetColor(8, 8);
                break;
            case'9':
                SetColor(9, 9);
                break;
            case'A':
                SetColor(10, 10);
                break;
            case'B':
                SetColor(11, 11);
                break;
            case'C':
                SetColor(12, 12);
                break;
            case'D':
                SetColor(13, 13);
                break;
            case'E':
                SetColor(14, 14);
                break;
            case'F':
                SetColor(15, 15);
                break;
            }
            if (c > 32) {
                cout << char(219);
            }
            else {
                cout << c;
            }

        }
    }
    else if (show == false) {
        for (char c : temp) {
            SetColor(15, 15);
            if (c > 32) {
                cout << " ";
            }
            else {
                cout << c;
            }
        }
    }
}

int HexToDec(char c) {
    int dec = c;
    if (dec >= 'A' && dec <= 'F')
        dec = dec - 55;
    if (dec >= '0' && dec <= '9')
        dec = dec - 48;
    return dec;
}

void ReadImageFromFie(const char* filename, int pos_x, int pos_y) {
    sizeImage = 0;
    FILE* f;
    fopen_s(&f, filename, "r");
    if (f == NULL) {
        return;
    }
    char c;
    GotoXY(pos_x, pos_y);
    int ini_x = pos_x;
    while ((c = fgetc(f)) != EOF)
    {
        if (c == '\n') {
            pos_y += 1;
            pos_x = ini_x;
            GotoXY(pos_x, pos_y);
        }
        else if (c != ' ') {
            image[sizeImage++].background = HexToDec(c);
            SetColor(image[sizeImage - 1].background, 0);
            GotoXY(pos_x, pos_y);
            cout << " ";
            pos_x++;
        }
        else {
            pos_x++;
            SetColor(15, 0);
        }
    }
    fclose(f);
    SetColor(14, 0);
}

void HighScoreListOut(int pos_x, int pos_y) {
    PlayerInformation top[10];
    FILE* f;
    fopen_s(&f, "HighScore.dat", "rb");
    if (f == NULL) {
        return;
    }
    for (int i = 0; i < 10 && feof(f) == false; i++) {
        fread(&top[i], sizeof(PlayerInformation), 1, f);
    }
    fclose(f);
    SetColor(15, 4);
    GotoXY(pos_x, pos_y - 1); cout << "PLAYER NAME";
    GotoXY(pos_x + 20, pos_y - 1); cout << "SCORE\t\tTIME\t\tLEVEL";
    for (int i = 0; i < 10; i++) {
        SetColor(15, 0);
        GotoXY(pos_x, pos_y + i); cout << i + 1 << ". " << top[i].Name;
        GotoXY(pos_x + 22, pos_y + i); cout << top[i].Score;
        GotoXY(pos_x + 38, pos_y + i); cout << top[i].Time;
        GotoXY(pos_x + 55, pos_y + i); cout << top[i].Level;
    }
    return;
}
