/******
*v 1.1*
******/

#include <SPI.h>
#include "snake.h"
#include "numbers.h"

#define STEP_TIME 200 

int blinkTime = 0;

boolean pic[RAWS_NUM][COLS_NUM];

int voidCellsArrayLength = 0;
int voidCells[CELLS_NUM-3][2];

int timer = 0, timerPrev = 0;

int snakeLength=3;
int lastDir = 1;
int coount = 0;

Point food;

Snake head = {0,2,down};
Snake body[CELLS_NUM-2] = {NULL};

void detectVoidCells(boolean pic[RAWS_NUM][COLS_NUM]){
	for(int y=0; y<RAWS_NUM; y++){
		for(int x=0; x<COLS_NUM; x++){
			if(pic[y][x] == 0){
				voidCells[voidCellsArrayLength][0]=y;
				voidCells[voidCellsArrayLength][1]=x;
				voidCellsArrayLength++;
			}
		}
	}                   
}

void drawRecord(int rec){
	int tens = rec/10;
	int units = rec%10;
	for(int x=0; x<COLS_NUM/2; x++){
	    for(int y=0; y<RAWS_NUM; y++){
	 		pic[y][x] = numbers[tens][y][x];       
	    }
	}
	for(int x=0; x<COLS_NUM/2; x++){
	    for(int y=0; y< RAWS_NUM; y++){
	 		pic[y][x+4] = numbers[units][y][x];       
	    }
	}
	draw(pic);
}

Snake generateHead(Snake head){
	head.dir = getDir(head.dir);
	switch(head.dir){
		case down:		head.y += 1;	break;
		case up:		head.y -= 1;	break;
		case left:		head.x -= 1;	break;
		case right:		head.x += 1;	break;
	}
	lastDir=head.dir;
	if ((head.x > COLS_NUM-1) || (head.x < 0)) head.x = COLS_NUM - abs(head.x);
	if ((head.y > RAWS_NUM-1) || (head.y < 0)) head.y = RAWS_NUM - abs(head.y);
	if (pic[head.y][head.x] == 1 && head.x != food.x && head.y != food.y) theDeath();
	pic[head.y][head.x] = 1;
	return head;
}

void theDeath(){
	int time = 0;
	while(time != 1050){
		drawRecord(snakeLength-3);
		time++;
		if(digitalRead(Z_AXE_PIN)==1){
			break;
		}
	}
	delay(350);
	asm volatile("jmp 0x00");
}

Snake generateBody(Snake body[CELLS_NUM-2]){
	for(int gbi = snakeLength-2; gbi >= 1; gbi--){
		body[gbi]=body[gbi-1];
		pic[body[gbi].y][body[gbi].x] = 1;
	}
	body[0] = head;
	pic[body[0].y][body[0].x] = 1;
	return body[CELLS_NUM-2];
}

void clearMatrix(){
	for(int x1 = 0; x1 < COLS_NUM; x1++){
		for(int y1 = 0; y1 < RAWS_NUM; y1++){	
			pic[x1][y1]=0;
		}
	}
}

void generateFood(){
	int FoodRand = random(voidCellsArrayLength-1);
	food.y = voidCells[FoodRand][0];
	food.x = voidCells[FoodRand][1];
}

Dirs getDir(Dirs dir){
	if(analogRead(X_AXE_PIN)>RIGHT_THRESHOLD && lastDir != right)	return left;
	if(analogRead(X_AXE_PIN)<LEFT_THRESHOLD && lastDir != left)		return right;
	if(analogRead(Y_AXE_PIN)>UP_THRESHOLD && lastDir != down)		return up;
	if(analogRead(Y_AXE_PIN)<DOWN_THRESHOLD && lastDir != up)		return down;
	return dir;
}

void paused(){
	if(digitalRead(Z_AXE_PIN)!=0){
		delay(300);
		while(digitalRead(Z_AXE_PIN)==0){
			draw(pause);
			if(blinkTime == 100){
				delay(100);
				blinkTime=0;
			}
			else{
				blinkTime++;
			}
		}
		delay(1000);
	}
}

void setup(){
	randomSeed(analogRead(A5));
          pinMode(SS_PIN, OUTPUT);
	pinMode(Z_AXE_PIN, INPUT_PULLUP);
          SPI.begin();
	detectVoidCells(pic);
	generateFood();
	voidCellsArrayLength=0;
}

void loop(){
	timer = millis();
	if((timer - timerPrev) >= STEP_TIME){
		clearMatrix();
		body[CELLS_NUM-2] = generateBody(body);
		head = generateHead(head);
		if(head.x == food.x && head.y == food.y){
			snakeLength++;
			detectVoidCells(pic);
			generateFood();
			voidCellsArrayLength=0;
		}
		pic[food.y][food.x] = 1;
		timerPrev = timer;
	}
	draw(pic);
          paused();
}