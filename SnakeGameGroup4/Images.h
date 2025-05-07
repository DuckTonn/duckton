#pragma once
#include <iostream>
#include <Windows.h>
#include <fstream>
#include <string>
#include "Graphic.h"
#include "Gameplay.h"

using namespace std;

void showImages(const char* filename, int pos_x, int pos_y, bool show);
void DrawImages(const string& temp, bool show);
int HexToDec(char c);
void ReadImageFromFie(const char* filename, int pos_x, int pos_y);
void HighScoreListOut(int pos_x, int pos_y);
