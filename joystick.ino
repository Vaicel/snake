
#include "snake.h"

#define STEP_TIME 200 


int pauseBlinkTime = 0;

int voidCellsArrayLength = 0;
int voidCells[CELLS_NUM-3][2];
// массив voidCells должен иметь тип Point!

int timer = 0, timerPrev = 0;


Snake snake = Snake();
Matrix matrix = Matrix(SS_PIN);

Point food;

Flesh head = {0,2,down};
Flesh body[CELLS_NUM-2] = {NULL};

void detectVoidCells(){
	for(int y=0; y<RAWS_NUM; y++){
		for(int x=0; x<COLS_NUM; x++){
			if(matrix.pic[y][x] == 0){
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
	if (matrix.pic[head.y][head.x] == 1 && head.x != food.x && head.y != food.y) snake.isDead();
	matrix.pic[head.y][head.x] = 1;
	return head;
}

Flesh generateBody(Flesh body[CELLS_NUM-2]){
	for(int gbi = snake.length-2; gbi >= 1; gbi--){
		body[gbi]=body[gbi-1];
		matrix.pic[body[gbi].y][body[gbi].x] = 1;
	}
	body[0] = head;
	matrix.pic[body[0].y][body[0].x] = 1;
	return body[CELLS_NUM-2];
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
		delay(300);
		while(digitalRead(Z_AXE_PIN)==0){
			matrix.drawPause();
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

void setup(){
	randomSeed(analogRead(A5));
	pinMode(Z_AXE_PIN, INPUT_PULLUP);
	detectVoidCells();
	generateFood();
	voidCellsArrayLength=0;
}

void loop(){
	timer = millis();
	if((timer - timerPrev) >= STEP_TIME){
		matrix.clear();
		body[CELLS_NUM-2] = generateBody(body);
		head = generateHead(head);
		if(head.x == food.x && head.y == food.y){
			snake.length++;
			detectVoidCells();
			generateFood();
			voidCellsArrayLength=0;
		}
		matrix.pic[food.y][food.x] = 1;
		timerPrev = timer;
	}
	matrix.drawFrame(matrix.pic);
	if(digitalRead(Z_AXE_PIN)!=0){
		paused();
	}
          
}