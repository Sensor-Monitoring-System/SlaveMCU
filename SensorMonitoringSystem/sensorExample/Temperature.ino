// Define User Types below here or use a .h file
//


// Define Function Prototypes that use User Types below here or use a .h file
//


// Define Functions below here or use other .ino or cpp files
//

// The setup() function runs once each time the micro-controller starts
#include <DHTesp.h>
DHTesp dht;
uint8_t dhtPin = 10;
void setup()
{
	Serial.begin(9600);
	dht.setup(10, DHTesp::DHT22);
}

// Add the main program code into the continuous loop() function
void loop()
{
	dht.values = dht.getTempAndHumidity();
	printf("T:%f  H:%f\n", dht.values.temperature, dht.values.humidity);

	delay(5000);
}
