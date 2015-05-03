#ifndef snake_h
#define snake_h

#include <Arduino.h>

/******************
* Макроопределения
*******************/

#define X_AXE_PIN A0
#define Y_AXE_PIN A1 
#define Z_AXE_PIN 4

#define UP_THRESHOLD 900
#define DOWN_THRESHOLD 200
#define LEFT_THRESHOLD 200
#define RIGHT_THRESHOLD 900

#define SS_PIN 10

#define COLS_NUM 8
#define RAWS_NUM 8
#define CELLS_NUM 64

#define MATRIX_REFRESH_TIME_MS 1

/***********
* Структуры
************/

enum Dirs {
	up,
	down,
	left,
	right
};

struct Point{
	int x;
	int y;
};

struct Snake{
	int x;
	int y;
	Dirs dir;
};

/*********
* Функции
**********/

void draw(boolean pic[8][8]);

#endif