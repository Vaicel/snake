#include <SPI.h>
#define X_AXE_PIN A0
#define Y_AXE_PIN A1

#define UP_THRESHOLD 900
#define DOWN_THRESHOLD 200

#define LEFT_THRESHOLD 200
#define RIGHT_THRESHOLD 900

#define SS_PIN 10
#define COLS_NUM 8


void setup()
{
	SPI.begin();
  	pinMode(SS_PIN, OUTPUT);
	pinMode(4, INPUT_PULLUP);
}
int p;

/*void loop()
{
	if(analogRead(X_AXE_PIN)>RIGHT_THRESHOLD){
		digitalWrite(p, LOW);
		while(analogRead(X_AXE_PIN)>RIGHT_THRESHOLD){
			digitalWrite(5, HIGH);
			p = 5;
		}
	}
	if(analogRead(Y_AXE_PIN)>UP_THRESHOLD){
		digitalWrite(p, LOW);
		while(analogRead(Y_AXE_PIN)>UP_THRESHOLD){
			digitalWrite(6, HIGH);
			p = 6;	
		}
	}
	if(analogRead(X_AXE_PIN)<LEFT_THRESHOLD){
		digitalWrite(p, LOW);
		while(analogRead(X_AXE_PIN)<LEFT_THRESHOLD){
			digitalWrite(7, HIGH);
			p = 7;
		}
	}
	if(analogRead(Y_AXE_PIN)<DOWN_THRESHOLD){
		digitalWrite(p, LOW);
		while(analogRead(Y_AXE_PIN)<DOWN_THRESHOLD){
			digitalWrite(8, HIGH);
			p = 8;
		}
	}
}*/
void loop()
	{
		digitalWrite(SS_PIN,LOW);
		SPI.transfer(0xFF^ 1<<map(analogRead(A1), 0, 1022, 0, 7));
		SPI.transfer(1<<map(analogRead(A0), 0, 1021, 0, 7));	
		digitalWrite(SS_PIN,HIGH);
	}	