#pragma once
#pragma execution_character_set("utf-8")
#include <Windows.h>
#include <iostream>
#include <stdio.h>
#include <conio.h>
#include <ctime>
using namespace std;

void GotoXY(int x, int y);
void FixConsoleWindow();
void DisableSelection();
void SetScreenBufferSize(SHORT width, SHORT height);
void SetWindowSize(SHORT width, SHORT height);
void FixConsole();
void SetColor(int backgound_color, int text_color);//text+background
void ShowConsoleCursor(bool showFlag);


void setConsoleFont(const wchar_t* fontName, int fontSizeY);
void setDefaultConsoleFont();