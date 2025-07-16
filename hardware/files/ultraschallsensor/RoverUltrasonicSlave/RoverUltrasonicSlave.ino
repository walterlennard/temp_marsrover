#include <Wire.h>
#include <HCSR04.h>

const int SLAVE_ADDRESS = 8; // Eine eindeutige I2C-Adresse für den Arduino
const byte triggerPin = 4;
const byte echoCount = 4;
const byte* echoPins = new byte[echoCount] { 3,5,6,9 };

void requestEvent();

void setup() {
  Wire.begin(SLAVE_ADDRESS);
  Wire.onRequest(requestEvent); // Event-Handler für Anfragen vom Master
  HCSR04.begin(triggerPin, echoPins, echoCount); // Sensor Init
  Serial.begin(9600);
  Serial.println("Arduino I2C Slave gestartet.");
  
  pinMode(LED_BUILTIN, OUTPUT); // status led
}

void loop() {

}

// Wird aufgerufen, wenn der Master Daten vom Slave anfordert
void requestEvent() {
  digitalWrite(LED_BUILTIN, HIGH);
  Serial.println("Anfrage vom Master erhalten.");

  // auslesen der Entfehrnungen
  double* distances = HCSR04.measureDistanceMm();
  String dataString = "";

  // String building
  for (int i = 0; i < echoCount; i++) {
    if (i > 0) dataString += ",";
    dataString += String(distances[i]);
  }
  
  Wire.write(dataString.c_str()); // send data to Rover

  Serial.print("Gesendet: ");
  Serial.println(dataString);
  
  digitalWrite(LED_BUILTIN, LOW);
}
