#define X_AXE_PIN A0
#define Y_AXE_PIN A1

#define UP_THRESHOLD 900
#define DOWN_THRESHOLD 200

#define LEFT_THRESHOLD 200
#define RIGHT_THRESHOLD 900

void setup()
{
	pinMode(4, INPUT_PULLUP);
	pinMode(5, OUTPUT);
	pinMode(6, OUTPUT);
	pinMode(7, OUTPUT);
	pinMode(8, OUTPUT);
	Serial.begin(9600);
}
int p;
/*void loop()
{
	Serial.print("x = ");
	Serial.print(analogRead(A0));
	Serial.print("     y = ");
	Serial.println(analogRead(A1));
	delay(100);
}*/
void loop()
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
}	