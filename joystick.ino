#include <SPI.h>

#define X_AXE_PIN A0
#define Y_AXE_PIN A1

#define UP_THRESHOLD 900
#define DOWN_THRESHOLD 200

#define LEFT_THRESHOLD 200
#define RIGHT_THRESHOLD 900

#define SS_PIN 10
#define COLS_NUM 8

#define DRAW_TIME 500

int headX=0;
int headY=0;

enum dir
{
	up,
	down,
	left,
	right
};


boolean pic[8][8]{
	{0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0}
};

int timer = 0, timePrev = 0;

void setup()
{
	SPI.begin();
  	pinMode(SS_PIN, OUTPUT);
	pinMode(4, INPUT_PULLUP);
}
int p;

void loop()
{
	enum dir = up;
	timer=millis();
	if((timer - timePrev) >= DRAW_TIME){
		generateHead(headX, headY, dir);
		timePrev=timer;
	}
	draw();
}

void draw(){
	int col;
	for(int raw = 0; raw < 8; raw++){
		for(int bitInCol = 7; bitInCol>=0; bitInCol--)
			col = pic[raw][bitInCol] << bitInCol;	
		digitalWrite(SS_PIN,LOW);
		SPI.transfer(0xFF ^ 1<<raw);
		SPI.transfer(col);
		digitalWrite(SS_PIN,HIGH);
		delay(1);
	}
}

void generateHead(int headX, int headY, int dir){
	switch(dir){
		case up: 	headY += 1; break;
		case down: 	headY -= 1; break;
		case left: 	headX -= 1; break;
		case right: headX += 1; break;
	}
	pic[8][8]={NULL};
	pic[headY][headX]=1;
}	