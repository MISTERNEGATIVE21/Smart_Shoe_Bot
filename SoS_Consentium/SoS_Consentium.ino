#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>
#include <UniversalTelegramBot.h>
#include <HTTPClient.h>
String receivekey="";
String boardkey="";
const char* ssid = "smartshoe";
const char* password = "smartshoe";
String serverUrl = "https://consentiuminc.online/api/board/getdata/recent?receivekey=" + receivekey + "&boardkey=" + boardkey;
const int lPin = 26;
const int knockSensor = 34;
const int threshold = 100;

#define BOT_TOKEN "xxx"
#define CHAT_ID xxx  // Change CHAT_ID to the actual chat ID as an integer

WiFiClientSecure secured_client;
UniversalTelegramBot bot(BOT_TOKEN, secured_client);

void setup() {
  pinMode(lPin, OUTPUT);
  Serial.begin(115200);
  connectToWiFi();
}

void loop() {
  alertcheck();
  delay(10);
}

void alertcheck() {
  int sensorReading = analogRead(knockSensor);
  Serial.println(knockSensor);
  if (sensorReading >= threshold) {
    Serial.println("Emergency Alert !");
    digitalWrite(lPin, HIGH);
    sendTelegramLocation();
    delay(3000);
    digitalWrite(lPin, LOW);
  }
  delay(10);
}

void connectToWiFi() {
  WiFi.begin(ssid, password);
  secured_client.setCACert(TELEGRAM_CERTIFICATE_ROOT); // Add root certificate for api.telegram.org
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  configTime(0, 0, "pool.ntp.org"); // get UTC time via NTP
  time_t now = time(nullptr);
  while (now < 24 * 3600)
  {
    Serial.print(".");
    delay(100);
    now = time(nullptr);
  }
  Serial.println("\nConnected to WiFi");
}

void makeGetRequest() {
  HTTPClient http;

  http.begin(serverUrl);
  int httpCode = http.GET();

  if (httpCode == HTTP_CODE_OK) {
    String payload = http.getString();
    Serial.println("Received response:");

    DynamicJsonDocument doc(4096);
    deserializeJson(doc, payload);

    String latitude = doc["sensors"][0]["sensorData"][0]["data"].as<String>();
    String longitude = doc["sensors"][0]["sensorData"][1]["data"].as<String>();

    String Gmaps_link = ("http://maps.google.com/maps?q=" + latitude + "+" + longitude);
    Serial.println(Gmaps_link);

    // Send the location to Telegram
    String message = "Emergency Alert! Location: " + Gmaps_link;
    bot.sendMessage(String(CHAT_ID), message, "" );

  } else {
    Serial.print("HTTP GET request failed, error code: ");
    Serial.println(httpCode);
  }

  http.end();
}

void sendTelegramLocation() {
  makeGetRequest(); // Update the location information
}
