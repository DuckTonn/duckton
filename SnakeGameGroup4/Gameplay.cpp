#include "Gameplay.h"
//Các biến cờ để kiểm soát luồng
std::atomic<bool> shouldTerminate(false); // Biến cờ kiểm soát kết thúc luồng
std::atomic<bool> shouldPause(false); // Biến cờ kiểm soát tạm dừng luồng

//Các biến liên quan đến người chơi (thay đổi tùy từng người chơi)
Point snake[32]; //Mảng để chứa con rắn
Point food[4]; // Mảng chứa thức ăn
Point LevelGate[6]; //Cổng rắn đi qua màn tiếp theo
int CHAR_LOCK;//Biến sử dụng để chỉ hướng con rắn không thể di chuyển (Hướng ngược lại với hướng di chuyển hiện tại của con rắn)
int MOVING; //Biến sử dụng để chỉ hướng di chuyển hiện tại của con rắn
int SPEED = 15;// Biến biểu thị tốc độ hiện tại của con rắn
int SCORE; // Điểm của người chơi
int FOOD_INDEX; // Chỉ số hiện tại của thức ăn
int SIZE_SNAKE; // Chiều dài của rắn hiện tại
int STATE; // Trạng thái rắn (sống hoặc chết)
int GATE_STATE;// Trạng thái của cổng
int LEVEL;// Level hiện tại
int TIME;//Thời gian người chơi hoàn thành game (Tính theo giây)
char NAME[16];

//Các biến liên quan đến map (mặc định)
Point map[20000]; // Map hiện tại
Point MapGate[2][2];
Point bird[5][11];
clock_t begin_t, end_t;// 2 biến để tính thời gian người chơi hoàn thành game
int sizeMap;// Kích thước của map
int HEIGH_CONSOLE = 36, WIDTH_CONSOLE = 150;// Chiều dài và rộng của khung trò chơi
int WALL_COLOR = 8;
int Pause_Time = 0;
int Snake_Color = 2;
int Music = 0;
//Các hàm về map
bool IsValid(int x, int y) {
	for (int i = 0; i < SIZE_SNAKE; i++) {
		if (snake[i].x == x && snake[i].y == y) {
			return false;
		}
	}
	for (int i = 0; i < sizeMap; i++) {
		if (map[i].x == x && map[i].y == y) {
			return false;
		}
	}
	return true;
}
void GenerateFood() {
	int x, y;
	srand(time(NULL));
	for (int i = 0; i < MAX_SIZE_FOOD; i++) {
		do {
			x = rand() % (WIDTH_CONSOLE - 4) + 3;
			y = rand() % (HEIGH_CONSOLE - 2) + 13;

		} while (IsValid(x, y) == false);
		food[i] = { x,y };
	}
}
void GenerateGate() {
	int x, y;
	srand(time(NULL));
	do {
		x = rand() % (WIDTH_CONSOLE - 5) + 5;
		y = rand() % (HEIGH_CONSOLE - 2) + 13;
	} while (IsValid(x, y) == false || IsValid(x, y + 1) == false || IsValid(x, y - 1) == false || IsValid(x + 1, y) == false || IsValid(x + 1, y + 1) == false || IsValid(x + 1, y - 1) == false);

	LevelGate[0] = { x,y };
	LevelGate[1] = { x,y + 1 };
	LevelGate[2] = { x,y - 1 };
	LevelGate[3] = { x + 1,y };
	LevelGate[4] = { x + 1, y - 1 };
	LevelGate[5] = { x + 1,y + 1 };
	food[FOOD_INDEX] = { 0,0 };
}
void InitMap(int x, int y, int width, int height) {
	sizeMap = 0;
	switch (LEVEL)
	{
	default:
		break;
	case 1: {
		ReadMapFromFie("map1.txt", 0, 13);
		break;
	}
	case 2: {
		ReadMapFromFie("map2.txt", 0, 13);
		break;
	}
	case 3: {
		ReadMapFromFie("map3.txt", 0, 13);
		break;
	}
	case 4: {
		ReadMapFromFie("map4.txt", 0, 13);
		break;
	}
	case 5: {
		ReadMapFromFie("map5.txt", 0, 13);
		MapGate[0][0] = { x + 7, y + 6 };
		MapGate[0][1] = { x + 140, y + 32 };
		break;
	}
	case 6: {
		MapGate[0][0] = { x + 9, y + 7 };
		MapGate[0][1] = { x + 88, y + 7 };
		MapGate[1][0] = { x + 60, y + 28 };
		MapGate[1][1] = { x + 140, y + 28 };
		ReadMapFromFie("map6.txt", 0, 13);
		InitBird(0, x + 10, y + 10);
		InitBird(1, x + 70, y + 20);
		InitBird(2, x + 110, y + 15);
		break;
	}
	case 7: {
		DeleteBird(3);
		DeleteMapGate();
		break;
	}
	}
}
void InitBird(int num, int x, int y) {
	bird[num][0] = { x ,y };
	bird[num][1] = { x + 1,y };
	bird[num][2] = { x + 2,y };
	bird[num][3] = { x + 3,y };
	bird[num][4] = { x + 4,y };
	bird[num][5] = { x + 5,y };
	bird[num][6] = { x + 6,y };
	bird[num][7] = { x + 3,y - 1 };
	bird[num][8] = { x + 4,y - 1 };
	bird[num][9] = { x + 3,y + 1 };
	bird[num][10] = { x + 4, y + 1 };
}
void DeleteLevelGate() {
	for (int i = 0; i < 6; i++) {
		LevelGate[i].x = 0;
		LevelGate[i].y = 0;
	}
}
void DeleteMapGate() {
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 2; j++) {
			MapGate[i][i] = { 0,0,14 };
		}
	}
}
void DeleteBird(int num) {
	for (int i = 0; i < num; i++) {
		bird[i][0] = { 0 ,0 };
		bird[i][1] = { 0 ,0 };
		bird[i][2] = { 0 ,0 };
		bird[i][3] = { 0 ,0 };
		bird[i][4] = { 0 ,0 };
		bird[i][5] = { 0 ,0 };
		bird[i][6] = { 0 ,0 };
		bird[i][7] = { 0 ,0 };
		bird[i][8] = { 0 ,0 };
		bird[i][9] = { 0 ,0 };
		bird[i][10] = { 0 ,0 };
	}
}
void BirdMove(int Num) {
	int moveleft = true;
	for (int i = 0; i < Num; i++) {
		if (bird[i][0].x < bird[i][1].x) {
			bird[i][6] = bird[i][5];
			bird[i][5] = bird[i][4];
			bird[i][4] = bird[i][3];
			bird[i][3] = bird[i][2];
			bird[i][2] = bird[i][1];
			bird[i][1] = bird[i][0];
			bird[i][8] = bird[i][7];
			bird[i][10] = bird[i][9];
			bird[i][0].x--;
			bird[i][7].x--;
			bird[i][9].x--;
			for (int j = 0; j < sizeMap; j++) {
				if (bird[i][0].x == map[j].x && bird[i][0].y == map[j].y) {
					bird[i][0].x = map[j].x + 6;
					bird[i][1].x = map[j].x + 5;
					bird[i][2].x = map[j].x + 4;
					bird[i][3].x = map[j].x + 3;
					bird[i][4].x = map[j].x + 2;
					bird[i][5].x = map[j].x + 1;
					bird[i][7].x = map[j].x + 3;
					bird[i][8].x = map[j].x + 2;
					bird[i][9].x = map[j].x + 3;
					bird[i][10].x = map[j].x + 2;
				}
			}
		}
		if (bird[i][0].x > bird[i][1].x) {
			bird[i][6] = bird[i][5];
			bird[i][5] = bird[i][4];
			bird[i][4] = bird[i][3];
			bird[i][3] = bird[i][2];
			bird[i][2] = bird[i][1];
			bird[i][1] = bird[i][0];
			bird[i][8] = bird[i][7];
			bird[i][10] = bird[i][9];
			bird[i][0].x++;
			bird[i][7].x++;
			bird[i][9].x++;
			for (int j = 0; j < sizeMap; j++) {
				if (bird[i][0].x == map[j].x && bird[i][0].y == map[j].y) {
					bird[i][0].x = map[j].x - 6;
					bird[i][1].x = map[j].x - 5;
					bird[i][2].x = map[j].x - 4;
					bird[i][3].x = map[j].x - 3;
					bird[i][4].x = map[j].x - 2;
					bird[i][5].x = map[j].x - 1;
					bird[i][7].x = map[j].x - 3;
					bird[i][8].x = map[j].x - 2;
					bird[i][9].x = map[j].x - 3;
					bird[i][10].x = map[j].x - 2;
				}
			}
		}
	}
}
void ResetData() {
	//Khởi tạo các dữ liệu ban đầu cho game
	CHAR_LOCK = 'A', MOVING = 'D'; FOOD_INDEX = 0, LEVEL = 1,
		SIZE_SNAKE = 6, SCORE = 0; GATE_STATE = 0; TIME = 0, Pause_Time = 0;
	strcpy(NAME, "NoName");
	//Khởi tạo giá trị tọa độ ban đầu cho con rắn
	snake[5] = { 10, 15 }; snake[4] = { 11, 15 };
	snake[3] = { 12, 15 }; snake[2] = { 13, 15 };
	snake[1] = { 14, 15 }; snake[0] = { 15, 15 };
	DeleteLevelGate();
	InitMap(0, 12, 150, 36);
	GenerateFood();//Create food array
}
void NextLevel() {
	EraseGate();
	PauseGame();
	GATE_STATE = 0;
	DeleteLevelGate();
	FOOD_INDEX = 0;
	LEVEL++;
	for (int i = 0; i < SIZE_SNAKE; i++) {
		snake[i].x = 10;
		snake[i].y = 15;
	}
	if (SPEED == MAX_SPEED) {
		SPEED = 10;
		SIZE_SNAKE = 6;
	}
	else SPEED++;
	if (LEVEL < 7) {
		DrawChangeLevelEffect();
		InitMap(0, 12, 150, 36);
		GenerateFood();
		Information(162, 12);
		DrawMap();
		Pause_Time += 8;
	}
	if (LEVEL == 7) {
		STATE = 3;
	}
}
void ReadMapFromFie(const char* filename, int x, int y) {
	sizeMap = 0;
	FILE* f = fopen(filename, "r");
	char c;
	int ini_x = x;
	while ((c = fgetc(f)) != EOF)
	{
		if (c == '\n') {
			x = ini_x;
			y += 1;
		}
		else if (c != ' ') {
			map[sizeMap++] = { x, y , HexToDec(c) };
			x += 1;
		}
		else
			x += 1;
	}
	fclose(f);
}

