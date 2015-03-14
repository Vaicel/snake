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

boolean pic [8][8]{

{0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0},
{0,0,0,0,1,0,0,0},
{0,0,0,0,0,0,0,0},
{0,0,0,1,0,0,0,0},
{0,0,0,0,0,0,0,0},
{0,0,0,0,0,1,0,0},
{0,0,0,0,1,0,0,0}
};

byte x;

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
	timer=millis();
	if(timer - timePrev == DRAW_TIME){
		draw();
		timePrev=timer;
	}
}

void draw(){
	for(int t = 0; t<8; t++){
		x=0;
		for(int v = 7; v>=0; v--){
			x = x+(pic[t][v] << v);	
		}
		digitalWrite(SS_PIN,LOW);
		SPI.transfer(0xFF ^ 1<<t);
		SPI.transfer(x);
		digitalWrite(SS_PIN,HIGH);
		delay(1);
	}
}	