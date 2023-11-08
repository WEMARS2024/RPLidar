#include <RPLidar.h>
#include <HardwareSerial.h>
#include "esp32-hal-ledc.h"


//Builds off the example code to run the motor on the Lidar

/*  Required Pin Connections  /
V5/Vmoto --> Any voltage pins
GND --> ground
RX (on lidar)--> TX (on esp) 
TX (on lidar) --> RX (on esp) 
(for serial2, RX on esp is pin 16, and TX on esp is pin 17, labelled RX2,TX2)
MOTOCTRL --> 18
(technically can be reassigned to any pin with pwm capabilties)
*/


// You need to create an driver instance 
RPLidar lidar;


#define RXD2 16
#define TXD2 17

#define RPLIDAR_MOTOR 18 // The PWM pin for control the speed of RPLIDAR's motor. -- connect to motocntrl pin on lidar

int co = 0; 

                        
void setup() {
  Serial.begin(115200); //set up serial for text printing text
  
  Serial.println("hi");
  
  // bind the RPLIDAR driver to the arduino hardware serial
  //lidar.begin(Serial2);
  
  /*
  lidar.isOpen();
  Serial2.begin(115200, SERIAL_8N1, 16,17);
  */
 
  
  pinMode(RPLIDAR_MOTOR,OUTPUT); //set up motor pin
  
  
  delay(2000); //esnure code below runs
  Serial.println("bye"); //check if code is being run
  digitalWrite(18,HIGH); //Start motor spinning
}

void loop() {
  
  /*
  if (IS_OK(lidar.waitPoint())) {


    float distance = lidar.getCurrentPoint().distance; //distance value in mm unit

    float mDistance = distance/1000; //distance value in meters

    float angle    = lidar.getCurrentPoint().angle; //anglue value in degree
    bool  startBit = lidar.getCurrentPoint().startBit; //whether this point is belong to a new scan
    byte  quality  = lidar.getCurrentPoint().quality; //quality of the current measurement
    
    Serial.println(distance);
   
  } else {

   
    digitalWrite(RPLIDAR_MOTOR, LOW); //stop the rplidar motor if not detected

    

    // try to detect RPLIDAR... 
    rplidar_response_device_info_t info;
    if (IS_OK(lidar.getDeviceInfo(info, 100))) { //if detected, start scan and rplidar motor
      
      lidar.startScan(); 
       
      // start motor rotating at max allowed speed
      
      digitalWrite(RPLIDAR_MOTOR,HIGH);

      delay(1000); 
    }  
  }
  */
}
