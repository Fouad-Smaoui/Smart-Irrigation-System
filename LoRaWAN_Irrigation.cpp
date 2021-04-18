#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <SoftwareSerial.h>
#include <SimpleTimer.h>


char auth[] = "LnMZlB1ZiT1kgYW1uGgc-yiPflg6BRav";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "AndroidAP7DF8"; // ZONG MBB-E8231-6E63
char pass[] = "jamshaid"; // 08659650

SimpleTimer timer;

String myString; 
String garbage;
char data; 

int firstVal; // sensor value, received from the transmitter.
int soilflag = 0; // stops the unecessary repetition of code. 

int relay = D0; // you can connect a relay over here. 
int relayf = 0; // relay flag

void setup() {
  // put your setup code here, to run once:
Serial.begin(115200); 
pinMode(relay, OUTPUT); 
digitalWrite(relay, LOW); 
Serial.print("AT\r\n");
delay(100); 


Blynk.begin(auth, ssid, pass);
timer.setInterval(1000L,sensorvalue1); 
timer.setInterval(1000L,Receivedata); 

}

void loop() {

  Blynk.run();
  timer.run(); // Initiates BlynkTimer


}


void Receivedata()
{
  // put your main code here, to run repeatedly:
if ( Serial.available() > 0 )
{
 
  garbage = Serial.readString(); // consists of the +ERR=2 ERROR. 
  
  myString = Serial.readString(); 
  //Serial.println(myString); 

  //Serial.println("Garbage:");
  //Serial.println(garbage);

String l = getValue(myString, ',', 0); // address
String m = getValue(myString, ',', 1); // data length
String n = getValue(myString, ',', 2); // data
String o = getValue(myString, ',', 3); //RSSI
String p = getValue(myString, ',', 4); //SNR
/*
Serial.println("Address:"); 
Serial.println(l); 

Serial.println("data length:"); 
Serial.println(m);

Serial.println("Data:"); 
Serial.println(n);

Serial.println("RSSI:"); 
Serial.println(o);

Serial.println("SNR:"); 
Serial.println(p);
*/
// now i am going to split the data string n which consists of the Sensors values
// in this string sensors values are separated by the % sign. 

String q = getValue(n, '%', 0); // sensor1


//Serial.println("Sensor1:"); 
//Serial.println(q); 

firstVal = q.toInt();

// the following conditions check the sensor value received from the transmitter
// and turn ON and Turn OFF the relay connected with D0 pin of the Nodemcu Module. 
// currently, i am using 20 and 30. you can change these values as per your requirement. 
// the purpose of the relayf is to stop the repition of code. 

if ( ( firstVal <= 20  ) && ( relayf == 0)  )
{

digitalWrite(relay, HIGH); 
relayf = 1; 
  
}

if ( ( firstVal >= 30  ) && ( relayf == 1)  )
{

digitalWrite(relay, LOW); 
relayf = 0; 
  
}

myString = ""; 
}
}


void sensorvalue1()
{
int sdata = firstVal;
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V2, sdata);

  if ((sdata <= 100 ) && (soilflag == 0))
  {
    Blynk.notify("Needs water"); 
    soilflag = 1; 
  }

  if ((sdata >= 900 ) && (soilflag == 1))
  {
    Blynk.notify("Soil Moisture is good"); 
    soilflag = 0; 
  }
}



String getValue(String data, char separator, int index)
{
    int found = 0;
    int strIndex[] = { 0, -1 };
    int maxIndex = data.length() - 1;

    for (int i = 0; i <= maxIndex && found <= index; i++) {
        if (data.charAt(i) == separator || i == maxIndex) {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i+1 : i;
        }
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}