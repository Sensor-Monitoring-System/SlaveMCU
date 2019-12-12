#include <EEPROM.h>
#include <ESP8266HTTPClient.h>
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

typedef struct _WiFiInfo
{
	String SSID;
	String PASS;
} WiFiInfo;


//초기 구성 정보
String ID = "Hyoung";

//const String SSID = "Xperia XZ1_8a47";
//const String PASSWORD = "520954d19428";
const String SSID = "KT_GiGA_2G_Wave2_1AA9";
const String PASSWORD = "6932763abc";
WiFiInfo WIFIINFO;

const int httpPort = 80;
//const String host = "172.25.242.68";
const String HOST = "124.80.117.37";

SoftwareSerial dustSerial(D5, D6); //RX TX
PMS pms(dustSerial);
PMS::DATA dustSensor;

//센서 값들
uint16_t bright;
uint16_t pm1_0;
uint16_t pm2_5;
uint16_t pm10;

//EEPROM 페이지 인덱스
int rom_addr = 0;

void setup()
{
	Serial.begin(9600);
	SaveWiFiInfo(SSID, PASSWORD);
	
	ReadWiFiInfo();
	//dustSerial.begin(9600);
	WiFiSetUp();
}

void loop()
{
	delay(1000);

	/*
	if (pms.read(dustSensor))
	{
		pm1_0 = dustSensor.PM_AE_UG_1_0;
		pm2_5 = dustSensor.PM_AE_UG_2_5;
		pm10 = dustSensor.PM_AE_UG_10_0;

		Serial.printf("UG PM1.0 : %uug/m3,  PM2.5 : %uug/m3,  PM10 : %uug/m3  \r\n", pm1_0, pm2_5, pm10);
	}
	*/

	bright = analogRead(A0);
	Serial.printf("cds : %d  wifi : %d\n", BrightValue, WiFi.status());

	//PostData(HOST, 80, bright);
	//PostDataTest();

	sendData_GET(HOST, 80, ID, "temperature", bright); //온도
	delay(500);
	sendData_GET(HOST, 80, ID, "humidity", bright); //습도
	delay(200);
	sendData_GET(HOST, 80, ID, "ultrasound", bright); //거리
	delay(100);
	sendData_GET(HOST, 80, ID, "illuminance", bright); //밝기
	delay(300);
	sendData_GET(HOST, 80, ID, "particulate_matter2", bright); //미세먼지 pm2.5
	delay(400);
	sendData_GET(HOST, 80, ID, "particulate_matter10", bright); //미세먼지 pm10.0
	delay(400);
	sendData_GET(HOST, 80, "NOT_MY_ID", "particulate_matter10", bright); //다른 아이디
	delay(60000);
}

void WiFiSetUp()
{
	// Connect to WiFi network
	Serial.println();
	Serial.print("Connecting to ");
	Serial.println(WIFIINFO.SSID);

	WiFi.mode(WIFI_OFF);
	delay(1000);
	WiFi.mode(WIFI_STA);  //esp12를 AP에 연결하고 외부 네트워크와 통신하므로 스테이션모드

	WiFi.begin(WIFIINFO.SSID, WIFIINFO.PASS);

	while (WiFi.status() != WL_CONNECTED)
	{
		delay(500);
		Serial.print(".");
	}

	Serial.println("");
	Serial.println("WiFi connected");
	Serial.println("--------------");
	Serial.println(WIFIINFO.SSID);
	Serial.print("IP address: ");
	Serial.println(WiFi.localIP());
	Serial.println("--------------");
}

void PostData(String host, int port, double value)
{
	Serial.print("connecting to ");
	Serial.println(HOST);
	if (WiFi.status() == WL_CONNECTED)
	{
		HTTPClient http;  //Declare an object of class HTTPClient

		String post_url = "http://124.80.117.37:80/texttest";

		//post_url += "temperature?id=Hyoung&value=113.00";
		//post_url += "id?=" + SlaveID + "&";
		//post_url += "temperature?value=";
		//post_url += value;

		Serial.print("begin URL: ");
		Serial.println(post_url);

		http.begin(post_url); //Specify request destination
		//http.addHeader("Content-Type", "application/x-www-form-urlencoded"); //Specify content-type header
		int httpCode = http.POST("text=testset"); //URL POST
		String payload = http.getString();

		Serial.printf("httpcode : %d\n", httpCode);
		Serial.println(payload);

		http.end();
	}
	else
	{
		Serial.println("WiFi NOT CONNECTED");
	}

}

