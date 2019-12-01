
// Define User Types below here or use a .h file
//


// Define Function Prototypes that use User Types below here or use a .h file
//


// Define Functions below here or use other .ino or cpp files
//
int bright;
// The setup() function runs once each time the micro-controller starts
void setup()
{
	Serial.begin(9600);
}

// Add the main program code into the continuous loop() function
void loop()
{
	bright = analogRead(A0);

	Serial.println(bright);

	delay(1000);

}
