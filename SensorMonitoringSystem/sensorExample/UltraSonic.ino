// Visual Micro is in vMicro>General>Tutorial Mode
// 
/*
    Name:       UltraSonic.ino
    Created:	2019-11-22 오전 3:19:45
    Author:     DESKTOP-1OIDAOU\opera
*/

// Define User Types below here or use a .h file
//


// Define Function Prototypes that use User Types below here or use a .h file
//


// Define Functions below here or use other .ino or cpp files
//

const int pin_trigger = D1;
const int pin_echo = D2;
//const int pin_led = D3;

// the setup function runs once when you press reset or power the board
void setup()
{
	Serial.begin(9600);
	pinMode(pin_trigger, OUTPUT);
	pinMode(LED_BUILTIN, OUTPUT);
	pinMode(pin_echo, INPUT);
}

// the loop function runs over and over again until power down or reset
void loop()
{
	float duration, distance;
	digitalWrite(LED_BUILTIN, HIGH);
	digitalWrite(pin_trigger, HIGH);
	delay(10);
	digitalWrite(pin_trigger, LOW);

	duration = pulseIn(pin_echo, HIGH);

	distance = ((float)(340 * duration) / 10000) / 2.0;

	Serial.printf("%f cm\n", distance);

	delay(1000);
	digitalWrite(LED_BUILTIN, LOW);
	delay(500);
}