//Các hàm về con rắn
void MoveRight() {
	if (TouchMap('R') || TouchGate('R') || Touch_Snake_Body('R') || TouchBird('R')) {
		ProcessDead();
	}
	else {
		for (int i = SIZE_SNAKE; i > 0; i--) {
			snake[i].x = snake[i - 1].x;
			snake[i].y = snake[i - 1].y;
		}
		if (snake[0].x + 1 == food[FOOD_INDEX].x && snake[0].y == food[FOOD_INDEX].y) {
			Eat();
		}
		if (snake[0].x == LevelGate[0].x && snake[0].y == LevelGate[0].y) {
			if (snake[SIZE_SNAKE - 1].x == LevelGate[0].x && snake[SIZE_SNAKE - 1].y == LevelGate[0].y) {
				NextLevel();
			}
		}
		else if (snake[0].x == MapGate[0][1].x && snake[0].y == MapGate[0][1].y) {
			snake[0].x = MapGate[0][0].x;
			snake[0].y = MapGate[0][0].y;
		}
		else if (snake[0].x == MapGate[1][1].x && snake[0].y == MapGate[1][1].y) {
			snake[0].x = MapGate[1][0].x;
			snake[0].y = MapGate[1][0].y;
		}
		else
			snake[0].x++;
	}
	if (MoveInWater() == true && STATE == 1) {
		SPEED /= 2.0;
		STATE = 2;
	}
	if (MoveInWater() == false && STATE == 2) {
		SPEED = int(SPEED * 2);
		STATE = 1;
	}
}
void MoveLeft() {
	if (TouchMap('L') || TouchGate('L') || Touch_Snake_Body('L') || TouchBird('L')) {
		ProcessDead();
	}
	else {
		for (int i = SIZE_SNAKE; i > 0; i--) {
			snake[i].x = snake[i - 1].x;
			snake[i].y = snake[i - 1].y;
		}
		if (snake[0].x - 1 == food[FOOD_INDEX].x && snake[0].y == food[FOOD_INDEX].y) {
			Eat();
		}
		if (snake[0].x == MapGate[0][0].x && snake[0].y == MapGate[0][0].y) {
			snake[0].x = MapGate[0][1].x;
			snake[0].y = MapGate[0][1].y;
		}
		else if (snake[0].x == MapGate[1][0].x && snake[0].y == MapGate[1][0].y) {
			snake[0].x = MapGate[1][1].x;
			snake[0].y = MapGate[1][1].y;
		}
		else
			snake[0].x--;
	}
	if (MoveInWater() == true && STATE == 1) {
		SPEED /= 2.0;
		STATE = 2;
	}
	if (MoveInWater() == false && STATE == 2) {
		SPEED = int(SPEED * 2);
		STATE = 1;
	}
}
void MoveDown() {
	if (TouchMap('D') || TouchGate('D') || Touch_Snake_Body('D') || TouchBird('L')) {
		ProcessDead();
	}
	else {
		if (snake[0].x == food[FOOD_INDEX].x && snake[0].y + 1 == food[FOOD_INDEX].y) {
			Eat();
		}
		for (int i = SIZE_SNAKE; i > 0; i--) {
			snake[i].x = snake[i - 1].x;
			snake[i].y = snake[i - 1].y;
		}
		snake[0].y++;
	}
	if (MoveInWater() == true && STATE == 1) {
		SPEED /= 2.0;
		STATE = 2;
	}
	if (MoveInWater() == false && STATE == 2) {
		SPEED = int(SPEED * 2);
		STATE = 1;
	}
}
void MoveUp() {
	if (TouchMap('U') || TouchGate('U') || Touch_Snake_Body('U') || TouchBird('U')) {
		ProcessDead();
	}
	else {
		if (snake[0].x == food[FOOD_INDEX].x && snake[0].y - 1 == food[FOOD_INDEX].y) {
			Eat();
		}
		for (int i = SIZE_SNAKE; i > 0; i--) {
			snake[i].x = snake[i - 1].x;
			snake[i].y = snake[i - 1].y;
		}
		snake[0].y--;
	}
	if (MoveInWater() == true && STATE == 1) {
		SPEED /= 2.0;
		STATE = 2;
	}
	if (MoveInWater() == false && STATE == 2) {
		SPEED = int(SPEED * 2);
		STATE = 1;
	}
}
void Eat() {
	soundeat();
	if (FOOD_INDEX == MAX_SIZE_FOOD - 1) {
		SCORE++;
		SIZE_SNAKE++;
		GenerateGate();
		GATE_STATE = 1;
	}
	else {
		SIZE_SNAKE++;
		SCORE++;
		FOOD_INDEX++;
	}
	PrintFoodBar();
	PrintNumber(SCORE, 178, 21, 15);
}
bool MoveInWater() {
	for (int i = 0; i < sizeMap; i++) {
		if (snake[0].x == map[i].x && snake[0].y == map[i].y && map[i].background == 9) {
			return true;
		}
	}
	return false;
}
bool Touch_Snake_Body(const char moving) {
	for (int i = 1; i < SIZE_SNAKE - 1; i++) {
		switch (moving)
		{
		default:
			break;
		case 'R': {
			if (snake[0].x + 1 == snake[i].x && snake[0].y == snake[i].y)
				return true;
			break;
		}
		case 'L': {
			if (snake[0].x - 1 == snake[i].x && snake[0].y == snake[i].y)
				return true;
			break;
		}
		case 'U': {
			if (snake[0].x == snake[i].x && snake[0].y - 1 == snake[i].y)
				return true;
			break;
		}
		case 'D': {
			if (snake[0].x == snake[i].x && snake[0].y + 1 == snake[i].y)
				return true;
			break;
		}
		}
	}
	return false;
}
bool TouchMap(const char moving) {
	for (int i = 0; i < sizeMap; i++) {
		switch (moving)
		{
		default:
			break;
		case 'R': {
			if (snake[0].x + 1 == map[i].x && snake[0].y == map[i].y && (map[i].background != 9 && map[i].background != 7))
			{
				for (int i = 0; i < 2; i++) {
					if (snake[0].x == MapGate[i][1].x && snake[0].y == MapGate[i][1].y)
						return false;
				}
				return true;
			}
			break;
		}
		case 'L': {
			if (snake[0].x - 1 == map[i].x && snake[0].y == map[i].y && (map[i].background != 9 && map[i].background != 7))
			{
				for (int i = 0; i < 2; i++) {
					if (snake[0].x == MapGate[i][0].x && snake[0].y == MapGate[i][0].y)
						return false;
				}
				return true;
			}
			break;
		}
		case 'U': {
			if (snake[0].x == map[i].x && snake[0].y - 1 == map[i].y && (map[i].background != 9 && map[i].background != 7))
				return true;
			break;
		}
		case 'D': {
			if (snake[0].x == map[i].x && snake[0].y + 1 == map[i].y && (map[i].background != 9 && map[i].background != 7))
				return true;
			break;
		}
		}
	}
	return false;
}
bool TouchGate(const char moving) {
	for (int i = 0; i < 6; i++) {
		switch (moving)
		{
		default:
			break;
		case 'R': {
			if (snake[0].x + 1 == LevelGate[i].x && snake[0].y == LevelGate[i].y && i != 0 && i != 3)
				return true;
			break;
		}
		case 'L': {
			if (snake[0].x - 1 == LevelGate[i].x && snake[0].y == LevelGate[i].y)
				return true;
			break;
		}
		case 'U': {
			if (snake[0].x == LevelGate[i].x && snake[0].y - 1 == LevelGate[i].y)
				return true;
			break;
		}
		case 'D': {
			if (snake[0].x == LevelGate[i].x && snake[0].y + 1 == LevelGate[i].y)
				return true;
			break;
		}
		}
	}
	return false;
}
bool TouchBird(const char moving) {
	switch (moving)
	{
	default:
		break;
	case 'R': {
		for (int j = 0; j < 5; j++) {
			for (int z = 0; z < 11; z++) {
				if (snake[0].x + 1 == bird[j][z].x && snake[0].y == bird[j][z].y)
					return true;
				for (int i = 0; i < SIZE_SNAKE; i++) {
					if (bird[j][0].x + 1 == snake[i].x && bird[j][0].y == snake[i].y)
						return true;
				}
			}
		}
		break;
	}
	case 'L': {
		for (int j = 0; j < 5; j++) {
			for (int z = 0; z < 11; z++) {
				if (snake[0].x - 1 == bird[j][z].x && snake[0].y == bird[j][z].y)
					return true;
				for (int i = 0; i < SIZE_SNAKE; i++) {
					if (bird[j][0].x - 1 == snake[i].x && bird[j][0].y == snake[i].y)
						return true;
				}
			}
		}
		break;
	}
	case 'U': {
		for (int j = 0; j < 5; j++) {
			for (int z = 0; z < 11; z++) {
				if (snake[0].x == bird[j][z].x && snake[0].y - 1 == bird[j][z].y)
					return true;
				for (int i = 0; i < SIZE_SNAKE; i++) {
					if (bird[j][0].x == snake[i].x && bird[j][0].y - 1 == snake[i].y)
						return true;
				}
			}
		}
		break;
	}
	case 'D': {
		for (int j = 0; j < 5; j++) {
			for (int z = 0; z < 11; z++) {
				if (snake[0].x == bird[j][z].x && snake[0].y + 1 == bird[j][z].y)
					return true;
				for (int i = 0; i < SIZE_SNAKE; i++) {
					if (bird[j][0].x == snake[i].x && bird[j][0].y + 1 == snake[i].y)
						return true;
				}
			}
		}
		break;
	}
	}
	return false;
}
void ProcessDead() {
	sounddead();
	STATE = 0;
}

