
// #include <RPLidar.h>

// #include <HardwareSerial.h>
// #include "esp32-hal-ledc.h"


// // You need to create an driver instance
// RPLidar lidar;



// #define RXD2 16
// #define TXD2 17

// #define RPLIDAR_MOTOR 18 // The PWM pin for control the speed of RPLIDAR's motor. -- connect to motocntrl pin on lidar


// void setup() {
//   Serial.begin(115200); //set up serial for text printing text



//   // bind the RPLIDAR driver to the arduino hardware serial
//   Serial2.begin(115200, SERIAL_8N1, RXD2, TXD2);
//   lidar.begin(Serial2);



//   pinMode(RPLIDAR_MOTOR, OUTPUT); //set up motor pin


//   delay(2000); //esnure code below runs
//   Serial.println("bye"); //check if code is being run


// }

// void loop() {

//   //digitalWrite(18,HIGH); //Start motor spinning

//   //Serial.println("hi");

//   if (IS_OK(lidar.waitPoint())) {

//    // Serial.println("printing");
//     float distance = lidar.getCurrentPoint().distance; //distance value in mm unit

//     float mDistance = distance/1000; //distance value in meters

//     float angle    = lidar.getCurrentPoint().angle; //anglue value in degree
//     bool  startBit = lidar.getCurrentPoint().startBit; //whether this point is belong to a new scan
//     byte  quality  = lidar.getCurrentPoint().quality; //quality of the current measurement


//     Serial.println(mDistance);

//  } else {


//     digitalWrite(RPLIDAR_MOTOR, LOW); //stop the rplidar motor if not detected



//     // try to detect RPLIDAR...
//     rplidar_response_device_info_t info;
//     if (IS_OK(lidar.getDeviceInfo(info, 100))) { //if detected, start scan and rplidar motor
//       Serial.println("scanning");
//       lidar.startScan();

//       // start motor rotating at max allowed speed

//       digitalWrite(RPLIDAR_MOTOR,HIGH);

//       delay(1000);
//     }
//  }
// }

// #include <RPLidar.h>
// #include <HardwareSerial.h>
// #include "esp32-hal-ledc.h"
// #include <WiFi.h>  // Include the WiFi library for ESP32

// #define LEDC_CHANNEL 0
// #define LEDC_FREQUENCY 1000
// #define LEDC_RESOLUTION 8
// int dutyCycle = 100;

// RPLidar lidar;

// #define RXD2 16
// #define TXD2 17
// #define RPLIDAR_MOTOR 18  // The PWM pin for controlling the speed of RPLIDAR's motor.

// // Define a struct to hold the distance and angle data
// struct LidarData {
//   float distance;
//   float angle;
// };

// // Declare the scanData array globally
// LidarData scanData[360]; 

// // Network credentials and server details
// const char* ssid = "shawnthree";
// const char* password = "testpasstwo";
// const char* host = "172.30.122.204";
// const uint16_t port = 8080;

// void setup() {
//   ledcSetup(LEDC_CHANNEL, LEDC_FREQUENCY, LEDC_RESOLUTION);
//   ledcAttachPin(RPLIDAR_MOTOR, LEDC_CHANNEL);
//   ledcWrite(LEDC_CHANNEL, dutyCycle);

//   Serial.begin(115200);
//   Serial2.begin(115200, SERIAL_8N1, RXD2, TXD2);
//   lidar.begin(Serial2);
//   pinMode(RPLIDAR_MOTOR, OUTPUT);
//   delay(2000);
//   Serial.println("Setup complete, entering main loop...");

//   WiFi.begin(ssid, password);
//   while (WiFi.status() != WL_CONNECTED) {
//     delay(1000);
//     Serial.println("Connecting to WiFi...");
//   }
//   Serial.println("Connected to WiFi");
//  }

// void loop() {
//  static bool recorded[360] = {false};
//   static String aggregatedData = "";

//   if (IS_OK(lidar.waitPoint())) {
//     float distance = lidar.getCurrentPoint().distance / 1000;
//     int angle = int(lidar.getCurrentPoint().angle);

//     if (!recorded[angle]) {
//       recorded[angle] = true;
//       scanData[angle].distance = distance;
//       scanData[angle].angle = angle;

//       // Append the current angle-distance pair to the aggregated data
//       aggregatedData += String(angle) + ":" + String(distance) + ";";
//     }

//     if (angle == 359) {
//     // Send all data to server after full rotation
//     sendDataToServer(aggregatedData);

//     // Debug message to confirm data transmission
//     Serial.println("data sent"); 

//     // Clear the recorded data for the next scan
//     memset(recorded, false, sizeof(recorded));
//     aggregatedData = ""; // Reset the aggregated data
//   }

//   } else {
//     digitalWrite(RPLIDAR_MOTOR, LOW);
//     rplidar_response_device_info_t info;
//     if (IS_OK(lidar.getDeviceInfo(info, 100))) {
//       lidar.startScan();
//       digitalWrite(RPLIDAR_MOTOR, HIGH);
//       delay(1000);
//     } else {
//       Serial.println("Failed to get device info");
//     }
//   }
// }

// void sendDataToServer(const String& data) {
//   WiFiClient client;
//   if (!client.connect(host, port)) {
//     Serial.println("Connection to server failed");
//     return;
//   }
//   client.print(data);
//   client.stop();
// }



#include <RPLidar.h>
#include <HardwareSerial.h>
#include "esp32-hal-ledc.h"
#include <WiFi.h>  // Include the WiFi library for ESP32

#define LEDC_CHANNEL 0
#define LEDC_FREQUENCY 1000
#define LEDC_RESOLUTION 8
int dutyCycle = 50;

RPLidar lidar;

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

  // int n = WiFi.scanNetworks();
  // Serial.printf("%d\n",n);
  // for (int i=0; i<n; i++) {
  //   Serial.printf("%d. %s\n",i+1,WiFi.SSID(i));
  // }

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println(WiFi.status());
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
 }

 void loop() {
  static bool recorded[360] = {false};
  static String aggregatedData = "";

  if (IS_OK(lidar.waitPoint())) {
    float distance = lidar.getCurrentPoint().distance / 1000;
    int angle = int(lidar.getCurrentPoint().angle);

    if (!recorded[angle]) {
      recorded[angle] = true;
      scanData[angle].distance = distance;
      scanData[angle].angle = angle;

      // Append the current angle-distance pair to the aggregated data
      aggregatedData += String(angle) + ":" + String(distance) + ";";
    }


    if (currentTime - lastSendTime >= sendInterval) {
      
      // Send all data to server after full rotation
      Serial.println(aggregatedData);
      sendDataToServer(aggregatedData);

     // Debug message to confirm data transmission
      Serial.println("Data sent"); 

      // Clear the recorded data for the next scan
      memset(recorded, false, sizeof(recorded));
      aggregatedData = ""; // Reset the aggregated data
      lastSendTime = currentTime;
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

void sendDataToServer(const String& data) {
  WiFiClient client;
  if (!client.connect(host, port)) {
    Serial.println("Connection to server failed");
    return;
  }
  client.print(data);
  client.stop();
 }
