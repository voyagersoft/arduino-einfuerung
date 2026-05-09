/**
 * @brief RCWL-0516 Bewegungsmelder steuert ein Relais mit Nachlaufzeit.
 *        Ein WS2812-Strip mit 8 LEDs zeigt den 8s-Countdown an.
 *
 * Funktion:
 * - Wird eine Bewegung erkannt, schaltet das Relais ein und alle 8 LEDs leuchten.
 * - Jede weitere Bewegung innerhalb der Nachlaufzeit startet den Timer neu (alle 8 LEDs an).
 * - Pro verstrichener Sekunde schaltet eine LED aus (von LED 8 -> 1).
 * - Nach 8 Sekunden ohne neue Bewegung schaltet das Relais und alle LEDs aus.
 *
 * Verdrahtung (Beispiel):
 * - RCWL-0516 OUT -> D2
 * - RCWL-0516 VIN -> 5V
 * - RCWL-0516 GND -> GND
 * - Relais  IN    -> D8
 * - Relais  VCC/GND entsprechend Modul und Board
 * - WS2812  DIN   -> D6
 * - WS2812  5V    -> 5V
 * - WS2812  GND   -> GND
 *
 * Benoetigte Bibliothek: FastLED (Bibliotheksmanager: "FastLED")
 *
 * @author Steffen Wagner <steffen@swagner.de>
 */
// FastLED@3.10.3
#include <FastLED.h>

const int MOTION_PIN = 2;
const int RELAY_PIN  = 5;

// WS2812 Strip
#define LED_PIN        6
#define NUM_LEDS       8
CRGB leds[NUM_LEDS];
const CRGB COLOR_ON  = CRGB::Orange;   // Farbe waehrend Countdown
const CRGB COLOR_OFF = CRGB::Black;

// Viele Relaismodule sind HIGH-aktiv. Bei LOW-aktivem Modul auf LOW ändern.
const int RELAY_ON_STATE = HIGH;
const int RELAY_OFF_STATE = (RELAY_ON_STATE == HIGH) ? LOW : HIGH;

const unsigned long RELAY_HOLD_MS = 8000;

bool relayIsOn = false;
unsigned long lastMotionAt = 0;
bool wasMotionDetected = false;


//------------------------------------------------------------------------------
// Alle LEDs auf COLOR_OFF setzen
//------------------------------------------------------------------------------
void clearStrip() {
    fill_solid(leds, NUM_LEDS, COLOR_OFF);
    FastLED.show();
}


//------------------------------------------------------------------------------
// Zeige remaining Sekunden: LEDs 0..(sek-1) leuchten, Rest aus
//------------------------------------------------------------------------------
void showCountdown(int remainingSec) {
    for (int i = 0; i < NUM_LEDS; i++) {
        leds[i] = (i < remainingSec) ? COLOR_ON : COLOR_OFF;
    }
    FastLED.show();
}


//------------------------------------------------------------------------------
// Arduino-Setup-Funktion: wird einmalig beim Start ausgefuehrt.
//------------------------------------------------------------------------------
void setup() {
  Serial.begin(115200);

  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(80);
  clearStrip();

  pinMode(MOTION_PIN, INPUT);
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, RELAY_OFF_STATE);

  Serial.println("RCWL-0516 Bewegungsmelder gestartet");
  Serial.println("Warte auf Bewegung...");
}


//------------------------------------------------------------------------------
// Arduino-Loop-Funktion: wird in Endlosschleife ausgefuehrt.
//------------------------------------------------------------------------------
// Logik:
// - Bei Bewegung: Relais einschalten und Timer zurücksetzen
// - Alle 20ms prüfen, ob 8s ohne Bewegung vergangen sind -> Relais ausschalten
// - Jede Bewegung innerhalb der 8s startet den Timer neu
// - Statusänderungen werden auf der seriellen Konsole ausgegeben
// - Alle 20ms prüfen, ob 8s ohne Bewegung vergangen sind -> Relais ausschalten
//------------------------------------------------------------------------------
void loop() {
  const unsigned long now = millis();
  const bool motionDetected = (digitalRead(MOTION_PIN) == HIGH);

  if (motionDetected && !wasMotionDetected) {
    lastMotionAt = now;

    if (!relayIsOn) {
      relayIsOn = true;
      digitalWrite(RELAY_PIN, RELAY_ON_STATE);
      Serial.println("Bewegung erkannt -> Relais EIN (8s Nachlauf)");
    } else {
      Serial.println("Weitere Bewegung erkannt -> Nachlauf neu gestartet (8s)");
    }
    showCountdown(NUM_LEDS); // alle 8 LEDs an
  }

  // Countdown-Anzeige aktualisieren
  if (relayIsOn) {
    unsigned long elapsed = now - lastMotionAt;
    int remaining = (int)(((long)RELAY_HOLD_MS - (long)elapsed + 999) / 1000);
    if (remaining < 0) remaining = 0;
    showCountdown(remaining);
  }

  if (relayIsOn && (now - lastMotionAt >= RELAY_HOLD_MS)) {
    relayIsOn = false;
    digitalWrite(RELAY_PIN, RELAY_OFF_STATE);
    clearStrip();
    Serial.println("8s ohne neue Bewegung -> Relais AUS");
  }

  wasMotionDetected = motionDetected;

  // 20ms warten
  delay(20);
}
