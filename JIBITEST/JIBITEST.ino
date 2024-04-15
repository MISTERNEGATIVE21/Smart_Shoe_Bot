#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>
#include <UniversalTelegramBot.h>
#include <HTTPClient.h>
#include <ezButton.h>

String receivekey="be4d63429db2f3e091055b8ac1d39006";
String boardkey="a478901820263aed";
const char* ssid = "smartsos";
const char* password = "smartsos";
String serverUrl = "https://consentiuminc.online/api/board/getdata/recent?receivekey=" + receivekey + "&boardkey=" + boardkey;
const int lPin = 4;
const int BUZZER_PIN = 5;
int channel = 0;
int resolution = 8;
int freq =2000;
#define SHORT_PRESS_TIME 200 // 1000 milliseconds
#define LONG_PRESS_TIME  3000 // 1000 milliseconds

ezButton button(26); // create ezButton object that attach to pin GPIO26

unsigned long pressedTime  = 0;
unsigned long releasedTime = 0;
bool isPressing = false;
bool isLongDetected = false;


int clickCount = 0;  // Counter for button clicks
#define BOT_TOKEN "7193740027:AAFZKxbM1npsSJGKRHpc6cPIAZYZxuJng8U"
#define CHAT_ID 1378489760  // Change CHAT_ID to the actual chat ID as an integer

WiFiClientSecure secured_client;
UniversalTelegramBot bot(BOT_TOKEN, secured_client);

void setup() {
  pinMode(lPin, OUTPUT);
    button.setDebounceTime(50);
  Serial.begin(115200);
   ledcSetup(channel, freq, resolution);
  ledcAttachPin(BUZZER_PIN, channel);
  connectToWiFi();
}

void loop() {
  buttoncheck();
  delay(10);
}


void onlyalert() {
    Serial.println("Emergency Alert !"); // use a counter double tap or check for tripple tab
    digitalWrite(lPin, HIGH);
    sendTelegramLocation();
    delay(3000);
    digitalWrite(lPin, LOW);
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
    pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
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
void buttoncheck() {
  button.loop(); // MUST call the loop() function first

  if (button.isPressed()) {
    if (!isPressing) { // Only assign on initial press
      pressedTime = millis();
      isPressing = true;
      isLongDetected = false;
    }
  }

  if (button.isReleased()) {
    if (isPressing) { // Only proceed if this was a valid press
      isPressing = false;
      releasedTime = millis();

      long pressDuration = releasedTime - pressedTime;

      if (pressDuration >= SHORT_PRESS_TIME && pressDuration <= LONG_PRESS_TIME) {
        Serial.println("A short press is detected");
        ledcWriteTone(channel, 2755);
        onlyalert(); // Alert for short press
        ledcWriteTone(channel, 0);
      }
    }
  }

  // Check for a long press only if the button is still being pressed
  // and a long press has not already been detected
  else if (isPressing && !isLongDetected) {
    long pressDuration = millis() - pressedTime;

    if (pressDuration > LONG_PRESS_TIME) {
      Serial.println("A long press is detected");
      isLongDetected = true;
 
      onlyalert(); // Alert for long press, this time without setting the tone to 0 immediately
    }
  }
}

void sendTelegramLocation() {
  makeGetRequest(); // Update the location information
}
