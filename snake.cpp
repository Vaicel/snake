#include "snake.h"
#include "numbers.h"
#include "pause.h"
#include <Arduino.h>
#include <SPI.h>

boolean Matrix::pic[RAWS_NUM][COLS_NUM]={NULL};


Matrix::Matrix(int sspin){
	int _sspin = sspin;     //??
	pinMode(_sspin, OUTPUT); //??
	SPI.begin();
}

Matrix::~Matrix(){
	//simply nothing
}

void Matrix::drawFrame(boolean frame[RAWS_NUM][COLS_NUM]){
	byte col;
	for(int raw = 0; raw < RAWS_NUM; raw++){
		col=0;
		for(int bitInCol = RAWS_NUM-1; bitInCol >= 0; bitInCol--){
			col += (frame[raw][bitInCol] << bitInCol);	
		}
		digitalWrite(SS_PIN,LOW);  //??
		SPI.transfer(0xFF ^ (1<<raw));
		SPI.transfer(col);
		digitalWrite(SS_PIN,HIGH);  //??
		delay(MATRIX_REFRESH_TIME_MS);
	}
}

void Matrix::drawRecord(int rec){
	int tens = rec/10;
	int units = rec%10;
	for(int x=0; x<COLS_NUM/2; x++){
	    for(int y=0; y<RAWS_NUM; y++){
	 		Matrix::pic[y][x] = numbers[tens][y][x];       
	    }
	}
	for(int x=0; x<COLS_NUM/2; x++){
	    for(int y=0; y< RAWS_NUM; y++){
	 		Matrix::pic[y][x+4] = numbers[units][y][x];       
	    }
	}
	Matrix::drawFrame(Matrix::pic);
}

void Matrix::drawPause(){
	Matrix::drawFrame(pause);
	// заменить на генерацию картинки
	// вместо использования массива
}

void Matrix::clear(){
	for(int x1 = 0; x1 < COLS_NUM; x1++){
		for(int y1 = 0; y1 < RAWS_NUM; y1++){	
			Matrix::pic[x1][y1]=0;
		}
	}
}



Snake::Snake(){
	Snake::length = 3;
	Snake::lastDir = down;
}

Snake::~Snake(){
	//simply nothing	
}

void Snake::isDead(){
	int time = 0;
	while(time != 1050){
		Matrix::drawRecord(Snake::length-3);
		time++;
		if(digitalRead(Z_AXE_PIN)==1){
			break;
		}
	}
	delay(350);
	asm volatile("jmp 0x00");
}