//Các hàm về vẽ chi tiết game lên màn hình
void DrawGate() {
	srand(time(NULL));
	int Color_background;
	do {
		Color_background = rand() % 16;
	} while (Color_background == 14);
	SetColor(Color_background, 0);
	for (int i = 1; i < 6; i++) {
		GotoXY(LevelGate[i].x, LevelGate[i].y);
		cout << " ";
	}
	SetColor(14, 0);
}
void EraseGate() {
	SetColor(14, 0);
	for (int i = 1; i < 6; i++) {
		for (int j = 0; j < sizeMap; j++) {
			SetColor(14, 0);
			if (LevelGate[i].x == map[j].x && LevelGate[i].y == map[j].y) {
				SetColor(map[j].background, 0);
			}
		}
		GotoXY(LevelGate[i].x, LevelGate[i].y);
		cout << " ";
	}
	SetColor(14, 0);
}
void DrawFood(const char* str) {
	SetColor(14, 4);
	GotoXY(food[FOOD_INDEX].x, food[FOOD_INDEX].y);
	printf(str);
	SetColor(14, 0);
}
void DrawSnake(const char* str) {
	SetColor(Snake_Color, 0);
	if (str == " ") {
		SetColor(14, 0);
		for (int i = 0; i < SIZE_SNAKE; i++) {
			GotoXY(snake[i].x, snake[i].y);
			cout << " ";
		}
	}
	else {
		SetColor(Snake_Color, 0);
		for (int i = 0; i < SIZE_SNAKE; i++) {
			GotoXY(snake[i].x, snake[i].y);
			cout << StudentCode[i % 32];
		}
	}
	if (str == " ") {
		for (int i = 0; i < sizeMap; i++) {
			if (snake[SIZE_SNAKE - 1].x == map[i].x && snake[SIZE_SNAKE - 1].y == map[i].y) {
				SetColor(map[i].background, 0);
				GotoXY(snake[SIZE_SNAKE - 1].x, snake[SIZE_SNAKE - 1].y);
				cout << str;
			}
		}
	}
	if (STATE == 0) {
		SetColor(12, 0);
		GotoXY(snake[0].x, snake[0].y);
		cout << "X";
		SetColor(Snake_Color, 0);
		for (int i = 1; i < SIZE_SNAKE; i++) {
			GotoXY(snake[i].x, snake[i].y);
			cout << StudentCode[i];
		}
	}
	//	else {
	//		SetColor(12, 0);
	//		GotoXY(snake[0].x, snake[0].y);
	//		cout << " ";
	//		SetColor(0 * 16 + 2, 0);
	//		for (int i = 1; i < SIZE_SNAKE; i++) {
	//			GotoXY(snake[i].x, snake[i].y);
	//			cout << " ";
	//		}
	//		Count++;
	//	}
	//}
	SetColor(14, 0);
}
void DrawMap() {
	if (LEVEL == 1)
		ReadImageFromFie("Level1.txt", 0, 13);
	if (LEVEL == 2)
		ReadImageFromFie("Level2.txt", 0, 13);
	if (LEVEL == 3)
		ReadImageFromFie("Level3.txt", 0, 13);
	if (LEVEL == 4)
		ReadImageFromFie("Level4.txt", 0, 13);
	if (LEVEL == 5)
		ReadImageFromFie("Level5.txt", 0, 13);
	if (LEVEL == 6)
		ReadImageFromFie("Level6.txt", 0, 13);

	Sleep(1000);
	ReadImageFromFie("ClearMap.txt", 0, 13);
	for (int i = 0; i < sizeMap; i++) {
		GotoXY(map[i].x, map[i].y);
		SetColor(map[i].background, 0);
		cout << " ";
	}
}
void DrawBoard(int x, int y, int width, int height) {
	for (int i = 0; i < width; i++) {
		GotoXY(x + i, y); SetColor(8, 0); cout << " ";
		GotoXY(x + i, y + height); SetColor(8, 0); cout << " ";
	}
	for (int i = 1; i < height; i++) {
		GotoXY(x, y + i); SetColor(8, 0); cout << " ";
		GotoXY(x + 1, y + i); SetColor(8, 0); cout << " ";
		GotoXY(x + width - 2, y + i); SetColor(8, 0); cout << " ";
		GotoXY(x + width - 1, y + i); SetColor(8, 0); cout << " ";
	}
}
void DrawBird(int Num) {
	SetColor(14, 5);
	for (int i = 0; i < Num; i++) {
		for (int j = 0; j < 11; j++) {
			if (bird[i][0].x < bird[i][1].x) {
				if (j == 0) {
					GotoXY(bird[i][j].x, bird[i][j].y);
					cout << "-";
				}
				if (j == 1) {
					GotoXY(bird[i][j].x, bird[i][j].y);
					cout << "<";
				}
				if (j == 2) {
					GotoXY(bird[i][j].x, bird[i][j].y);
					cout << " ";
				}
				if (j == 3) {
					GotoXY(bird[i][j].x, bird[i][j].y);
					cout << " ";
				}
				if (j == 4) {
					GotoXY(bird[i][j].x, bird[i][j].y);
					cout << " ";
				}
				if (j == 5) {
					GotoXY(bird[i][j].x, bird[i][j].y);
					cout << "{";
				}
				if (j == 6) {
					GotoXY(bird[i][j].x, bird[i][j].y);
					cout << "-";
				}
				if (j == 7) {
					GotoXY(bird[i][j].x, bird[i][j].y);
					cout << "/";
				}
				if (j == 8) {
					GotoXY(bird[i][j].x, bird[i][j].y);
					cout << "}";
				}
				if (j == 9) {
					GotoXY(bird[i][j].x, bird[i][j].y);
					cout << "\\";
				}
				if (j == 10) {
					GotoXY(bird[i][j].x, bird[i][j].y);
					cout << "}";
				}
			}
			else {
				if (j == 0) {
					GotoXY(bird[i][j].x, bird[i][j].y);
					cout << "-";
				}
				if (j == 1) {
					GotoXY(bird[i][j].x, bird[i][j].y);
					cout << ">";
				}
				if (j == 2) {
					GotoXY(bird[i][j].x, bird[i][j].y);
					cout << " ";
				}
				if (j == 3) {
					GotoXY(bird[i][j].x, bird[i][j].y);
					cout << " ";
				}
				if (j == 4) {
					GotoXY(bird[i][j].x, bird[i][j].y);
					cout << " ";
				}
				if (j == 5) {
					GotoXY(bird[i][j].x, bird[i][j].y);
					cout << "}";
				}
				if (j == 6) {
					GotoXY(bird[i][j].x, bird[i][j].y);
					cout << "-";
				}
				if (j == 7) {
					GotoXY(bird[i][j].x, bird[i][j].y);
					cout << "\\";
				}
				if (j == 8) {
					GotoXY(bird[i][j].x, bird[i][j].y);
					cout << "{";
				}
				if (j == 9) {
					GotoXY(bird[i][j].x, bird[i][j].y);
					cout << "/";
				}
				if (j == 10) {
					GotoXY(bird[i][j].x, bird[i][j].y);
					cout << "{";
				}
			}

		}
	}
}
void EraseBird(int Num) {
	for (int i = 0; i < Num; i++) {
		for (int j = 0; j < 11; j++) {
			SetColor(14, 0);
			for (int z = 0; z < sizeMap; z++) {
				if (bird[i][j].x == map[z].x && bird[i][j].y == map[z].y) {
					SetColor(map[z].background, 0);
				}
			}
			GotoXY(bird[i][j].x, bird[i][j].y);
			cout << " ";
		}
	}
}
void Information(int x, int y) {
	SetColor(15, 0);

	GotoXY(x + 7, y);     cout << "      ┓ ┏┓┓┏┏┓┓ ";
	GotoXY(x + 7, y + 1); cout << "━━━━  ┃ ┣ ┃┃┣ ┃ ";
	GotoXY(x + 7, y + 2); cout << "      ┗┛┗┛┗┛┗┛┗┛";
	PrintNumber(LEVEL, x + 25, y, 15);
	GotoXY(x + 28, y + 1); cout << "━━━━";

	GotoXY(x, y + 5); cout << " ┏┳┓┳┳┳┓┏┓ ";
	GotoXY(x, y + 6); cout << "  ┃ ┃┃┃┃┣ :";
	GotoXY(x, y + 7); cout << "  ┻ ┻┛ ┗┗┛ ";
	PrintNumber(TIME, 162 + 16, 17, 15);

	GotoXY(x, y + 9);  cout << " ┏┓┏┓┏┓┳┓┏┓ ";
	GotoXY(x, y + 10); cout << " ┗┓┃ ┃┃┣┫┣  :";
	GotoXY(x, y + 11); cout << " ┗┛┗┛┗┛┛┗┗┛ ";
	PrintNumber(SCORE, x + 16, y + 9, 15);

	PrintFoodBar();

	GotoXY(x, y + 17); cout << "╔═══╗";
	GotoXY(x, y + 18); cout << "║ P ║ Pause game";
	GotoXY(x, y + 19); cout << "╚═══╝";

	GotoXY(x + 21, y + 17); cout << "╔═══╗";
	GotoXY(x + 21, y + 18); cout << "║ L ║ Save Game";
	GotoXY(x + 21, y + 19); cout << "╚═══╝";

	GotoXY(x, y + 20); cout << "╔═══╗";
	GotoXY(x, y + 21); cout << "║ E ║ Exit game";
	GotoXY(x, y + 22); cout << "╚═══╝";

	GotoXY(x + 21, y + 20); cout << "╔═══╗";
	GotoXY(x + 21, y + 21); cout << "║ESC║ Back to menu";
	GotoXY(x + 21, y + 22); cout << "╚═══╝";
}
void PrintNumber(int num, int x, int y, int background) {
	if (num == 0) {
		SetColor(background, 0);
		GotoXY(x, y);     cout << "┏┓" << endl;
		GotoXY(x, y + 1); cout << "┃┃" << endl;
		GotoXY(x, y + 2); cout << "┗┛" << endl;
	}
	else if (num == 1) {
		SetColor(background, 0);
		GotoXY(x, y);     cout << "┓ " << endl;
		GotoXY(x, y + 1); cout << "┃ " << endl;
		GotoXY(x, y + 2); cout << "┻ " << endl;
	}
	else if (num == 2) {
		SetColor(background, 0);
		GotoXY(x, y);     cout << "┏┓" << endl;
		GotoXY(x, y + 1); cout << "┏┛" << endl;
		GotoXY(x, y + 2); cout << "┗━" << endl;
	}
	else if (num == 3) {
		SetColor(background, 0);
		GotoXY(x, y);     cout << "┏┓" << endl;
		GotoXY(x, y + 1); cout << " ┫" << endl;
		GotoXY(x, y + 2); cout << "┗┛" << endl;
	}
	else if (num == 4) {
		SetColor(background, 0);
		GotoXY(x, y);     cout << "┏┓" << endl;
		GotoXY(x, y + 1); cout << "┃┃" << endl;
		GotoXY(x, y + 2); cout << "┗╋" << endl;
	}
	else if (num == 5) {
		SetColor(background, 0);
		GotoXY(x, y);     cout << "┏━" << endl;
		GotoXY(x, y + 1); cout << "┗┓" << endl;
		GotoXY(x, y + 2); cout << "┗┛" << endl;
	}
	else if (num == 6) {
		SetColor(background, 0);
		GotoXY(x, y);     cout << "┏┓" << endl;
		GotoXY(x, y + 1); cout << "┣┓" << endl;
		GotoXY(x, y + 2); cout << "┗┛" << endl;
	}
	else if (num == 7) {
		SetColor(background, 0);
		GotoXY(x, y);     cout << "━┓" << endl;
		GotoXY(x, y + 1); cout << " ┃" << endl;
		GotoXY(x, y + 2); cout << " ╹" << endl;
	}
	else if (num == 8) {
		SetColor(background, 0);
		GotoXY(x, y);     cout << "┏┓" << endl;
		GotoXY(x, y + 1); cout << "┣┫" << endl;
		GotoXY(x, y + 2); cout << "┗┛" << endl;
	}
	else if (num == 9) {
		SetColor(background, 0);
		GotoXY(x, y);     cout << "┏┓" << endl;
		GotoXY(x, y + 1); cout << "┗┫" << endl;
		GotoXY(x, y + 2); cout << "┗┛" << endl;
	}
	else if (num >= 10) {
		int* b = NULL;
		int count = 0;
		while (num != 0) {
			b = (int*)realloc(b, (count + 1) * sizeof(int));
			if (b == NULL) return;
			b[count] = num % 10;
			num = num / 10;
			count++;
		}
		for (int i = count - 1; i >= 0; i--) {
			SetColor(background, 0);
			PrintNumber(b[i], x + (count - 1 - i) * 2, y, background);
		}
		free(b);
	}
	GotoXY(0, 0);
}
void PrintFoodBar() {
	SetColor(15, 0);
	if (FOOD_INDEX == 0) {
		GotoXY(162 + 3, 12 + 14); cout << "█▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀█";
		GotoXY(162 + 3, 12 + 15); cout << "█▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄█";
	}
	if (FOOD_INDEX == 1) {
		GotoXY(162 + 3, 12 + 14); cout << "█████████▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀█";
		GotoXY(162 + 3, 12 + 15); cout << "████████▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄█";
	}
	if (FOOD_INDEX == 2) {
		GotoXY(162 + 3, 12 + 14); cout << "█████████████████▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀█";
		GotoXY(162 + 3, 12 + 15); cout << "████████████████▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄█";
	}
	if (FOOD_INDEX == 3) {
		GotoXY(162 + 3, 12 + 14); cout << "█████████████████████████▀▀▀▀▀▀▀▀█";
		GotoXY(162 + 3, 12 + 15); cout << "████████████████████████▄▄▄▄▄▄▄▄▄█";
	}
	if (GATE_STATE == 1) {
		GotoXY(162 + 3, 12 + 14); cout << "██████████████████████████████████";
		GotoXY(162 + 3, 12 + 15); cout << "██████████████████████████████████";
	}
}

