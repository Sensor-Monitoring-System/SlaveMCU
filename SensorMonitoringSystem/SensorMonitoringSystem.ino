

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

#define BrightValue analogRead(A0)
#define Temperature //
#define Humidity //
#define DustPM2_5 //
#define DustPM10_0 //

const char* ssid = "Xperia XZ1_8a47";
const char* password = "520954d19428";
const int httpPort = 80;
const String KMA_url = "/wid/queryDFSRSS.jsp?zone=4146358500";
const char* SERVER = "www.kma.go.kr";
const char* host = "172.25.242.68";

WiFiClient client;
SoftwareSerial mySerial(D5, D6); //RX TX
PMS pms(mySerial);
PMS::DATA dustSensor;

//센서 값들
uint16_t bright;
uint16_t pm1_0;
uint16_t pm2_5;
uint16_t pm10;
void setup()
{
	Serial.begin(9600);
	mySerial.begin(9600);

	WiFiSetUp();

}

void loop()
{
	if (pms.read(dustSensor))
	{
		pm1_0 = dustSensor.PM_AE_UG_1_0;
		pm2_5 = dustSensor.PM_AE_UG_2_5;
		pm10 = dustSensor.PM_AE_UG_10_0;

		Serial.printf("UG PM1.0 : %uug/m3,  PM2.5 : %uug/m3,  PM10 : %uug/m3  \r\n", pm1_0, pm2_5, pm10);
	}
	//bright = analogRead(A0);
	Serial.printf("cds : %d\n", BrightValue);

	SendData(BrightValue);
	//delay(5000);
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
	Serial.println("--------------");
	Serial.println(ssid);
	Serial.print("IP address: ");
	Serial.println(WiFi.localIP());
	Serial.println("--------------");
}

void SendData(double value)
{
	Serial.print("connecting to ");
	Serial.print(host);

	// Use WiFiClient class to create TCP connections
	if (!client.connect(host, httpPort))
	{
		Serial.println(" is failed");
		return;
	}
	else
	{
		Serial.println(" is Succese");
	}

	// We now create a URI for the request
	String url = "/";
	//  url += streamId;
	//  url += "?private_key=";
	//  url += privateKey;
	url += "bright?value=";
	url += value;

	Serial.print("Requesting URL: ");
	Serial.println(url);

	// This will send the request to the server
	client.print(String("GET ") + url + " HTTP/1.1\r\n" +
		"Host: " + host + "\r\n" +
		"Connection: close\r\n\r\n");
	int timeout = millis() + 5000;
	while (client.available() == 0)
	{
		if (timeout - millis() < 0)
		{
			Serial.println(">>> Client Timeout !");
			//client.stop();
			return;
		}
	}

	// Read all the lines of the reply from server and print them to Serial
	while (client.available())
	{
		String line = client.readStringUntil('\r');
		Serial.println("Reading line\n");
		Serial.print(line);
	}

	Serial.println();
	Serial.println("closing connection");
}