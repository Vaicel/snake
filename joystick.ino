#include <SPI.h>
#include "snake.h"

#define STEP_TIME 200

/*dir: 0 - up 1 - down 2 - left 3 - right*/

boolean pic[8][8];

int timer = 0, timerPrev = 0;

Snake head = {0,0,1};
Snake body[62] = {NULL};

void draw(){
	byte col;
	for(int raw = 0; raw < 8; raw++){
		col=0;
		for(int bitInCol = 7; bitInCol >= 0; bitInCol--){
			col = col + (pic[raw][bitInCol] << bitInCol);	
		}
		digitalWrite(SS_PIN,LOW);
		SPI.transfer(0xFF ^ (1<<raw));
		SPI.transfer(col);
		digitalWrite(SS_PIN,HIGH);
		delay(1);
	}
}

Snake generateHead(Snake head){
	head.dir = getDir(head.dir);
	switch(head.dir){
		case 1: 	head.y += 1; break;
		case 0: 	head.y -= 1; break;
		case 2: 	head.x -= 1; break;
		case 3: 	head.x += 1; break;
	}
	pic[head.y][head.x] = 1;
	return head;
}

void clearMatrix(){
	for(int x1 = 0; x1 < 8; x1++){
		for(int y1 = 0; y1 < 8; y1++){	
			pic[x1][y1]=0;
		}
	}
}

void generateFood(){
	Point food = {random(8),random(8)};
	if (pic[food.x][food.y] == 0){
		pic[food.x][food.y] = 1; 
	}
	else generateFood();
}

int getDir(int dir){
	if(analogRead(X_AXE_PIN)>RIGHT_THRESHOLD)	return 2;
	if(analogRead(Y_AXE_PIN)>UP_THRESHOLD)		return 0;
	if(analogRead(X_AXE_PIN)<LEFT_THRESHOLD)	return 3;
	if(analogRead(Y_AXE_PIN)<DOWN_THRESHOLD)	return 1;
	return dir;
}

void setup(){
	randomSeed(analogRead(A5));
	SPI.begin();
  	pinMode(SS_PIN, OUTPUT);
	pinMode(Z_AXE_PIN, INPUT_PULLUP);
}

void loop(){
	timer = millis();
	if((timer - timerPrev) >= STEP_TIME){
		head = generateHead(head);
		timerPrev = timer;
	}
	draw();
}