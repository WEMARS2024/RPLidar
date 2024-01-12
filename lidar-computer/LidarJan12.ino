
#include <RPLidar.h>
#include <HardwareSerial.h>
#include "esp32-hal-ledc.h"
#include <WiFi.h>  // Include the WiFi library for ESP32

#define LEDC_CHANNEL 0
#define LEDC_FREQUENCY 1000
#define LEDC_RESOLUTION 8
int dutyCycle = 50;

RPLidar lidar;

bool recorded[360] = {false};

unsigned long currentTime = millis();

#define RXD2 16
#define TXD2 17
#define RPLIDAR_MOTOR 18  // The PWM pin for controlling the speed of RPLIDAR's motor.

// Define a struct to hold the distance and angle data
struct LidarData {
  float distance;
  float angle;
};

// Declare the scanData array globally
LidarData scanData[360]; 

// Network credentials and server details
const char* ssid = "Howard"; // Replace with your WiFi network's SSID
const char* password = "howardiscool"; // Replace with your WiFi network's password
const char* host = "172.20.10.2"; // Replace with your server's IP address
const uint16_t port = 8080; // The port on which your server is listening

void setup() {
  ledcSetup(LEDC_CHANNEL, LEDC_FREQUENCY, LEDC_RESOLUTION);
  ledcAttachPin(RPLIDAR_MOTOR, LEDC_CHANNEL);
  ledcWrite(LEDC_CHANNEL, dutyCycle);

  Serial.begin(115200);
  Serial2.begin(115200, SERIAL_8N1, RXD2, TXD2);
  lidar.begin(Serial2);
  pinMode(RPLIDAR_MOTOR, OUTPUT);
  delay(2000);
  Serial.println("Setup complete, entering main loop...");

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
 
   while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
     
  }
  Serial.println("Connected to WiFi");
 }

 void loop() {
    static String aggregatedData = "";

//new test
    bool allAnglesRecorded = true;

  if (IS_OK(lidar.waitPoint())) {
    float distance = lidar.getCurrentPoint().distance / 1000;
    int angle = int(lidar.getCurrentPoint().angle)-1 ; // Adjust angle to 0-359

    if (angle >= 0 && angle < 360 && !recorded[angle]) {
      recorded[angle] = true;
      scanData[angle].distance = distance;
      scanData[angle].angle = angle;
      aggregatedData += String(angle + 1) + ":" + String(distance) + ";"; // Store angles as 1-360
    }

    for (int i = 0; i < 360; i++) {
      if (!recorded[i]) {
        allAnglesRecorded = false;
        break;
      }
    }

    if (allAnglesRecorded) {
      Serial.println(aggregatedData);
      sendDataToServer(aggregatedData);

      memset(recorded, false, sizeof(recorded));
      aggregatedData = "";
    }

  


  } else {
    digitalWrite(RPLIDAR_MOTOR, LOW);
    rplidar_response_device_info_t info;
    if (IS_OK(lidar.getDeviceInfo(info, 100))) {
      lidar.startScan();
      digitalWrite(RPLIDAR_MOTOR, HIGH);
      delay(1000);
    } else {
      Serial.println("Failed to get device info");
    }
  }
}

void sendDataToServer(const String & data) {

  if (!client.connect(host, port)) {
   WiFiClient client;
    Serial.println("Connection to server failed");
    return;
  }
  client.print(data);
  client.stop();
 }


