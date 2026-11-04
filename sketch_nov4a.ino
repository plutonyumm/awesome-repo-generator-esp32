#include <WiFi.h>
#include <HTTPClient.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ArduinoJson.h> 

// pls edit line 15 and 16 so that it works on your own device

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const char* ssid = "ur_wifi_name"; // change me
const char* password = "ur_wifi_password"; // change me
const int buttonPin = 4;

void setup() {
  Serial.begin(115200);

  pinMode(buttonPin, INPUT_PULLDOWN); 

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("there's a wiring diagram in the repo btw"));
    for(;;);
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("connecting");
  display.display();

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nconnected");
  randomSeed(analogRead(34)); 

  fetchRandomRepo();
}

void loop() {
  if (digitalRead(buttonPin) == HIGH) {
    Serial.println("button pressed -- rolling");
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("rolling");
    display.display();
    delay(300);
    fetchRandomRepo();
    while(digitalRead(buttonPin) == HIGH) {
      delay(10);
    }
  }
}

void fetchRandomRepo() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    
    int randomPage = random(1, 50);
    String url = "https://api.github.com/search/repositories?q=topic:awesome&sort=stars&order=desc&per_page=1&page=" + String(randomPage);
    
    http.begin(url);
    http.addHeader("User-Agent", "ESP32-Thing-9000"); 
    
    int httpCode = http.GET();

    if (httpCode == 200) {
      String payload = http.getString();
      JsonDocument doc; 
      DeserializationError error = deserializeJson(doc, payload);

      if (!error && doc["items"].size() > 0) {
        String repoName = doc["items"][0]["name"].as<String>();
        String ownerName = doc["items"][0]["owner"]["login"].as<String>();
        int stars = doc["items"][0]["stargazers_count"];

        if (ownerName.length() > 14) ownerName = ownerName.substring(0, 12) + "..";
        if (repoName.length() > 14) repoName = repoName.substring(0, 12) + "..";

        display.clearDisplay();
        display.setTextSize(1);
        display.setTextColor(WHITE);
        
        display.setCursor(0, 1);
        display.println("awesome repo gen 9000");
        display.drawLine(0, 10, 128, 10, WHITE); 

        display.setCursor(0, 16);
        display.print("owner: ");
        display.println(ownerName);

        display.setCursor(0, 28);
        display.print("repo:  ");
        display.println(repoName);

        display.setCursor(0, 40);
        display.print("stars: ");
        display.println(stars);

        display.setCursor(0, 56);
        display.println("push btn to reroll");

        display.display();
        
        Serial.println("Loaded: " + ownerName + "/" + repoName + " (" + String(stars) + " stars)");
      } else {
        display.clearDisplay();
        display.setCursor(0, 0);
        display.println("nothing here");
        display.println("push again?");
        display.display();
      }
    } else {
       display.clearDisplay();
       display.setCursor(0, 0);
       display.println("api error (rate limit?)");
       display.display();
    }
    http.end();
  }
}