#define PIEZO_PIN 2

void init_piezo(){
	pinMode(PIEZO_PIN,OUTPUT);
	pinMode(PIEZO_PIN,INPUT_PULLDOWN);

	digitalWrite(PIEZO_PIN,HIGH);
	delay(100);
	digitalWrite(PIEZO_PIN,LOW);

	delay(40);

	digitalWrite(PIEZO_PIN,HIGH);
	delay(100);
	digitalWrite(PIEZO_PIN,LOW);

}

void single_beep(){

	digitalWrite(PIEZO_PIN,HIGH);
	delay(200);
	digitalWrite(PIEZO_PIN,LOW);

}