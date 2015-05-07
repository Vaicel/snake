#include "snake.h"
#include "numbers.h"
#include <Arduino.h>
#include <SPI.h>


Snake::Snake(int sspin){
	int _sspin = sspin;
	pinMode(_sspin, OUTPUT);
	SPI.begin();
	Snake::length = 3;
	Snake::lastDir = down;
}

Snake::~Snake(){
	//simply nothing	
}

void Snake::draw(boolean pic[8][8]){
	byte col;
	for(int raw = 0; raw < RAWS_NUM; raw++){
		col=0;
		for(int bitInCol = RAWS_NUM-1; bitInCol >= 0; bitInCol--){
			col += (pic[raw][bitInCol] << bitInCol);	
		}
		digitalWrite(SS_PIN,LOW);
		SPI.transfer(0xFF ^ (1<<raw));
		SPI.transfer(col);
		digitalWrite(SS_PIN,HIGH);
		delay(MATRIX_REFRESH_TIME_MS);
	}
}

void Snake::isDead(){
	int time = 0;
	while(time != 1050){
		Snake::drawRecord(Snake::length-3);
		time++;
		if(digitalRead(Z_AXE_PIN)==1){
			break;
		}
	}
	delay(350);
	asm volatile("jmp 0x00");
}

void Snake::drawRecord(int rec){
	int tens = rec/10;
	int units = rec%10;
	for(int x=0; x<COLS_NUM/2; x++){
	    for(int y=0; y<RAWS_NUM; y++){
	 		Snake::pic[y][x] = numbers[tens][y][x];       
	    }
	}
	for(int x=0; x<COLS_NUM/2; x++){
	    for(int y=0; y< RAWS_NUM; y++){
	 		Snake::pic[y][x+4] = numbers[units][y][x];       
	    }
	}
	Snake::draw(Snake::pic);
}