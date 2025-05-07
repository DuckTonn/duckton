#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "Graphic.h"
#include "Menu.h"
#include <iostream>
#include <thread>
#include "Sound.h"
#define MAX_SIZE_SNAKE 32
#define MAX_SIZE_FOOD 4
#define MAX_SPEED 51
struct Point {
	int x;
	int y;
	int background;
};
struct PlayerInformation {
	char Name[16];
	int Score;
	int State;
	int Moving;
	int char_lock;
	int Speed;
	int food_index;
	int size_snake;
	int Gate_State;
	int Level;
	int Time;
	Point Snake[32];
	Point Food[4];
	Point levelGate[6];
};

//Các biến liên quan đến người chơi (thay đổi tùy từng người chơi)
extern Point snake[32]; //Mảng để chứa con rắn
extern Point food[4]; // Mảng chứa thức ăn
extern Point LevelGate[6]; //Cổng rắn đi qua màn tiếp theo
extern int CHAR_LOCK;//Biến sử dụng để chỉ hướng con rắn không thể di chuyển (Hướng ngược lại với hướng di chuyển hiện tại của con rắn)
extern int MOVING; //Biến sử dụng để chỉ hướng di chuyển hiện tại của con rắn
extern int SPEED;// Biến biểu thị tốc độ hiện tại của con rắn
extern int SCORE; // Điểm của người chơi
extern int FOOD_INDEX; // Chỉ số hiện tại của thức ăn
extern int SIZE_SNAKE; // Chiều dài của rắn hiện tại
extern int STATE; // Trạng thái rắn (sống hoặc chết)
extern int GATE_STATE;// Trạng thái của cổng
extern int LEVEL;// Level hiện tại
extern int TIME;//Thời gian người chơi hoàn thành game (Tính theo giây)
extern char NAME[16];

//Các biến liên quan đến map (mặc định)
const int StudentCode[32] = { 2,3,1,2,7,2,4,1,2,3,1,2,7,1,0,2,2,3,1,2,7,5,4,0,2,3,1,2,7,4,5,3 };
extern Point map[20000]; // Map hiện tại
extern Point image[20000];
extern Point MapGate[2][2];
extern Point bird[5][11];
extern int sizeImage;
extern int sizeMap;// Kích thước của map
extern int HEIGH_CONSOLE, WIDTH_CONSOLE;// Chiều dài và rộng của khung trò chơi
extern int WALL_COLOR;
extern clock_t begin_t, end_t;// 2 biến để tính thời gian người chơi hoàn thành game
extern int Pause_Time;
extern int Snake_Color;
extern int Music;

//Các hàm về map
bool IsValid(int x, int y);
void GenerateFood();
void GenerateGate();
void InitMap(int x, int y, int width, int height);
void InitBird(int num, int x, int y);
void DeleteLevelGate();
void DeleteMapGate();
void DeleteBird(int num);
void BirdMove(int Num);
void ResetData();
void NextLevel();
void ReadMapFromFie(const char* filename, int x, int y);

//Các hàm về con rắn
void MoveRight();
void MoveLeft();
void MoveDown();
void MoveUp();
void Eat();
bool MoveInWater();
bool Touch_Snake_Body(const char moving);
bool TouchMap(const char moving);
bool TouchGate(const char moving);
bool TouchBird(const char moving);
void ProcessDead();

//Các hàm về vẽ chi tiết game lên màn hình
void DrawGate();
void EraseGate();
void DrawFood(const char* str);
void DrawSnake(const char* str);
void DrawMap();
void DrawBoard(int x, int y, int width, int height);
void DrawBird(int Num);
void EraseBird(int Num);
void Information(int x, int y);
void PrintNumber(int num, int x, int y, int background);
void PrintFoodBar();

//Các hàm về start, load, pause, save và exit game
void StartNewGame();
void StartResumeGame();
void StartLoadGame(const char* PlayerName);
void ExitGame(thread& t);
void PauseGame();
void ResumeGame();
void RestartGame();
void SaveGame(const char* FileName, const char* PlayerName);
void SaveLoadGame();
void SaveResumeGame();
void LoadGame(const char* FileName, const char* PlayerName);
void LoadResumeGame();
void PlayAgainMenu();

//Các mini menu game trong gameplay
void PrintStartGameOption(int x, int y);
void SaveGameMenu();
void SaveLoadGameMenu();
void OptionMenu(int x, int y);

//Các hàm về highscore
int CheckHighscore(PlayerInformation A);
void UpdateHightScore(PlayerInformation Player);
void InitPlayerInfo(PlayerInformation& A, const char* PlayerName);
void LoadPlayerInfo(PlayerInformation A);

//Luồng để chạy trò chơi
void ThreadFunc();

//Các hàm khác
void DrawChangeLevelEffect();
void VictoryEffect();
bool CheckValidName(const char* Name);