//Các hàm về start, load, pause, save và exit game
void StartNewGame() {
	shouldTerminate = false;
	system("cls");
	system("color e0");
	setConsoleFont(L"Cascadia Code Bold", 16);
	ResetData();
	ReadImageFromFie("Frame1.txt", 152, 1);
	Information(162, 12);
	ReadImageFromFie("Frame2.txt", 0, 1);
	DrawMap();
	begin_t = clock();
	STATE = 1;
	thread t1(ThreadFunc); // Tạo ra 1 luồng cho con rắn
	while (1) {
		if (STATE == 1 || STATE == 2) {
			int temp = toupper(_getch());
			if (temp == 'P') {
				playsoundeffect();
				PauseGame();
			}
			else if (temp == 'E') {
				playsoundeffect();
				SaveResumeGame();
				ExitGame(t1);
				return;
			}
			else if (temp == 'L') {
				playsoundeffect();
				PauseGame();
				Sleep(1000);
				SaveGameMenu();
				Sleep(1000);
				SaveResumeGame();
				ExitGame(t1);
				PrintMenu();
				return;
			}
			else if (temp == 27) {
				playsoundeffect();
				SaveResumeGame();
				ExitGame(t1);
				PrintMenu();
				return;
			}
			else {
				ResumeGame();
				if ((temp != CHAR_LOCK) && (temp == 'D' || temp == 'A' || temp ==
					'W' || temp == 'S'))
				{
					if (temp == 'D') CHAR_LOCK = 'A';
					else if (temp == 'W') CHAR_LOCK = 'S';
					else if (temp == 'S') CHAR_LOCK = 'W';
					else CHAR_LOCK = 'D';
					MOVING = temp;
				}
			}
		}
		else if (STATE == 0 || STATE == 3) {
			playsoundeffect();
			Sleep(2000);
			if (shouldTerminate == true) {
				ExitGame(t1);
				return;
			}
			SaveGameMenu();
			SaveResumeGame();
			ExitGame(t1);
			PrintMenu();
			return;
		}
	}
}
void StartResumeGame() {
	shouldTerminate = false;
	Pause_Time = 0;
	system("cls");
	system("color e0");
	setConsoleFont(L"Cascadia Code Bold", 16);
	LoadResumeGame();
	ReadImageFromFie("Frame1.txt", 152, 1);
	Information(162, 12);
	ReadImageFromFie("Frame2.txt", 0, 1);
	DrawMap();
	begin_t = clock() - clock_t(TIME * CLOCKS_PER_SEC);
	STATE = 1;
	thread t1(ThreadFunc); // Tạo ra 1 luồng cho con rắn
	while (1) {
		if (STATE == 1 || STATE == 2) {
			int temp = toupper(_getch());
			if (temp == 'P') {
				PauseGame();
			}
			else if (temp == 'E') {
				if (strcmp(NAME, "NoName") == 0) {
					SaveResumeGame();
				}
				else
					SaveLoadGame();
				ExitGame(t1);
				return;
			}
			else if (temp == 'L') {
				PauseGame();
				Sleep(1000);
				if (strcmp(NAME, "NoName") == 0) {
					SaveGameMenu();
					Sleep(1000);
				}
				else
					SaveLoadGame();
				SaveResumeGame();
				ExitGame(t1);
				PrintMenu();
				return;
			}
			else if (temp == 27) {
				if (strcmp(NAME, "NoName") != 0)
					SaveLoadGame();
				SaveResumeGame();
				ExitGame(t1);
				PrintMenu();
				return;
			}
			else {
				ResumeGame();
				if ((temp != CHAR_LOCK) && (temp == 'D' || temp == 'A' || temp ==
					'W' || temp == 'S'))
				{
					if (temp == 'D') CHAR_LOCK = 'A';
					else if (temp == 'W') CHAR_LOCK = 'S';
					else if (temp == 'S') CHAR_LOCK = 'W';
					else CHAR_LOCK = 'D';
					MOVING = temp;
				}
			}
		}
		else if (STATE == 0 || STATE == 3) {
			Sleep(2000);
			if (shouldTerminate == true) {
				ExitGame(t1);
				return;
			}
			if (strcmp(NAME, "NoName") == 0) {
				SaveGameMenu();
			}
			else {
				SaveLoadGame();
				SaveLoadGameMenu();
			}
			SaveResumeGame();
			ExitGame(t1);
			PrintMenu();
			return;
		}
	}
}
void StartLoadGame(const char* PlayerName) {
	shouldTerminate = false;
	Pause_Time = 0;
	system("cls");
	system("color e0");
	setConsoleFont(L"Cascadia Code Bold", 16);
	LoadGame("SaveGame.dat", PlayerName);
	ReadImageFromFie("Frame1.txt", 152, 1);
	Information(162, 12);
	ReadImageFromFie("Frame2.txt", 0, 1);
	DrawMap();
	begin_t = clock() - clock_t(TIME * CLOCKS_PER_SEC);
	STATE = 1;
	thread t1(ThreadFunc); // Tạo ra 1 luồng cho con rắn
	while (1) {
		if (STATE == 1 || STATE == 2) {
			int temp = toupper(_getch());
			if (temp == 'P') {
				PauseGame();
			}
			else if (temp == 'E') {
				SaveLoadGame();
				SaveResumeGame();
				ExitGame(t1);
				return;
			}
			else if (temp == 'L') {
				SaveLoadGame();
				SaveResumeGame();
				ExitGame(t1);
				PrintMenu();
				return;
			}
			else if (temp == 27) {
				SaveLoadGame();
				SaveResumeGame();
				ExitGame(t1);
				PrintMenu();
				return;
			}
			else {
				ResumeGame();
				if ((temp != CHAR_LOCK) && (temp == 'D' || temp == 'A' || temp ==
					'W' || temp == 'S'))
				{
					if (temp == 'D') CHAR_LOCK = 'A';
					else if (temp == 'W') CHAR_LOCK = 'S';
					else if (temp == 'S') CHAR_LOCK = 'W';
					else CHAR_LOCK = 'D';
					MOVING = temp;
				}
			}
		}
		else if (STATE == 0 || STATE == 3) {
			Sleep(2000);
			if (shouldTerminate == true) {
				ExitGame(t1);
				return;
			}
			SaveLoadGame();
			SaveResumeGame();
			SaveLoadGameMenu();
			ExitGame(t1);
			PrintMenu();
			return;
		}
	}
}
void ExitGame(thread& t) {
	shouldPause = false;
	shouldTerminate = true;
	t.join();
	system("cls");
}
void PauseGame() {
	shouldPause = true;
}
void ResumeGame()
{
	shouldPause = false; // Đặt biến cờ để tiếp tục luồng
}
void RestartGame() {
	shouldTerminate = false;
	system("cls");
	system("color e0");
	ResetData();
	ReadImageFromFie("Frame1.txt", 152, 1);
	Information(162, 12);
	ReadImageFromFie("Frame2.txt", 0, 1);
	DrawMap();
	begin_t = clock();
	STATE = 1;
}
void SaveGame(const char* FileName, const char* PlayerName) {
	FILE* f = fopen(FileName, "ab");
	if (f == NULL)
		return;
	PlayerInformation Player;
	InitPlayerInfo(Player, PlayerName);
	fwrite(&Player, sizeof(PlayerInformation), 1, f);
	fclose(f);
	return;
}
void SaveLoadGame() {
	FILE* f = fopen("SaveGame.dat", "rb+");
	if (f == NULL)
		return;
	PlayerInformation Player;
	fread(&Player, sizeof(PlayerInformation), 1, f);
	while (strcmp(NAME, Player.Name) != 0 || Player.State != 1) {
		fread(&Player, sizeof(PlayerInformation), 1, f);
	}
	fseek(f, (-1) * (sizeof(PlayerInformation)), SEEK_CUR);
	InitPlayerInfo(Player, NAME);
	fwrite(&Player, sizeof(PlayerInformation), 1, f);
	fclose(f);
	return;
}
void SaveResumeGame() {
	FILE* f = fopen("ResumeData.dat", "wb");
	if (f == NULL)
		return;
	PlayerInformation Player;
	InitPlayerInfo(Player, NAME);
	fwrite(&Player, sizeof(PlayerInformation), 1, f);
	fclose(f);
	return;
}
void LoadGame(const char* FileName, const char* PlayerName) {
	FILE* f = fopen(FileName, "rb");
	if (f == NULL)
		return;
	PlayerInformation Player;
	fread(&Player, sizeof(PlayerInformation), 1, f);
	while (strcmp(PlayerName, Player.Name) != 0 || Player.State != 1) {
		fread(&Player, sizeof(PlayerInformation), 1, f);
	}
	fclose(f);
	LoadPlayerInfo(Player);
	return;
}
void LoadResumeGame() {
	FILE* f = fopen("ResumeData.dat", "rb");
	if (f == NULL)
		return;
	PlayerInformation Player;
	fread(&Player, sizeof(PlayerInformation), 1, f);
	fclose(f);
	LoadPlayerInfo(Player);
	return;
}
void PlayAgainMenu() {
	int Arrcolors[] = { 8, 14 };
	int counter = 1;
	while (1) {
		SetColor(14, 0);
		GotoXY(66, 38); cout << "┏┓┓ ┏┓┓┏  ┏┓┏┓┏┓┳┳┓ ┏┓";
		GotoXY(66, 39); cout << "┃┃┃ ┣┫┗┫  ┣┫┃┓┣┫┃┃┃ ┏┛";
		GotoXY(66, 40); cout << "┣┛┗┛┛┗┗┛  ┛┗┗┛┛┗┻┛┗ • ";

		SetColor(Arrcolors[0], 0);
		GotoXY(62, 42); cout << "┓┏";
		GotoXY(62, 43); cout << "┗┫┏┓┏";
		GotoXY(62, 44); cout << "┗┛┗ ┛";

		SetColor(Arrcolors[1], 0);
		GotoXY(88, 42); cout << "┳┓  ";
		GotoXY(88, 43); cout << "┃┃┏┓";
		GotoXY(88, 44); cout << "┛┗┗┛";

		int temp = toupper(_getch());
		if (temp == 'D' && counter == 1)
			counter++;
		if (temp == 'A' && counter == 2)
			counter--;
		if (temp == '\r') {
			if (counter == 1) {
				RestartGame();
				return;
			}
			if (counter == 2) {
				shouldTerminate = 1;
				return;
			}
		}
		if (counter == 1) {
			Arrcolors[0] = 8;
			Arrcolors[1] = 14;
		}
		if (counter == 2) {
			Arrcolors[0] = 14;
			Arrcolors[1] = 8;
		}
	}
}

