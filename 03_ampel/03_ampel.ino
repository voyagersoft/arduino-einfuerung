/**
 * @brief Ampelsteuerung mit TTP223/TPP223 Touch-Sensor.
 *
 * Bei jeder Beruehrung des Sensors (steigende Flanke) wechselt die Ampel in den
 * naechsten Zustand:
 *
 *   Rot -> Rot+Gelb -> Gruen -> Gelb -> Rot ...
 *
 * Die Steuerung ist damit zustandsbasiert:
 * - Kein automatischer Zeitablauf
 * - Jeder Touch-Druck schaltet genau einen Schritt weiter
 *
 * Pinbelegung Ampel:
 * - Rot   -> D13
 * - Gelb  -> D12
 * - Gruen -> D11
 *
 * Pinbelegung Touch-Sensor:
 * - OUT -> D2
 * - VCC -> 5V (oder 3.3V je nach Board/Sensor)
 * - GND -> GND
 * 
 * @author Steffen Wagner <steffen@swagner.de>
 */

const int TOUCH_PIN = 2;

const int RED_PIN = 13;
const int YELLOW_PIN = 12;
const int GREEN_PIN = 11;

// Letzter gemerkter Sensorzustand, um eine steigende Flanke zu erkennen.
int lastTouchState = LOW;

// Ampelzustand:
// 0 = Rot, 1 = Rot+Gelb, 2 = Gruen, 3 = Gelb
int trafficState = 0;

//------------------------------------------------------------------------------
// Schaltet die drei Ampel-LEDs entsprechend der uebergebenen Zustandswerte.
//------------------------------------------------------------------------------
void setLights(bool redOn, bool yellowOn, bool greenOn) {
    digitalWrite(RED_PIN, redOn ? HIGH : LOW);
    digitalWrite(YELLOW_PIN, yellowOn ? HIGH : LOW);
    digitalWrite(GREEN_PIN, greenOn ? HIGH : LOW);
}

//------------------------------------------------------------------------------
// Wendet den aktuellen numerischen Ampelzustand (trafficState) auf die
// Ausgaenge an.
//------------------------------------------------------------------------------
void applyTrafficState() {
    if (trafficState == 0) {
        setLights(true, false, false);   // Rot
    } else if (trafficState == 1) {
        setLights(true, true, false);    // Rot + Gelb
    } else if (trafficState == 2) {
        setLights(false, false, true);   // Gruen
    } else {
        setLights(false, true, false);   // Gelb
    }
}

//------------------------------------------------------------------------------
// Arduino-Setup-Funktion: wird einmalig beim Start ausgefuehrt.
//------------------------------------------------------------------------------
void setup() {
    // Sensor als digitalen Eingang konfigurieren.
    pinMode(TOUCH_PIN, INPUT);
    
    // LED-Pins der Ampel als Ausgaenge konfigurieren.
    pinMode(RED_PIN, OUTPUT);
    pinMode(YELLOW_PIN, OUTPUT);
    pinMode(GREEN_PIN, OUTPUT);
    
    // Startzustand anzeigen (trafficState = 0 -> Rot).
    applyTrafficState();
}

//------------------------------------------------------------------------------
// Arduino-Loop-Funktion: wird in einer Endlosschleife ausgefuehrt.
//------------------------------------------------------------------------------
void loop() {
  // Aktuellen Sensorzustand lesen.
  int touchState = digitalRead(TOUCH_PIN);

  // Steigende Flanke: nur beim Uebergang LOW -> HIGH weiterschalten.
  if (touchState == HIGH && lastTouchState == LOW) {
    // Naechster Zustand (0..3), danach wieder von vorne.
    trafficState = (trafficState + 1) % 4;
    applyTrafficState();
  }

  // Aktuellen Zustand fuer den naechsten Schleifendurchlauf merken.
  lastTouchState = touchState;

  // Kleine Pause zur Entlastung der CPU und zur Beruhigung des Signals.
  delay(10);
}
