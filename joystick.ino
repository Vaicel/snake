#include <SPI.h>
#include "snake.h"

#define STEP_TIME 200

/*dir: 0 - up 1 - down 2 - left 3 - right*/

boolean pic[8][8];

int timer = 0, timerPrev = 0;

int snakeLength=3;
int lastdir = 1;

Snake head = {0,2,1};
Snake body[62] = {NULL};

//body[0]={0,1,1};
//body[1]={0,0,1};

//body[0].x = 0;
//body[0].y = 1;
//body[0].dir = 1;

void draw(boolean pic[8][8]){
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
		case 1: 	
		lastdir !=0 ? head.y += 1 : head.y -=1; break;
		case 0:
		lastdir !=1 ? head.y -= 1 : head.y +=1; break;
		case 2:
		lastdir !=3 ? head.x -= 1 : head.x +=1; break;
		case 3: 
		lastdir !=2 ? head.x += 1 : head.x -=1; break;
	}
	lastdir = head.dir;
	pic[head.y][head.x] = 1;
	return head;
}

Snake generateBody(Snake body[62]){
	clearMatrix();
	for(int gbi = snakeLength-2; gbi >= 1; gbi--){
		body[gbi]=body[gbi-1];
		pic[body[gbi].y][body[gbi].x] = 1;
	}
	body[0] = head;
	pic[body[0].y][body[0].x] = 1;
	return body[62];
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
//	Serial.begin(115200);
	randomSeed(analogRead(A5));
	SPI.begin();
  	pinMode(SS_PIN, OUTPUT);
	pinMode(Z_AXE_PIN, INPUT_PULLUP);
}

void loop(){
	timer = millis();
	if((timer - timerPrev) >= STEP_TIME){
		body[62] = generateBody(body);
		head = generateHead(head);
		timerPrev = timer;
	}
	draw(pic);
//	Serial.print(lastdir);
}