//Các mini menu game trong gameplay
void PrintStartGameOption(int x, int y) {
	PlayerInformation A[20];
	FILE* f = fopen("SaveGame.dat", "rb");
	if (f == NULL) {
		StartNewGame();
		return;
	}
	int num = 0;
	for (int i = 0; i < 100 && feof(f) == false && num <= 20; i++) {
		fread(&A[num], sizeof(PlayerInformation), 1, f);
		if (A[num].State == 1) {
			num++;
		}
	}
	fclose(f);
	ReadImageFromFie("StartGameFrame.txt", 11, 4);
	GotoXY(17, 28);
	cout << "Eat 4 food to go to the next map!";
	GotoXY(17, 29);
	cout << "Finish all the maps to win the game!";
	GotoXY(17, 30);
	cout << "You just have one chance. If your snake die,";
	GotoXY(17, 31);
	cout << "the game is over so be careful!";
	int* Arrcolors = new int[num + 1];
	Arrcolors[0] = 11;
	for (int i = 1; i < num + 1; i++) {
		Arrcolors[i] = 14;
	}
	int counter = 0;

	GotoXY(x, y - 1); cout << "PLAYER NAME";
	GotoXY(x + 30, y - 1); cout << "SCORE\tTIME\tLEVEL";
	while (1) {
		for (int i = 0; i < num; i++) {
			SetColor(Arrcolors[i], 0);
			GotoXY(x, y + i); cout << i + 1 << ". " << A[i].Name;
			GotoXY(x + 30, y + i); cout << A[i].Score << "\t\t" << A[i].Time << '\t' << A[i].Level;
		}
		SetColor(Arrcolors[num], 0);
		GotoXY(x, y + 21); cout << "New Game";
		int temp = toupper(_getch());
		if (temp == 'S' && counter < num) {
			playsoundeffect();
			counter++;
		}
		if (temp == 'W' && counter > 0) {
			playsoundeffect();
			counter--;
		}
		playsoundeffect();
		if (temp == 27) {
			playsoundeffect();
			delete[] Arrcolors;
			system("cls");
			PrintMenu();
			return;
		}
		if (temp == '\r') {
			playsoundeffect();
			delete[] Arrcolors;
			if (counter < num) {
				StartLoadGame(A[counter].Name);
				playsoundeffect();
			}
			if (counter == num) {
				StartNewGame();
				playsoundeffect();
			}
			return;
		}
		for (int i = 0; i < num + 1; i++) {
			if (i == counter)
				Arrcolors[i] = 11;
			else
				Arrcolors[i] = 14;
		}
	}
}
void SaveGameMenu() {
	if (STATE == 1) {
		ReadImageFromFie("SaveGameFrame.txt", 46, 23);
		GotoXY(68, 25); cout << "┏┓┏┓┓┏┏┓  ┏┓┏┓┳┳┓┏┓";
		GotoXY(68, 26); cout << "┗┓┣┫┃┃┣   ┃┓┣┫┃┃┃┣ ";
		GotoXY(68, 27); cout << "┗┛┛┗┗┛┗┛  ┗┛┛┗┛ ┗┗┛";
		GotoXY(56, 30); cout << "Enter your name: ";

		char PlayerName[50];
		cin.getline(PlayerName, 50);
		while (strlen(PlayerName) > 15 || CheckValidName(PlayerName) == 0) {
			SetColor(14, 12);
			GotoXY(60, 31); cout << "                                   ";
			if (strlen(PlayerName) > 15) {
				GotoXY(60, 31); cout << "Please enter less than 15 character";
			}
			else {
				GotoXY(60, 31); cout << "Player name already existed";
			}
			SetColor(14, 0);
			GotoXY(73, 30); cout << "                                  ";
			GotoXY(73, 30); cin.getline(PlayerName, 50);
		}

		SaveGame("SaveGame.dat", PlayerName);
		GotoXY(60, 31); cout << "                                   ";
		SetColor(14, 10);
		GotoXY(68, 31); cout << "Saved successfully!";
		return;
	}
	else if (STATE == 0) {
		PlayerInformation Player;
		InitPlayerInfo(Player, NAME);
		if (CheckHighscore(Player) == 0) {
			PlayAgainMenu();
			return;
		}
		SetColor(14, 0);
		GotoXY(68, 39); cout << "┏┳┓┏┓┏┓";
		GotoXY(68, 40); cout << " ┃ ┃┃┃┃";
		GotoXY(68, 41); cout << " ┻ ┗┛┣┛";
		PrintNumber(CheckHighscore(Player), 77, 39, 14);
		SetColor(14, 0);
		GotoXY(54, 43); cout << "Enter your name: ";
		char PlayerName[50];
		GotoXY(71, 43);
		cin.getline(PlayerName, 50);
		while (strlen(PlayerName) > 15 || CheckValidName(PlayerName) == 0) {
			SetColor(14, 12);
			GotoXY(58, 44); cout << "                                   ";
			if (strlen(PlayerName) > 15) {
				GotoXY(58, 44); cout << "Please enter less than 15 character";
			}
			else {
				GotoXY(58, 44); cout << "Player name already existed";
			}
			SetColor(14, 0);
			GotoXY(71, 43); cout << "                                  ";
			GotoXY(71, 43); cin.getline(PlayerName, 50);
		}
		InitPlayerInfo(Player, PlayerName);
		UpdateHightScore(Player);
		shouldTerminate = true;
		return;
	}
	else if (STATE == 3) {
		PlayerInformation Player;
		InitPlayerInfo(Player, "Player");
		if (CheckHighscore(Player) == 0) {
			PlayAgainMenu();
			return;
		}
		ReadImageFromFie("SaveGameFrame.txt", 46, 33);
		GotoXY(72, 36); cout << "┏┳┓┏┓┏┓";
		GotoXY(72, 37); cout << " ┃ ┃┃┃┃";
		GotoXY(72, 38); cout << " ┻ ┗┛┣┛";
		PrintNumber(CheckHighscore(Player), 81, 36, 14);
		GotoXY(56, 40); cout << "Enter your name: ";
		char PlayerName[50];
		cin.getline(PlayerName, 50);
		while (strlen(PlayerName) > 15 || CheckValidName(PlayerName) == 0) {
			SetColor(14, 12);
			GotoXY(60, 41); cout << "                                   ";
			if (strlen(PlayerName) > 15) {
				GotoXY(60, 41); cout << "Please enter less than 15 character";
			}
			else {
				GotoXY(60, 41); cout << "Player name already existed";
			}
			SetColor(14, 0);
			GotoXY(73, 40); cout << "                                  ";
			GotoXY(73, 40); cin.getline(PlayerName, 50);
		}
		InitPlayerInfo(Player, PlayerName);
		UpdateHightScore(Player);
		system("cls");
		shouldTerminate = true;
		PrintMenu();
		return;
	}
}
void SaveLoadGameMenu() {
	PlayerInformation Player;
	InitPlayerInfo(Player, NAME);
	if (CheckHighscore(Player) == 0) {
		PlayAgainMenu();
		return;
	}
	SetColor(14, 0);
	GotoXY(68, 39); cout << "┏┳┓┏┓┏┓";
	GotoXY(68, 40); cout << " ┃ ┃┃┃┃";
	GotoXY(68, 41); cout << " ┻ ┗┛┣┛";
	PrintNumber(CheckHighscore(Player), 77, 39, 14);
	Sleep(3000);
	UpdateHightScore(Player);
	shouldTerminate = true;
	return;
}
void OptionMenu(int x, int y) {
	ReadImageFromFie("OptionFrame.txt", 50, 4);
	SetColor(14, 0);
	GotoXY(x, y);     cout << "┏┓┳┓┏┓┓┏┓┏┓  ┏┓┏┓┓ ┏┓┳┓";
	GotoXY(x, y + 1); cout << "┗┓┃┃┣┫┃┫ ┣   ┃ ┃┃┃ ┃┃┣┫";
	GotoXY(x, y + 2); cout << "┗┛┛┗┛┗┛┗┛┗┛  ┗┛┗┛┗┛┗┛┛┗";

	GotoXY(x, y + 4); cout << "┏┓┏┓┏┓┏┓┳┓";
	GotoXY(x, y + 5); cout << "┗┓┃┃┣ ┣ ┃┃";
	GotoXY(x, y + 6); cout << "┗┛┣┛┗┛┗┛┻┛";

	GotoXY(x, y + 8);  cout << "┳┳┓┳┳┏┓┳┏┓";
	GotoXY(x, y + 9);  cout << "┃┃┃┃┃┗┓┃┃ ";
	GotoXY(x, y + 10); cout << "┛ ┗┗┛┗┛┻┗┛";
	int snakecolor[3] = { 1,2,12 };
	int snakecolor_index = 0;
	for (int i = 0; i < 3; i++) {
		if (snakecolor[i] == Snake_Color)
			snakecolor_index = i;
	}
	int speed = SPEED;
	int music = Music;
	int counter = 3;
	while (1) {
		if (snakecolor_index == 1) {
			mutesoundeffect();
			playsoundeffect();
			GotoXY(x + 33, y);     cout << "┏┓┳┓┏┓┏┓┳┓";
			GotoXY(x + 33, y + 1); cout << "┃┓┣┫┣ ┣ ┃┃";
			GotoXY(x + 33, y + 2); cout << "┗┛┛┗┗┛┗┛┛┗";
		}
		else if (snakecolor_index == 0) {
			mutesoundeffect();
			playsoundeffect();
			GotoXY(x + 33, y);     cout << " ┳┓┓ ┳┳┏┓ ";
			GotoXY(x + 33, y + 1); cout << " ┣┫┃ ┃┃┣  ";
			GotoXY(x + 33, y + 2); cout << " ┻┛┗┛┗┛┗┛ ";
		}
		else if (snakecolor_index == 2) {
			mutesoundeffect();
			playsoundeffect();
			GotoXY(x + 33, y);     cout << "  ┳┓┏┓┳┓  ";
			GotoXY(x + 33, y + 1); cout << "  ┣┫┣ ┃┃  ";
			GotoXY(x + 33, y + 2); cout << "  ┛┗┗┛┻┛  ";
		}

		PrintNumber(speed, x + 36, y + 4, 14);

		if (music == 1) {
			mutesoundeffect();
			playsoundeffect();
			sound1();
			GotoXY(x + 33, y + 8); cout << "   ┏┓┳┓ ";
			GotoXY(x + 33, y + 9); cout << "   ┃┃┃┃ ";
			GotoXY(x + 33, y + 10); cout <<"   ┗┛┛┗ ";
		}
		else {
			mutesoundeffect();
			playsoundeffect();
			mutesound();
			GotoXY(x + 33, y + 8); cout << "  ┏┓┏┓┏┓";
			GotoXY(x + 33, y + 9); cout << "  ┃┃┣ ┣ ";
			GotoXY(x + 33, y + 10); cout <<"  ┗┛┻ ┻ ";
		}
		if (counter == 1) {
			GotoXY(x + 31, y);     cout << "┏";
			GotoXY(x + 31, y + 1); cout << "┃";
			GotoXY(x + 31, y + 2); cout << "┗";
			GotoXY(x + 44, y);     cout << "┓";
			GotoXY(x + 44, y + 1); cout << "┃";
			GotoXY(x + 44, y + 2); cout << "┛";
		}
		else if (counter == 2) {
			GotoXY(x + 31, y + 4); cout << "┏";
			GotoXY(x + 31, y + 5); cout << "┃";
			GotoXY(x + 31, y + 6); cout << "┗";
			GotoXY(x + 44, y + 4); cout << "┓";
			GotoXY(x + 44, y + 5); cout << "┃";
			GotoXY(x + 44, y + 6); cout << "┛";
		}
		else if (counter == 3) {
			GotoXY(x + 31, y + 8); cout << "┏";
			GotoXY(x + 31, y + 9); cout << "┃";
			GotoXY(x + 31, y + 10); cout <<"┗";
			GotoXY(x + 44, y + 8); cout << "┓";
			GotoXY(x + 44, y + 9); cout << "┃";
			GotoXY(x + 44, y + 10); cout <<"┛";
		}
		int temp = toupper(_getch());
		if (temp == 'S' && counter < 3) {
			playsoundeffect();
			counter++;
		}
		if (temp == 'W' && counter > 1) {
			playsoundeffect();
			counter--;
		}
		if (temp == 'A') {
			playsoundeffect();
			if (counter == 1) {
				if (snakecolor_index > 0)
					snakecolor_index--;
			}
			if (counter == 2) {
				if (speed > 10)
					speed--;
			}
			if (counter == 3) {
				if (music > 0)
					music--;
			}
		}
		if (temp == 'D') {
			playsoundeffect();
			if (counter == 1) {
				if (snakecolor_index < 2)
					snakecolor_index++;
			}
			if (counter == 2) {
				if (speed < 50)
					speed++;
			}
			if (counter == 3) {
				if (music < 1)
					music++;
			}
		}
		if (temp == 27) {
			playsoundeffect();
			SPEED = speed;
			Music = music;
			Snake_Color = snakecolor[snakecolor_index];
			system("cls");
			PrintMenu();
			return;
		}
		GotoXY(x + 31, y); cout << " ";
		GotoXY(x + 31, y + 1); cout << " ";
		GotoXY(x + 31, y + 2); cout << " ";
		GotoXY(x + 44, y); cout << " ";
		GotoXY(x + 44, y + 1); cout << " ";
		GotoXY(x + 44, y + 2); cout << " ";
		GotoXY(x + 31, y + 4); cout << " ";
		GotoXY(x + 31, y + 5); cout << " ";
		GotoXY(x + 31, y + 6); cout << " ";
		GotoXY(x + 44, y + 4); cout << " ";
		GotoXY(x + 44, y + 5); cout << " ";
		GotoXY(x + 44, y + 6); cout << " ";
		GotoXY(x + 31, y + 8); cout << " ";
		GotoXY(x + 31, y + 9); cout << " ";
		GotoXY(x + 31, y + 10); cout << " ";
		GotoXY(x + 44, y + 8); cout << " ";
		GotoXY(x + 44, y + 9); cout << " ";
		GotoXY(x + 44, y + 10); cout << " ";
	}
}

