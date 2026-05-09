/**
 * @brief Einfaches Giesssystem mit kapazitivem Bodenfeuchtesensor und Relais.
 *
 * Funktion:
 * - Ist die Erde zu trocken -> Relais EIN (Pumpe/Ventil an)
 * - Ist die Erde wieder feucht genug -> Relais AUS
 *
 * Pinbelegung:
 * - Kapazitiver Feuchtesensor OUT -> A0 (SENSOR_PIN)
 * - Relais IN                  -> D8 (RELAIS_PIN)
 * - Sensor VCC / Relais VCC   -> 5V (oder 3.3V je nach Modul)
 * - Sensor GND / Relais GND   -> GND
 *
 * Hinweis:
 * Die Sensor-Rohwerte sind je nach Sensor unterschiedlich.
 * Bitte TROCKEN_WERT und FEUCHT_WERT nach einer kurzen Messung kalibrieren.
 */

const int SENSOR_PIN = A0;
const int RELAIS_PIN = 8;

// true: groesserer Messwert = trockener
// false: kleinerer Messwert = trockener
const bool HIGHER_VALUE_MEANS_DRY = true;

// Hysterese gegen Flattern:
// - bei "zu trocken" einschalten
// - erst bei "feucht genug" wieder ausschalten
const int TROCKEN_WERT = 400;
const int FEUCHT_WERT = 250;

bool relaisIstAn = false;

bool isTooDry(int sensorValue) {
  if (HIGHER_VALUE_MEANS_DRY) {
    return sensorValue >= TROCKEN_WERT;
  }
  return sensorValue <= TROCKEN_WERT;
}

bool isWetEnough(int sensorValue) {
  if (HIGHER_VALUE_MEANS_DRY) {
    return sensorValue <= FEUCHT_WERT;
  }
  return sensorValue >= FEUCHT_WERT;
}

void setup() {
  Serial.begin(115200);

  pinMode(RELAIS_PIN, OUTPUT);
  digitalWrite(RELAIS_PIN, LOW); // Relais aus

  Serial.println("Giesssystem gestartet");
}

void loop() {
  int sensorValue = analogRead(SENSOR_PIN);

  if (!relaisIstAn && isTooDry(sensorValue)) {
    relaisIstAn = true;
    digitalWrite(RELAIS_PIN, HIGH);
    Serial.println("Zu trocken -> Relais EIN");
  }

  if (relaisIstAn && isWetEnough(sensorValue)) {
    relaisIstAn = false;
    digitalWrite(RELAIS_PIN, LOW);
    Serial.println("Feucht genug -> Relais AUS");
  }

  Serial.print("Sensor: ");
  Serial.print(sensorValue);
  Serial.print(" | Relais: ");
  Serial.println(relaisIstAn ? "EIN" : "AUS");

  delay(1000);
}
