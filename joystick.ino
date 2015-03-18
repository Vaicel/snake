#include <SPI.h>
#include "snake.h"

#define X_AXE_PIN A0
#define Y_AXE_PIN A1

#define UP_THRESHOLD 900
#define DOWN_THRESHOLD 200
#define LEFT_THRESHOLD 200
#define RIGHT_THRESHOLD 900

#define SS_PIN 10
#define COLS_NUM 8

#define DRAW_TIME 2000

//struct Point{
//   int x;
//   int y;
//};

//int headX=0;
//int headY=6;

int dir = 1;
// 0 - up
// 1 - down
// 2 - left
// 3 - right

boolean pic[8][8];//= {NULL};

int timer = 0, timePrev = 0;

Point head={0,0};

void draw(){
	int col;
	for(int raw = 0; raw < 8; raw++){
		col=0;
		for(int bitInCol = 7; bitInCol>=0; bitInCol--){
			col = col+(pic[raw][bitInCol] << bitInCol);	
		}
		digitalWrite(SS_PIN,LOW);
		SPI.transfer(0xFF ^ (1<<raw));
		SPI.transfer(col);
		digitalWrite(SS_PIN,HIGH);
		delay(1);
	}
}

Point generateHead(Point head, int dir){
	switch(dir){
		case 1: 	head.y += 1; break;
		case 0: 	head.y -= 1; break;
		case 2: 	head.x -= 1; break;
		case 3: 	head.x += 1; break;
	}
	for(int x1 = 0; x1<8; x1++){
		for(int y1 = 0; y1<8; y1++){	
			pic[x1][y1]=0;
		}
	}
	pic[head.y][head.x]=1;
	return head;
}	
	byte col = 0;

void setup(){
	SPI.begin();
  	pinMode(SS_PIN, OUTPUT);
	pinMode(4, INPUT_PULLUP);
}

void loop(){
	dir = 1;
	timer=millis();
	if((timer - timePrev) >= DRAW_TIME){
		head = generateHead(head, dir);
		timePrev=timer;
	}
	draw();
}

