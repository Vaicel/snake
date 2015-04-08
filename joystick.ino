//Beta v 1.0
#include <SPI.h>
#include "snake.h"

#define RAWSNUM 8
#define COLSNUM 8

#define STEP_TIME 100
#define MATRIX_REFRESH_TIME_MS 1

/** Dirs: 
* 	0 - up 
*	1 - down 
*	2 - left 	
*	3 - right
**/
byte end [8] = { 
0b00000000,
0b11101111,
0b10101101,
0b11111101,
0b10101101,
0b11101111,
0b00000000,
0b00000000,
};

boolean pic[COLSNUM][RAWSNUM];

int timer = 0, timerPrev = 0;

int snakeLength=3;
int lastDir = 1;
int coount = 0;
Point food;

Snake head = {0,2,down};
Snake body[(COLSNUM*RAWSNUM)-2] = {NULL};

void draw(boolean pic[COLSNUM][RAWSNUM]){
	byte col;
	for(int raw = 0; raw < RAWSNUM; raw++){
		col=0;
		for(int bitInCol = COLSNUM-1; bitInCol >= 0; bitInCol--){
			col = col + (pic[raw][bitInCol] << bitInCol);	
		}
		digitalWrite(SS_PIN,LOW);
		SPI.transfer(0xFF ^ (1<<raw));
		SPI.transfer(col);
		digitalWrite(SS_PIN,HIGH);
		delay(MATRIX_REFRESH_TIME_MS);
	}
}


Snake generateHead(Snake head){
	head.dir = getDir(head.dir);
	switch(head.dir){
		case down:		head.y += 1; break;
		case up:		head.y -= 1; break;
		case left:		head.x -=1; break;
		case right:		head.x +=1; break;
	}
	lastDir = head.dir;
	if(head.x>(COLSNUM-1)){
		head.x = 0;
	}
	else{ 
		if(head.x<0){
			head.x = COLSNUM-1;
		}
	}
	if(head.y>(RAWSNUM-1)){
		head.y = 0;
	}
	else{ 
		if(head.y<0){
			head.y = RAWSNUM-1;
		}
	}
	if(pic[head.y][head.x] == 1 && head.x != food.x && head.y != food.y){
		TheDeath();
	}
	pic[head.y][head.x] = 1;
	return head;
}
void TheDeath(){
	for(int x1 = 0; x1 < COLSNUM; x1++){
		for(int y1 = 0; y1 < RAWSNUM; y1++){	
			pic[x1][y1]=0;
		}
	}
	delay(2000);
	asm volatile("jmp 0x00");
}

Snake generateBody(Snake body[(COLSNUM*RAWSNUM)-2]){
	clearMatrix(); 										//почему именно здесь?
	// думаю, что очищать всю матрицу, а затем рисовать всё снова - 
	// не самая лучшая идея. Давай стирать только хвост и все.
	for(int gbi = snakeLength-2; gbi >= 1; gbi--){
		body[gbi]=body[gbi-1];
		pic[body[gbi].y][body[gbi].x] = 1;
	}
	body[0] = head;
	pic[body[0].y][body[0].x] = 1;
	return body[62];
}

void clearMatrix(){
	for(int x1 = 0; x1 < COLSNUM; x1++){
		for(int y1 = 0; y1 < RAWSNUM; y1++){	
			pic[x1][y1]=0;
		}
	}
}

void generateFood(){
//	food = {random(8),random(8)};
	food.x = random(COLSNUM);
	food.y = random(RAWSNUM);
	if (pic[food.x][food.y] != 0){
		generateFood();
	}
}

Dirs getDir(Dirs dir){
	if(analogRead(X_AXE_PIN)>RIGHT_THRESHOLD && lastDir != right)	return left;
	if(analogRead(X_AXE_PIN)<LEFT_THRESHOLD && lastDir != left)		return right;
	if(analogRead(Y_AXE_PIN)>UP_THRESHOLD && lastDir != down)		return up;
	if(analogRead(Y_AXE_PIN)<DOWN_THRESHOLD && lastDir != up)		return down;
	return dir;
}

void setup(){
	randomSeed(analogRead(A5));
	SPI.begin();
  	pinMode(SS_PIN, OUTPUT);
	pinMode(Z_AXE_PIN, INPUT_PULLUP);
	generateFood();
}
void loop(){
	timer = millis();
	if((timer - timerPrev) >= STEP_TIME){
		body[(COLSNUM*RAWSNUM)-2] = generateBody(body);
		if(head.x == food.x && head.y == food.y){
			snakeLength++;
			generateFood();
		}
		head = generateHead(head);
		timerPrev = timer;
		pic[food.y][food.x] = 1;
	}
	draw(pic);
}
