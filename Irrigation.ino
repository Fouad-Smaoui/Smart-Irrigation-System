// importer les librairies necessaires 
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Hash.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>

// Remplacer entre guillements avec les parametres du reseau 
const char* ssid = "Remplacer Avec SSID ";
const char* password = "Remplacer Avec Mot De Passe";

#define DHTPIN 5     // pin connecté avec le capteur dht 

#define DHTTYPE    DHT11     // le type du dht utilisé est le DHT 11

DHT dht(DHTPIN, DHTTYPE); // instancier un objet DHT 

// initialiser la temperature & l'humidité
float t = 0.0;
float h = 0.0;

// creation d'un serveur web sur le port 80 
AsyncWebServer server(80); // 

// on utilise " unsigned long " pour les variables qui portent le temps comme variance 
// car la valeur va etre rapidement large pour qu'un simple int sera capable de la stocker

unsigned long previousMillis = 0;    

// mettre a jour les lectures de dht chaque 10 secondes 
const long interval = 10000;
// partie CSS 
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.7.2/css/all.css" integrity="sha384-fnmOCqbTlWIlj8LyTjo7mOUStjsKC4pOpQbqyi7RrhN7udi9RwhKkMHpvLbHG9Sr" crossorigin="anonymous">
  <style>
    html {
     font-family: Arial;
     display: inline-block;
     margin: 0px auto;
     text-align: center;
    }
    h2 { font-size: 3.0rem; }
    p { font-size: 3.0rem; }
    .units { font-size: 1.2rem; }
    .dht-labels{
      font-size: 1.5rem;
      vertical-align:middle;
      padding-bottom: 15px;
    }
  </style>
</head>
<body>
  <h2>ESP8266 DHT Server</h2>
  <p>
    <i class="fas fa-thermometer-half" style="color:#059e8a;"></i> 
    <span class="dht-labels">temperature</span> 
    <span id="temperature">%temperature%</span>
    <sup class="units">&deg;C</sup>
  </p>
  <p>
    <i class="fas fa-tint" style="color:#00add6;"></i> 
    <span class="dht-labels">humidite</span>
    <span id="humidite">%humidite%</span>
    <sup class="units">%</sup>
  </p>
</body>
<script>
setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("temperature").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/temperature", true);
  xhttp.send();
}, 10000 ) ;

setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("humidite").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/humidite", true);
  xhttp.send();
}, 10000 ) ;
</script>
</html>)rawliteral";
// Remplacer "temperature" et " humidite" par les valeurs de DHT 
String processor(const String& var) {
    //affichage des valeurs de temperature et d'humidité 
    if (var == "temperature") {
        return String(t);
    }
    else if (var == "humidite") {
        return String(h);
    }
    return String();
}
void setup() {
    // port serie pour le debug
    Serial.begin(115200);
    dht.begin(); // appeler la fonction de demarrage de dht 
    // Connexion au Wi-Fi
    WiFi.begin(ssid, password); // appeler la fonction prédifinie dans la bilbiothéque ESP8266WiFi
    Serial.println("connexion au WiFi");
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println(".");
    }
    // afficher l'adresse ip locale de ESP8266 
    Serial.println(WiFi.localIP());
    // page web les routes 
    server.on("/", HTTP_GET, [](AsyncWebServerRequest* request) {
        request->send_P(200, "text/html", index_html, processor);
        });
    server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest* request) {
        request->send_P(200, "text/plain", String(t).c_str());
        });
    server.on("/humidite", HTTP_GET, [](AsyncWebServerRequest* request) {
        request->send_P(200, "text/plain", String(h).c_str());
        });

    // demarrer le serveur en appelant la fonction predéfinie dans la bilbiotheque ESPAsyncWebServer
    server.begin();
}
void loop() {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
        // sauvegarder la derniere fois qu'on a mise a jour les valeurs de dht
        previousMillis = currentMillis;
        // lecture de temperature 
        float newT = dht.readTemperature();
        // si la lecture de temperature echoue on change pas la valeur de t
        if (isnan(newT)) {
            Serial.println("La lecture de la temperature depuis le capteur dht a echouée!");
        }
        else {
            t = newT;
            Serial.println(t);
        }
        // lecture d'Humidité
        float newH = dht.readHumidity();
        // si la lecture de l'humidité echoue on change pas la valeur de h
        if (isnan(newH)) {
            Serial.println("La lecture de la temperature depuis le capteur dht a echouée!");
        }
        else {
            h = newH;
            Serial.println(h);
        }
    }
}