

//const char* ssid = "KT_GiGA_2G_C62E";
//const char* password = "fzfd8ed535";
//const int httpPort = 80;

#include <WiFiUdp.h>
#include <WiFiServerSecure.h>
#include <WiFiServer.h>
#include <WiFiClientSecure.h>
#include <WiFiClient.h>
#include <ESP8266WiFiType.h>
#include <ESP8266WiFiSTA.h>
#include <ESP8266WiFiScan.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266WiFiGeneric.h>
#include <ESP8266WiFiAP.h>
#include <ESP8266WiFi.h>
#include <PMS.h>
#include <DHTesp.h>
#include <SoftwareSerial.h>

const char* ssid = "Xperia XZ1_8a47";
const char* password = "520954d19428";
const int httpPort = 80;
const String KMA_url = "/wid/queryDFSRSS.jsp?zone=4146358500";
const char* SERVER = "www.kma.go.kr";

SoftwareSerial mySerial(D5, D6); //RX TX
PMS pms(mySerial);
PMS::DATA data;
int bright;
void setup()
{
	Serial.begin(9600);
	mySerial.begin(9600);

	WiFiSetUp();

}

void loop()
{
	if (pms.read(data))
	{
		Serial.printf("PM1.0 : %uug/m3,  PM2.5 : %uug/m3,  PM10 : %uug/m3  \r\n", data.PM_AE_UG_1_0, data.PM_AE_UG_2_5, data.PM_AE_UG_10_0);
	}
	bright = analogRead(A0);
	Serial.printf("cds : %d\n", bright);
}

void WiFiSetUp()
{
	// Connect to WiFi network
	Serial.println();
	Serial.print("Connecting to ");
	Serial.println(ssid);

	WiFi.mode(WIFI_STA);  //esp12를 AP에 연결하고 외부 네트워크와 통신하므로 스테이션모드

	WiFi.begin(ssid, password);

	while (WiFi.status() != WL_CONNECTED)
	{
		delay(500);
		Serial.print(".");
	}

	Serial.println("");
	Serial.println("WiFi connected");
}