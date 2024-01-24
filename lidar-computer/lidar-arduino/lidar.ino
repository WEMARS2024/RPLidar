
#include <RPLidar.h>
#include <HardwareSerial.h>
#include "esp32-hal-ledc.h"
#define DEBUG 1

#if !DEBUG
  #include <WiFi.h>  // Include the WiFi library for ESP32
#endif

#define LEDC_CHANNEL 0
#define LEDC_FREQUENCY 1000
#define LEDC_RESOLUTION 8
int dutyCycle = 50;

RPLidar lidar;

unsigned long currentTime = millis();

#define RXD2 16
#define TXD2 17
#define RPLIDAR_MOTOR 18  // The PWM pin for controlling the speed of RPLIDAR's motor.


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
  
  // don't start motor until command
  while(Serial.available() > 0) {
    if (Serial.read() == 'o') {
      Serial2.begin(115200, SERIAL_8N1, RXD2, TXD2);
      lidar.begin(Serial2);
      pinMode(RPLIDAR_MOTOR, OUTPUT);
      delay(2000);
      Serial.println("Setup complete, entering main loop...");

      #if !DEBUG
        WiFi.mode(WIFI_STA);
        WiFi.begin(ssid, password);
      
        while (WiFi.status() != WL_CONNECTED) {
          delay(1000);
          Serial.println("Connecting to WiFi...");
          
        }
        Serial.println("Connected to WiFi");
      #endif
    }
  }
}

// Declare the scanData array globally
int scanData[360]; 
int datumRecorded = 0;

void loop() {
  if (IS_OK(lidar.waitPoint())) {
    float distance = lidar.getCurrentPoint().distance / 1000;
    int angle = int(lidar.getCurrentPoint().angle)-1 ; // Adjust angle to 0-359

    // set scanData distance to corresponding angle
    scanData[angle] = distance;
    // increment recorded by 1
    datumRecorded++;

    // if datumRecorded is 90, 180, 270, or 360, send data
    if (datumRecorded % 90 == 0) {
      #if !DEBUG
      sendDataToServer(scanData, datumRecorded);
      #endif
      // Serial.println("Data sent!");
      // Serial.println(String(datumRecorded) + "th data point: " + String(scanData[angle]));
    }

    // if datumRecorded is 360, reset counter
    if (datumRecorded >= 360) {
      datumRecorded = 0;
    } 

    // if user enters 'r', will print data
    if (Serial.read() == 'r'){
      String printout = String(angle) + "deg : " + String(distance) +"m";
      Serial.println(printout);
    }
    else if (Serial.read() == 'c') {
      Serial.println("ending...")
      lidar.end();
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

#if !DEBUG
void sendDataToServer(int data[360], int datumRecorded) {
  if (!client.connect(host, port)) {
   WiFiClient client;
    Serial.println("Connection to server failed");
    return;
  }

  // declare string variable for data sent
  String sData = "";

  // parse data into string
  for (int i = datumRecorded; i < datumRecorded+90; i++) {
    sData = sData + data[i];
  }

  // send to front end
  client.print(data);
  client.stop();
 }
#endif