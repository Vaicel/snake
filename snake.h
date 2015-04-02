#ifndef snake_h
#define snake_h

#define X_AXE_PIN A0
#define Y_AXE_PIN A1 
#define Z_AXE_PIN 4

#define UP_THRESHOLD 900
#define DOWN_THRESHOLD 200
#define LEFT_THRESHOLD 200
#define RIGHT_THRESHOLD 900

#define SS_PIN 10
#define COLS_NUM 8

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



#endif