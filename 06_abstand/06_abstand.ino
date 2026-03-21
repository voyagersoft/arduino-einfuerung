/**
 * @brief Misst den Abstand mit einem HC-SR04 und zeigt ihn auf einem 8er WS2812-Ring/Strip.
 *
 * Verwendete Bibliothek: Adafruit NeoPixel
 * (Arduino IDE: Bibliotheksverwalter -> "Adafruit NeoPixel")
 *
 * Anschluss HC-SR04:
 * - VCC  -> 5V
 * - GND  -> GND
 * - TRIG -> D9
 * - ECHO -> D10
 *
 * Anschluss WS2812 (8 LEDs):
 * - DIN  -> D6
 * - VCC  -> 5V (bei groesseren Strips externes Netzteil nutzen)
 * - GND  -> GND (gemeinsame Masse mit Arduino)
 */

#include <Adafruit_NeoPixel.h>


//------------------------------------------------------------------------------
// Pin-Definitionen und Konstanten
//------------------------------------------------------------------------------
const int TRIG_PIN = 9;
const int ECHO_PIN = 10;
const int PIXEL_PIN = 6;
const int PIXEL_COUNT = 8;


//------------------------------------------------------------------------------
// Abstandswerte fuer die LED-Anzeige (in cm)
// Naeher als MIN_DISTANCE_CM -> alle LEDs an, weiter als MAX_DISTANCE_CM -> alle LEDs aus.
// Dazwischen wird die Anzahl der leuchtenden LEDs linear interpoliert.
// Diese Werte koennen je nach Anwendungsfall angepasst werden.
// Beispiel: MIN=3cm (sehr nah), MAX=50cm (weit genug).
//------------------------------------------------------------------------------
const float MIN_DISTANCE_CM = 3.0;
const float MAX_DISTANCE_CM = 50.0;



Adafruit_NeoPixel pixels(PIXEL_COUNT, PIXEL_PIN, NEO_GRB + NEO_KHZ800);


//------------------------------------------------------------------------------
// Misst den Abstand in cm. Gibt -1.0 zurück, wenn kein Echo empfangen wurde.
//------------------------------------------------------------------------------
float measureDistanceCm() {
    // Triggersignal fuer den Ultraschallsensor.
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);
    
    // Timeout verhindert haengende Messungen, falls kein Echo kommt.
    unsigned long durationMicros = pulseIn(ECHO_PIN, HIGH, 25000);
  if (durationMicros == 0) {
      return -1.0;
    }
    
    // Schallgeschwindigkeit: ca. 0.0343 cm/us, Hin- und Rueckweg -> /2.
    return (durationMicros * 0.0343) / 2.0;
}


//------------------------------------------------------------------------------
// Misst mehrere Abstaende und gibt den Durchschnitt zurück.
// Ungueltige Messungen (-1) werden ignoriert.
//------------------------------------------------------------------------------
float measureAverageDistanceCm(int sampleCount = 5) {
    float sum = 0.0;
    int validSamples = 0;
    
    for (int i = 0; i < sampleCount; i++) {
        float sample = measureDistanceCm();
        if (sample >= 0) {
            sum += sample;
            validSamples++;
        }
        
        // Kurze Pause zwischen den Messungen gegen Stoerechos.
        delay(20);
    }
    
    if (validSamples == 0) {
        return -1.0;
    }
    
    return sum / validSamples;
}


//------------------------------------------------------------------------------
// Setzt die Anzahl der leuchtenden LEDs entsprechend der gemessenen Distanz.
// Nahe Distanz -> mehr LEDs, grosse Distanz -> weniger LEDs.
// Farbverlauf von Gruen (weit) zu Rot (nah).
//------------------------------------------------------------------------------
void setBarByDistance(float distanceCm) {
    if (distanceCm < 0) {
        // Kein Messwert: alles aus.
        pixels.clear();
        pixels.show();
        return;
    }
    
    float clampedDistance = distanceCm;
    if (clampedDistance < MIN_DISTANCE_CM) clampedDistance = MIN_DISTANCE_CM;
    if (clampedDistance > MAX_DISTANCE_CM) clampedDistance = MAX_DISTANCE_CM;
    
    // Nahe Distanz -> viele LEDs, grosse Distanz -> wenige LEDs.
    float closeness = (MAX_DISTANCE_CM - clampedDistance) / (MAX_DISTANCE_CM - MIN_DISTANCE_CM);
    int ledsOn = (int)(closeness * PIXEL_COUNT + 0.5);
    
    if (ledsOn < 0) ledsOn = 0;
    if (ledsOn > PIXEL_COUNT) ledsOn = PIXEL_COUNT;
    
    for (int i = 0; i < PIXEL_COUNT; i++) {
        if (i < ledsOn) {
            // Farbverlauf von Gruen (weit) zu Rot (nah).
            uint8_t red = (uint8_t)(255 * closeness);
            uint8_t green = (uint8_t)(255 * (1.0 - closeness));
            pixels.setPixelColor(i, pixels.Color(red, green, 0));
        } else {
            pixels.setPixelColor(i, 0);
        }
    }
    
    pixels.show();
}


//------------------------------------------------------------------------------
// Arduino-Setup-Funktion: wird einmalig beim Start ausgefuehrt.
//------------------------------------------------------------------------------
void setup() {
    Serial.begin(115200);
    
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);
    
    pixels.begin();
    pixels.clear();
    pixels.setBrightness(40);
    pixels.show();
    
    Serial.println("Abstandsmessung mit HC-SR04 + WS2812 gestartet");
}


//------------------------------------------------------------------------------
// Arduino-Loop-Funktion: wird in Endlosschleife ausgefuehrt.
//------------------------------------------------------------------------------
void loop() {
  float distanceCm = measureAverageDistanceCm();

  Serial.print("Abstand (cm): ");
  if (distanceCm < 0) {
    Serial.println("kein Echo");
  } else {
    Serial.println(distanceCm);
  }

  setBarByDistance(distanceCm);
  delay(100);
}
