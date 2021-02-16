int LDRSensorPin = D3 ; // selecting a digital input pin for LDR Sensor 

int LDRSensorValue = 0; // variable to store LDR sensor's value 
void setup() {
Serial.begin(9600); //setting serial port for communication
}
void loop() {
LDRSensorValue = digitalRead(LDRSensorPin); // reading LDR sensor's values
Serial.println(LDRSensorValue); //prints LDR sensor's values on the screen

delay(100); // choosing a delay 

}
