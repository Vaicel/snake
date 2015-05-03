#include "snake.h"
#include <Arduino.h>
#include <SPI.h>

/*
* Сюда должны переехать все функции из скетча
* В идеале, должы быть классы Snake и Matrix,
* а все остальное - их поля и методы
*/

void draw(boolean pic[8][8]){
	byte col;
	for(int raw = 0; raw < 8; raw++){
		col=0;
		for(int bitInCol = 7; bitInCol >= 0; bitInCol--){
			col += (pic[raw][bitInCol] << bitInCol);	
		}
		digitalWrite(SS_PIN,LOW);
		SPI.transfer(0xFF ^ (1<<raw));
		SPI.transfer(col);
		digitalWrite(SS_PIN,HIGH);
		delay(MATRIX_REFRESH_TIME_MS);
	}
}