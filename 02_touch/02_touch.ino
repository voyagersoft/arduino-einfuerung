/**
 * @brief Liest einen TTP223 Touch-Sensor ein und zeigt den Zustand auf der internen LED an.
 *
 * Der Sensorausgang (SIG/OUT) wird digital eingelesen. Bei Berührung liefert der TTP223
 * in der Standardkonfiguration typischerweise HIGH, ohne Berührung LOW.
 *
 * Verdrahtung (typisch):
 * - TTP223 VCC -> 5V (oder 3.3V je nach Board/Sensor)
 * - TTP223 GND -> GND
 * - TTP223 OUT -> D2 (TOUCH_PIN)
 *
 * @author Steffen Wagner <steffen@swagner.de>
 */

const int TOUCH_PIN = 2;

void setup() {
  pinMode(TOUCH_PIN, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  int touchState = digitalRead(TOUCH_PIN);

  if (touchState == HIGH) {
    digitalWrite(LED_BUILTIN, HIGH);
  } else {
    digitalWrite(LED_BUILTIN, LOW);
  }

  delay(10);
}
