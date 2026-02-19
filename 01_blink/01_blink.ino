/**
 * @brief Initialisiert die integrierte LED des Arduino-Boards
 * 
 * Diese Funktion wird einmalig beim Start des Arduino ausgeführt und konfiguriert
 * den digitalen Pin LED_BUILTIN als Ausgang (OUTPUT). Dies ist notwendig, um die
 * integrierte LED des Boards steuern zu können.
 * 
 * @note LED_BUILTIN ist eine vordefinierte Konstante, die den Pin der integrierten
 *       LED des jeweiligen Arduino-Boards referenziert (z.B. Pin 13 beim Arduino UNO)
 * 
 * @see setup() - Funktion zur Initialisierung von Einstellungen und Konfigurationen
 * @see pinMode() - Funktion zur Konfiguration von digitalen Pins
 * @see LED_BUILTIN - Vordefinierte Konstante für die integrierte LED des Arduino-Boards
 * @see OUTPUT - Vordefinierte Konstante für den Zustand eines digitalen Pins als Ausgang
 * 
 * @see loop() - Hauptschleife, die nach setup() wiederholt ausgeführt wird
 * @see digitalWrite() - Funktion zum Setzen des Spannungsniveaus eines digitalen Pins
 * @see delay() - Funktion zum Pausieren des Programms für eine bestimmte Zeit
 * @see HIGH - Vordefinierte Konstante für den HIGH-Zustand eines digitalen Pins
 * @see LOW - Vordefinierte Konstante für den LOW-Zustand eines digitalen Pins
 * 
 * @author Steffen Wagner <steffen@swagner.de>
 */


void setup() {
  // LED_BUILTIN als Output definieren
  pinMode(LED_BUILTIN, OUTPUT);
}

// die loop-Funktion wird immer wieder ausgeführt
void loop() {
  digitalWrite(LED_BUILTIN, HIGH);  // LED anschalten (HIGH ist das Spannungsniveau)
  delay(1000);                      // eine Sekunde warten
  digitalWrite(LED_BUILTIN, LOW);   // LED ausschalten (LOW ist das Spannungsniveau)
  delay(1000);                      // eine Sekunde warten
}