//Các hàm về highscore
int CheckHighscore(PlayerInformation A) {
	PlayerInformation top10[10] = {};
	int top = 0;
	FILE* f = fopen("HighScore.dat", "rb");
	for (int i = 0; i < 10 && feof(f) == false; i++) {
		fread(&top10[i], sizeof(PlayerInformation), 1, f);
	}
	fclose(f);
	for (int i = 0; i < 10; i++) {
		if (A.Level > top10[i].Level) {
			top = i + 1;
			return top;
		}
		else if (A.Level == top10[i].Level) {
			for (int j = i; j < 10 && A.Level == top10[j].Level; j++) {
				if (A.Score > top10[j].Score) {
					top = j + 1;
					return top;
				}
				if (A.Score == top10[j].Score) {
					for (int z = j; z < 10 && A.Score == top10[z].Score; z++) {
						if (A.Time < top10[z].Time) {
							top = z + 1;
							return top;
						}
						if (z != 9 && A.Time == top10[z].Time && top10[z].Score > top10[z + 1].Score) {
							top = z + 1;
							return top;
						}
					}
				}
				if (top10[j].Level > top10[j + 1].Level && j != 9) {
					top = j + 2;
					return top;
				}
			}
		}
	}
	return top;
}
void UpdateHightScore(PlayerInformation Player) {
	PlayerInformation top[20] = {};
	FILE* f = fopen("HighScore.dat", "rb");
	for (int i = 0; i < 20 && feof(f) == false; i++) {
		fread(&top[i], sizeof(PlayerInformation), 1, f);
	}
	fclose(f);
	int pos = CheckHighscore(Player) - 1;
	for (int j = 19; j > pos; j--) {
		top[j] = top[j - 1];
	}
	top[pos] = Player;
	f = fopen("HighScore.dat", "wb");
	for (int i = 0; i < 20; i++) {
		fwrite(&top[i], sizeof(PlayerInformation), 1, f);
	}
	fclose(f);
}
void InitPlayerInfo(PlayerInformation& A, const char* PlayerName) {
	strcpy(A.Name, PlayerName);
	A.Score = SCORE;
	A.State = STATE;
	A.Moving = MOVING;
	A.char_lock = CHAR_LOCK;
	A.Speed = SPEED;
	A.food_index = FOOD_INDEX;
	A.size_snake = SIZE_SNAKE;
	A.Gate_State = GATE_STATE;
	A.Level = LEVEL;
	A.Time = TIME;
	for (int i = 0; i < SIZE_SNAKE; i++) {
		A.Snake[i] = snake[i];
	}
	for (int i = 0; i < 4; i++) {
		A.Food[i] = food[i];
	}
	for (int i = 0; i < 6; i++) {
		A.levelGate[i] = LevelGate[i];
	}
}
void LoadPlayerInfo(PlayerInformation A) {
	strcpy(NAME, A.Name);
	SCORE = A.Score;
	STATE = A.State;
	MOVING = A.Moving;
	CHAR_LOCK = A.char_lock;
	SPEED = A.Speed;
	FOOD_INDEX = A.food_index;
	SIZE_SNAKE = A.size_snake;
	GATE_STATE = A.Gate_State;
	LEVEL = A.Level;
	TIME = A.Time;
	for (int i = 0; i < SIZE_SNAKE; i++) {
		snake[i] = A.Snake[i];
	}
	for (int i = 0; i < 4; i++) {
		food[i] = A.Food[i];
	}
	for (int i = 0; i < 6; i++) {
		LevelGate[i] = A.levelGate[i];
	}
	InitMap(0, 12, 150, 36);
}

