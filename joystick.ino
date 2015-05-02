//Beta v 1.0

#include <SPI.h>
#include "snake.h"

#define STEP_TIME 200 
#define MATRIX_REFRESH_TIME_MS 1

/** Dirs: 
* 	0 - up 
*	1 - down 
*	2 - left 	
*	3 - right
**/

int blinkTime = 0;

boolean pic[8][8];

int timer = 0, timerPrev = 0;

int snakeLength=3;
int lastDir = 1;
int coount = 0;
Point food;
int i = 3;
int t = 1;

Snake head = {0,2,down};
Snake body[62] = {NULL};

boolean numbers[10][8][4] = {
{
{1,1,1,1}, //0
{1,0,0,1},
{1,0,0,1},
{1,0,0,1},
{1,0,0,1},
{1,0,0,1},
{1,0,0,1},
{1,1,1,1}
},

{
{0,0,0,1}, //1
{0,0,1,1},
{0,1,0,1},
{1,0,0,1},
{0,0,0,1},
{0,0,0,1},
{0,0,0,1},
{0,0,0,1} 
},

{
{1,1,1,1}, //2
{1,0,0,1},
{0,0,0,1},
{0,0,0,1},
{0,0,1,0},
{0,1,0,0},
{1,0,0,0},
{1,1,1,1}
},

{
{1,1,1,1}, //3
{0,0,0,1},
{0,0,0,1},
{0,0,0,1},
{1,1,1,1},
{0,0,0,1},
{0,0,0,1},
{1,1,1,1}
},

{
{1,0,0,1}, //4
{1,0,0,1},
{1,0,0,1},
{1,0,0,1},
{1,1,1,1},
{0,0,0,1},
{0,0,0,1},
{0,0,0,1}
},

{
{1,1,1,1},
{1,0,0,0}, //5
{1,0,0,0},
{1,0,0,0},
{1,1,1,1},
{0,0,0,1},
{0,0,0,1},
{1,1,1,1}
},

{
{1,1,1,1}, //6
{1,0,0,0},
{1,0,0,0},
{1,0,0,0},
{1,1,1,1},
{1,0,0,1},
{1,0,0,1},
{1,1,1,1}
},

{
{1,1,1,1}, //7
{0,0,0,1},
{0,0,0,1},
{0,0,0,1},
{0,0,0,1},
{0,0,0,1},
{0,0,0,1},
{0,0,0,1}
},

{
{1,1,1,1}, //8
{1,0,0,1},
{1,0,0,1},
{1,0,0,1},
{1,1,1,1},
{1,0,0,1},
{1,0,0,1},
{1,1,1,1}
},
{
{1,1,1,1}, //9
{1,0,0,1},
{1,0,0,1},
{1,0,0,1},
{1,1,1,1},
{0,0,0,1},
{0,0,0,1},
{1,1,1,1}
}
};


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
		delay(MATRIX_REFRESH_TIME_MS);
	}
}

void drawRecord(int rec){
	int tens = rec/10;
	int units = rec%10;
	for(int x=0; x<4; x++){
	    for(int y=0; y<8; y++){
	 		pic[y][x] = numbers[tens][y][x];       
	    }
	}
	for(int x=0; x<4; x++){
	    for(int y=0; y<8; y++){
	 		pic[y][x+4] = numbers[units][y][x];       
	    }
	}
	draw(pic);
}

Snake generateHead(Snake head){
	head.dir = getDir(head.dir);
	switch(head.dir){
		case down:		head.y += 1; break;
		case up:		head.y -= 1; break;
		case left:		head.x -=1; break;
		case right:		head.x +=1; break;
	}
	lastDir=head.dir;
	if(head.x>7){
		head.x = 0;
	}
	else{ 
		if(head.x<0){
			head.x = 7;
		}
	}
	if(head.y>7){
		head.y = 0;
	}
	else{ 
		if(head.y<0){
			head.y = 7;
		}
	}
	if(pic[head.y][head.x] == 1 && head.x != food.x && head.y != food.y){
		TheDeath();
	}
	pic[head.y][head.x] = 1;
	return head;
}
void TheDeath(){
	int time = 0;
	while(time != 1050){
		drawRecord(snakeLength-2);
		time++;
		if(digitalRead(Z_AXE_PIN)==1){
			time = 1050;
		}
	}
	delay(350);
	asm volatile("jmp 0x00");
}

Snake generateBody(Snake body[62]){

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
	food.x= random(8);
	food.y= random(8);
	if (pic[food.x][food.y] != 0){
		generateFood();
	}
}

void playMusic() {
	if(t%2 == 0){	
		noTone(5);
		tone(5, i*100, 300);
		i++;
	}
	else{
		noTone(5);
		tone(5, i*200, 300);
		i++;
	}
	if(i>6){
		i=3;
		t++;
	}
	
	if(t>24){
		noTone(5);
		t=1;
	}
}

Dirs getDir(Dirs dir){
	if(analogRead(X_AXE_PIN)>RIGHT_THRESHOLD && lastDir != right)	return left;
	if(analogRead(X_AXE_PIN)<LEFT_THRESHOLD && lastDir != left)		return right;
	if(analogRead(Y_AXE_PIN)>UP_THRESHOLD && lastDir != down)		return up;
	if(analogRead(Y_AXE_PIN)<DOWN_THRESHOLD && lastDir != up)		return down;
	return dir;
}

boolean pause[8][8]={
{0,0,0,0,0,0,0,0},
{0,1,1,0,0,1,1,0},
{0,1,1,0,0,1,1,0},
{0,1,1,0,0,1,1,0},
{0,1,1,0,0,1,1,0},
{0,1,1,0,0,1,1,0},
{0,1,1,0,0,1,1,0},
{0,0,0,0,0,0,0,0}
};

void setup(){
	randomSeed(analogRead(A5));
	SPI.begin();
  	pinMode(SS_PIN, OUTPUT);
  	pinMode(5, OUTPUT);
	pinMode(Z_AXE_PIN, INPUT_PULLUP);
	generateFood();
}
void loop(){
	timer = millis();
	if((timer - timerPrev) >= STEP_TIME){
		clearMatrix();
		body[62] = generateBody(body);
		head = generateHead(head);
		if(head.x == food.x && head.y == food.y){
			snakeLength++;
			generateFood();
		}
		pic[food.y][food.x] = 1;
		playMusic();
		timerPrev = timer;
	}
	draw(pic);
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