//small program to test that the arduino ide and esp32 are interfaced properly

void setup() {
  //when running code, open the serial monitor, and set the baud rate to match the begin function (115200)
  Serial.begin(115200);
}

void loop() {
  Serial.println("hello world");
}