//Luồng để chạy trò chơi
void ThreadFunc() {
	while (!shouldTerminate) {
		if (STATE == 1 || STATE == 2) {
			end_t = clock();
			TIME = (int)(end_t - begin_t) / CLOCKS_PER_SEC - Pause_Time;
			PrintNumber(TIME, 162 + 16, 17, 15);
			if (LEVEL == 6) {
				EraseBird(3);
			}
			DrawSnake(" ");
			if (GATE_STATE == 0) {
				DrawFood(" ");
			}
			switch (MOVING) {
			case 'A':
				MoveLeft();
				break;
			case 'D':
				MoveRight();
				break;
			case 'W':
				MoveUp();
				break;
			case 'S':
				MoveDown();
				break;
			}
			if (GATE_STATE == 1) {
				DrawFood(" ");
				DrawGate();
			}
			if (GATE_STATE == 0) {
				DrawFood("⬤");
			}
			if (LEVEL == 6) {
				BirdMove(3);
				DrawBird(3);
			}
			DrawSnake("O");
			Sleep(1000 / SPEED);
		}
		if (STATE == 0) {
			Sleep(1000);
			ReadImageFromFie("GameOver.txt", 0, 13);
			while (!shouldTerminate && STATE == 0);
		}
		if (STATE == 3) {
			VictoryEffect();
			while (!shouldTerminate && STATE == 3);
		}
		if (shouldPause) {
			PrintNumber(TIME, 162 + 16, 17, 15);
			clock_t pbegin_t, pend_t;
			pbegin_t = clock();
			while (shouldPause);
			pend_t = clock();
			Pause_Time += (int)(pend_t - pbegin_t) / CLOCKS_PER_SEC;
		}
	}
}

