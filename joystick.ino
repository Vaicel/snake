/******
*v 1.1* 
******/

#include "snake.h"
#include "pause.h"

#define STEP_TIME 200 

int pauseBlinkTime = 0;

int voidCellsArrayLength = 0;
int voidCells[CELLS_NUM-3][2];

int timer = 0, timerPrev = 0;


Snake snake = Snake(SS_PIN);

Point food;

Flesh head = {0,2,down};
Flesh body[CELLS_NUM-2] = {NULL};

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



Flesh generateHead(Flesh head){
	head.dir = getDir(head.dir);
	switch(head.dir){
		case down:		head.y += 1;	break;
		case up:		head.y -= 1;	break;
		case left:		head.x -= 1;	break;
		case right:		head.x += 1;	break;
	}
	snake.lastDir=head.dir;
	if ((head.x > COLS_NUM-1) || (head.x < 0)) head.x = COLS_NUM - abs(head.x);
	if ((head.y > RAWS_NUM-1) || (head.y < 0)) head.y = RAWS_NUM - abs(head.y);
	if (snake.pic[head.y][head.x] == 1 && head.x != food.x && head.y != food.y) snake.isDead();
	snake.pic[head.y][head.x] = 1;
	return head;
}

Flesh generateBody(Flesh body[CELLS_NUM-2]){
	for(int gbi = snake.length-2; gbi >= 1; gbi--){
		body[gbi]=body[gbi-1];
		snake.pic[body[gbi].y][body[gbi].x] = 1;
	}
	body[0] = head;
	snake.pic[body[0].y][body[0].x] = 1;
	return body[CELLS_NUM-2];
}

void clearMatrix(){
	for(int x1 = 0; x1 < COLS_NUM; x1++){
		for(int y1 = 0; y1 < RAWS_NUM; y1++){	
			snake.pic[x1][y1]=0;
		}
	}
}

void generateFood(){
	int FoodRand = random(voidCellsArrayLength-1);
	food.y = voidCells[FoodRand][0];
	food.x = voidCells[FoodRand][1];
}

Dirs getDir(Dirs dir){
	if(analogRead(X_AXE_PIN)>RIGHT_THRESHOLD && snake.lastDir != right)	return left;
	if(analogRead(X_AXE_PIN)<LEFT_THRESHOLD && snake.lastDir != left)		return right;
	if(analogRead(Y_AXE_PIN)>UP_THRESHOLD && snake.lastDir != down)		return up;
	if(analogRead(Y_AXE_PIN)<DOWN_THRESHOLD && snake.lastDir != up)		return down;
	return dir;
}

void paused(){
	if(digitalRead(Z_AXE_PIN)!=0){
		delay(300);
		while(digitalRead(Z_AXE_PIN)==0){
			snake.draw(pause);
			if(pauseBlinkTime == 100){
				delay(100);
				pauseBlinkTime=0;
			}
			else{
				pauseBlinkTime++;
			}
		}
		delay(1000);
	}
}

void setup(){
	randomSeed(analogRead(A5));
	pinMode(Z_AXE_PIN, INPUT_PULLUP);
	detectVoidCells(snake.pic);
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
			snake.length++;
			detectVoidCells(snake.pic);
			generateFood();
			voidCellsArrayLength=0;
		}
		snake.pic[food.y][food.x] = 1;
		timerPrev = timer;
	}
	snake.draw(snake.pic);
          paused();
}