const int sensor_pin = A0; // Soil moisture analog sensor pin

void setup() {
  Serial.begin(9600); // for serial communication 
}

void loop() {
  float moisture_percentage; // variable for moisture percentage 
  int sensor_analog; // variable for moisture sensor values  
  sensor_analog = analogRead(sensor_pin);  // reading analog values from moisture sensor 
  moisture_percentage = ( 100 - ( (sensor_analog/1023.00) * 100 ) ); // from a value to a percentage 
  // printing on the screen 
  Serial.print("Moisture Percentage = "); 
  Serial.print(moisture_percentage);
  Serial.print("%\n\n"); 
  delay(1000); // choosing a delay 
}