//Các hàm khác
void DrawChangeLevelEffect() {
	ReadImageFromFie("ClearMap.txt", 0, 13);
	for (int i = 0; i < 246; i++) {
		//Vẽ phần đầu rắn
		if (i < 118)
			ReadImageFromFie("NextLevelEffect1.txt", 2 + i, 26);
		if (118 <= i && i < 124)
			ReadImageFromFie("NextLevelEffect3.txt", 2 + i, 26);
		if (124 <= i && i < 126)
			ReadImageFromFie("NextLevelEffect4.txt", 2 + i, 26);
		if (126 <= i && i < 130)
			ReadImageFromFie("NextLevelEffect5.txt", 2 + i, 26);
		if (130 <= i && i < 132)
			ReadImageFromFie("NextLevelEffect6.txt", 2 + i, 26);
		if (132 <= i && i < 144)
			ReadImageFromFie("NextLevelEffect7.txt", 2 + i, 26);
		if (144 <= i && i < 146)
			ReadImageFromFie("NextLevelEffect8.txt", 2 + i, 26);

		//Vẽ phần đuôi rắn
		if (i >= 90 && i < 236)
			ReadImageFromFie("NextLevelEffect9.txt", 2 + i - 90, 26);
		if (i >= 98 && i < 243)
			ReadImageFromFie("NextLevelEffect10.txt", 2 + i - 98, 26);
		if (i >= 100)
			ReadImageFromFie("NextLevelEffect2.txt", 2 + i - 100, 26);
		Sleep(1);
	}
}
void VictoryEffect() {
	soundvictory();
	ReadImageFromFie("ClearMap.txt", 0, 13);
	ReadImageFromFie("Victory.txt", 0, 13);
	for (int i = 0; i < 6; i++) {
		if (i % 2 == 0) {
			ReadImageFromFie("VictoryEffect1.txt", 15, 16);
			ReadImageFromFie("VictoryEffect2.txt", 126, 17);
			ReadImageFromFie("VictoryEffect1.txt", 121, 28);
			ReadImageFromFie("VictoryEffect2.txt", 9, 28);
		}
		else {
			ReadImageFromFie("VictoryEffect2.txt", 15, 16);
			ReadImageFromFie("VictoryEffect1.txt", 126, 17);
			ReadImageFromFie("VictoryEffect2.txt", 121, 28);
			ReadImageFromFie("VictoryEffect1.txt", 9, 28);
		}
		Sleep(1000);
	}
}
bool CheckValidName(const char* Name) {
	FILE* f = fopen("SaveGame.dat", "rb");
	if (f == NULL)
		return 1;
	PlayerInformation Player;
	fread(&Player, sizeof(PlayerInformation), 1, f);
	while (!feof(f)) {
		if (strcmp(Name, Player.Name) == 0 && Player.State == 1) {
			fclose(f);
			return 0;
		}
		fread(&Player, sizeof(PlayerInformation), 1, f);
	}
	fclose(f);
	return 1;
}