void PostDataTest()
{
	WiFiClient client;
	if(!client.connect(HOST, httpPort))
	{
		Serial.println("연결에 실패");
		return;
	}

	String PostData = "text=testetest1234test";

	client.println("POST /texttest HTTP/1.1");
	client.println("Host: 124.80.117.37:80");
	//client.println("Cache-Control: no-cache");
	//client.println("Content-Type: application/x-www-form-urlencoded");
	client.print("Content-Length: ");
	client.println(PostData.length());
	//client.println();
	//client.println("value=12&id=Hyoung");
	client.println(PostData);

	unsigned long timeout = millis();
	while (client.available() == 0)
	{
		if (millis() - timeout > 5000)
		{
			Serial.println(">>> 클라이언트 시간초과 !");
			client.stop();
			return;
		}
	}

	while (client.connected())
	{
		if (client.available())
		{
			char str = client.read();
			Serial.print(str);
		}
	}
}

void sendData_GET(String host, int port, String id,String sensor_type, double sensor_value)
{
	if (WiFi.status() == WL_CONNECTED)
	{
		HTTPClient http;

		String url = "http://"+ host + ":" + port + "/";
		
		url += sensor_type + "?" + "value=" + sensor_value + "&";
		url += "id=" + id;
		Serial.println("begin :" + url);

		http.begin(url); //Specify request destination

		int httpCode = http.GET(); //Send the request

		Serial.printf("httpCode : %d\n", httpCode);
		if (httpCode > 0)
		{ //Check the returning code
			String payload = http.getString();   //Get the request response payload
			Serial.println(payload);                     //Print the response payload
		}
		http.end();   //Close connection
	}
	//delay(10000);    //Send a request every 10 seconds
}

void EEPROMClear()
{
	EEPROM.begin(512);
	// write a 0 to all 512 bytes of the EEPROM
	for (int i = 0; i < 512; i++)
	{
		EEPROM.write(i, 0);
	}
	EEPROM.end();
	// turn the LED on when we're done
	pinMode(BUILTIN_LED, OUTPUT);
	digitalWrite(BUILTIN_LED, HIGH);
}

int EEPROMWriteString(int addr, String str)
{
	int i;
	EEPROM.begin(512);
	for (i = 0; i < str.length(); i++)
	{
		EEPROM.write(i + addr, str[i]);
	}
	EEPROM.commit();
	EEPROM.end();
	return i; //마지막 페이지 인덱스 반환
}

String EEPROMReadString(int addr, int size)
{
	String str;
	int i;
	EEPROM.begin(512);
	for (i = addr; i < size; i++)
	{
		str += char(EEPROM.read(i));
	}
	EEPROM.end();
	return str;
}

//모든 페이지를 십진수로 출력
void EEPROMprint()
{
	int i;
	int val;
	EEPROM.begin(512);
	for (i = 0; i < 512; i++)
	{
		val = EEPROM.read(i);
		Serial.print(i);
		Serial.print("\t");
		Serial.print(val, DEC);
		Serial.println();
		delay(10);
	}
	EEPROM.end();
}

//와이파이 이름, 비밀번호를 저장
//이름의 길이를 pos에
//전체 길이를 size에
//0부터 pos까지 읽으면 이름
//pos부터 size까지 읽으면 비밀번호
void SaveWiFiInfo(String ssid, String pass)
{
	int i = 0;
	int pos = ssid.length();
	String concat = ssid + pass;
	int size = concat.length();

	EEPROM.begin(512);
	EEPROM.write(0, size);
	EEPROM.write(1, pos);
	EEPROM.commit();

	Serial.println("save :" + concat);
	EEPROMWriteString(2, concat);
	EEPROM.end();
}

void ReadWiFiInfo()
{
	//[0] = size
	//[1] = pos
	//[2~size] = WiFi Info
	int i;
	int pos = 0;
	int size = 0;
	String ssid;
	String pass;

	EEPROM.begin(512);
	size = EEPROM.read(0);
	pos = EEPROM.read(1);

	//+2는 size와 pos값을 건너 뛰는 오프셋
	for (i = 2; i < pos + 2; i++)
		ssid += char(EEPROM.read(i));

	for(i = pos + 2; i < size + 2; i++)
		pass += char(EEPROM.read(i));

	Serial.println();
	Serial.println("load SSID :" + ssid);
	Serial.println("load PASS :" + pass);

	WIFIINFO.SSID = ssid;
	WIFIINFO.PASS = pass;

	EEPROM.end();
}