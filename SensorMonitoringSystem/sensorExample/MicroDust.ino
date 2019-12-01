// Define User Types below here or use a .h file
//

// Define Function Prototypes that use User Types below here or use a .h file
//


// Define Functions below here or use other .ino or cpp files
//

// The setup() function runs once each time the micro-controller starts

#include <PMS.h>
#include <SoftwareSerial.h>

SoftwareSerial mySerial(D5, D6);
PMS pms(mySerial);
PMS::DATA data;

void setup()
{
	Serial.begin(9600);   // GPIO1, GPIO3 (TX/RX pin on ESP-12E Development Board)
	mySerial.begin(9600);  // GPIO2 (D4 pin on ESP-12E Development Board)
}

void loop()
{
	if (pms.read(data))
	{
		Serial.printf("PM1.0 : %uug/m3,  PM2.5 : %uug/m3,  PM10 : %uug/m3  \r\n", data.PM_AE_UG_1_0, data.PM_AE_UG_2_5, data.PM_AE_UG_10_0);
	}

	// Do other stuff...
}

/*
#include <SoftwareSerial.h>
SoftwareSerial PMS(D5, D6);
void setup()
{
	Serial.begin(9600);
	PMS.begin(9600);
}
void loop()
{
	unsigned char packet[32] = { 0, };
	int chksum = 0;
	if(PMS.available() >= 32)
	{
		for (int j = 0; j < 32; j++)
		{
			packet[j] = PMS.read();
			if (j < 30)
				chksum += packet[j];
		}
		if (packet[0] != 0x42 || packet[1] != 0x4d)
			Serial.println("Not PMS Packet");
		else if (packet[30] != (unsigned char)(chksum >> 8) || packet[31] != (unsigned char)(chksum))
			Serial.println("Checksum EROOR");
		else
		{
			int PM1_0 = (packet[10] << 8) | packet[11];
			int PM2_5 = (packet[12] << 8) | packet[13];
			int PM10 = (packet[14] << 8) | packet[15];
			Serial.printf("PM1.0 : %dug/m3,  PM2.5 : %dug/m3,  PM10 : %dug/m3  \r\n", PM1_0, PM2_5, PM10);
		}
		chksum = 0;
	}
}
*/