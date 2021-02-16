/***************
 SMAOUI Fouad
 ------------
 Système d'irrigation intelligent
 --------------------------------------------------------------------
 ***************/

 // Capteur de Temperature et d'humidité atmosphérique
#include <dht11.h> // libraire du capteur de temperature et d'humidité dht11
dht11 DHT11;       // type du capteur DHT choisi : dans notre projet :  DHT 11
#define DHT A0     // pin correspondante au capteur de temperature et d'humidité

// capteur d'humidité de sol
#define Soil_moisture A1 // pin correspondante au capteur d'humidité de sol
float Sec = 200;         //valeur de seuil de l'humidité de sol

//capteur de lumiere photoresistance
#define Photo_resistance 8 // pin correspondante au photoresistance

//capteur du niveau de l'eau

#define Water A2       // pin correspondante au capteur de niveau de l'eau
float Reservoir = 100; // valeur de seuil pour le niveau de l'eau dans le reservoir

// capteur de pluie
#define Rain A3          // pin correspondante au capteur de pluie
float Pluie_seuil = 500; //valeur de seuil pour la pluie

#define Buzzer 5 // pin correspondante au buzzer
#define Relay 6  // pin correspondante au relay

void setup()
{
    Serial.begin(9600);
    // les entrées
    pinMode(DHT, INPUT);
    pinMode(Soil_moisture, INPUT);
    pinMode(Photo_resistance, INPUT);
    pinMode(Water, INPUT);
    pinMode(Rain, INPUT);

    // les sorties
    pinMode(Buzzer, OUTPUT);
    pinMode(Relay, OUTPUT);
}

void loop()
{                                                   // lecture depuis les capteurs
    float Temperature = DHT11.read(DHT);            // lecture des valeurs du capteur de temperature et d'humidité
    float Humidite_sol = AnalogRead(Soil_moisture); // lecture des valeurs du capteur d'humidité de sol
    int Lumiere = DigitalRead(Photo_resistance);    // lecture des valeurs du capteur de lumiére
    float Niveau_eau = AnalogRead(Water);           // lecture des valeurs du capteur de niveau de l'eau
    float Pluie = AnalogRead(Rain)                  // lecture des valeurs du capteur de pluie

    // Irrigation
    int Lumiere = 0;                                // initialisation de l'etat de la photoresistance
    while (Temperature < 40 && Lumiere == 0) // meilleur temps d'irrigation
    {
        if (Humidite_sol < Sec)
        {                            // condition d'irrigation
            DigitalWrite(Relay, HIGH) // actionner le relay
        }
        // Arrêt d'irrigation
        if ((Niveau_eau <= 2 / 3 Reservoir) || (Pluie < Pluie_seuil))
        {                            // arreter l'irrigation le cas ou le niveau de l'eau est bas ou présence de pluie
            DigitalWrite(Relay, LOW) // desactiver le relay
                                     // Alarme
            tone(Buzzer, 1000);  // envoie 1KHz signal son
            delay(60000);            //pendant une minute
            noTone(Buzzer);          // arrêter l'alarme
        }
    }
}