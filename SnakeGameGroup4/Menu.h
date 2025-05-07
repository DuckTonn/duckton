#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <Windows.h>
#include <conio.h>
#include <random>
#include <string>
#include "Gameplay.h"
#include "Graphic.h"
#include "Images.h"
#include "Sound.h"
using namespace std;

void SetTitle();
void PrintMenu();
void PrintOptions();
void Back();
void PrintAbout();
void BehindExit(bool show);
void PrintTitle();
void StartGameMini(bool show);
void ResumeGameMini(bool show);
void HighscoresMini(bool show);
void AboutMini(bool show);
void OptionsMini(bool show);
void ExitGameMini(bool show);