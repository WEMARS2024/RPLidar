// This sketch code is based on the RPLIDAR driver library provided by RoboPeak
#include <RPLidar.h>
#include <HardwareSerial.h> //for serial connections?
#include "esp32-hal-ledc.h"



/*  Required Pin Connections  /
V5/Vmoto --> Any voltage pins
GND --> ground
RX (on lidar)--> TX (on esp) 
TX (on lidar) --> RX (on esp) 
(for serial2, RX on esp is pin 16, and TX on esp is pin 17, labelled RX2,TX2)
MOTOCTRL --> 18
(technically can be reassigned to any pin with pwm capabilties)
*/




RPLidar lidar;

//pins
///////////////////////////////////////////////////////////////////////////////////////////////////////////
#define RX = 16; //RX and TX pins for serial communication -- connect to respective pins on lidar
#define TX = 17;


#define RPLIDAR_MOTOR 18 // The PWM pin for control the speed of RPLIDAR's motor. -- connect to motocntrl pin on lidar




//used as reference point to see if sensor working as intended
int maxAngle = 0;
int minAngle = 360;
int IntAngle = 0;
int currentAngle = 0;


//constantly replace the values in this array, then send it to the pi when requested

int sendDistances[360];

////////////////////////////////////////////////////////////////////////////////////////////////////////
                        
void setup() {
  Serial.begin(115200); //to print text
  
  // bind the RPLIDAR driver to the arduino hardware serial
  lidar.begin(Serial2);

  //set pin to control lidar motor
  pinMode(RPLIDAR_MOTOR, OUTPUT);
  digitalWrite(RPLIDAR_MOTOR,HIGH);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////




void loop() {

  //add a button/switch to 'push' the information
  //for now just print the array ig
  /**
  if()//button pressed{
    for(int i=0; i<arrLength; i++){
        Serial.println("["+ i + "," + sizeOf(sendDistances) + "]");
    }
  }
  **/
  if (IS_OK(lidar.waitPoint())) {


    float distance = lidar.getCurrentPoint().distance; //distance value in mm unit

    float mDistance = distance/1000; //distance value in meters

    float angle    = lidar.getCurrentPoint().angle; //anglue value in degree
    bool  startBit = lidar.getCurrentPoint().startBit; //whether this point is belong to a new scan
    byte  quality  = lidar.getCurrentPoint().quality; //quality of the current measurement
      
    IntAngle = angle; //rounds angle to an int value

    

    //test for largest/smallest possible angles
    /*
    if(iAngle > maxAngle)
      maxAngle = iAngle;
      Serial.println("max: " + String(maxAngle));
    if(iAngle<minAngle)
      minAngle = iAngle;
      Serial.println("min: " + String(minAngle));
    */
    /** 
    Restricts output to see how frequent inconsistancies occur/ possible reasons
      if(IntAngle<15 || (IntAngle>345 && IntAngle<=360)){
        Serial.println(String(distance) + "," + IntAngle);
      }
    **/


    //fill the array of send data
    // find method to sort data into a 360 or less long list that represents the distance at each degree from the lidar

    //1. only accept measurements/angles within a certain range
          //set a check for the angle to be under 360 b4 entering to array?
    /**
    int max = 360,

    if(angle<=max){
      sendDistances[angle] = mDistance;
    }

    **/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    
    // 2. detect if angle is consistent with the ones around it? 
          //set a variable to keep track of last used angle, if next input is over 15 deg, greater, don't include it?
    /**
    int lastA;


    if(angle lastA){
      sendDistances[angle] = mDistance;
    }
    **/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // 3. take an average of the distances within a certain range
          
        //3.1 put everything into an array normally, then write code to push data from that array into a new one?
    /**      
    int roundVal = 5; //number would be changed depending on how summarized data should be
    int counter = 0;
    int sum = 0;
    int[] rounded = new int[sizeOf(sendDistances)/15];
    
    for(int i: sendDistances){
      sum+=i;
      if(counter%roundVal==0){
        rounded[counter/roundVal] = sum/roundVal;
        sum = 0;
      }
      
      counter++;
    }
    **////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
     
        //3.2 add the first x number of data points together into one value to add to an array
    
    /**
    int counter = 0;

    if(counter = 5){
      sendDistances[counter]
    }
    counter++; //might be optimal to just set to zero
    **/


    // 4. Instead of constantly spinning, spin n times every couple secconds, placing values into n different arrays, then average them?
    //until a button pressed or a counter reaches a certain value, stay still

    /**
    int[] arr1 = 

    int[] arr2 = 

    int[] arr3 = 

    // how to switch between using different arrays?? Check if last used angle was 360? if yes, 
    

    // 5. try to fix baud rates through optimization?
    **/
  } else {

   
    digitalWrite(RPLIDAR_MOTOR, LOW); //stop the rplidar motor if not detected

    

    // try to detect RPLIDAR... 
    rplidar_response_device_info_t info;
    if (IS_OK(lidar.getDeviceInfo(info, 100))) { //if detected, start scan and rplidar motor
      
      lidar.startScan(); 
       
      // start motor rotating at max allowed speed
      //ledcWrite(PWM1_Ch, PWM1_DutyCycle);
      digitalWrite(RPLIDAR_MOTOR,HIGH);

      delay(1000); 
    }  
  }
  
}
