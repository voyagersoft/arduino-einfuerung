/**
 * @brief Liest RFID/NFC-Karten mit einem PN532 und gibt die UID auf der seriellen Schnittstelle aus.
 *
 * Verwendete Bibliothek: Adafruit PN532
 * (Arduino IDE: Bibliotheksverwalter -> "Adafruit PN532")
 *
 * Anschluss (I2C-Modus):
 * - PN532 SDA  -> SDA
 * - PN532 SCL  -> SCL
 * - PN532 IRQ  -> D2   (optionale Steuerleitung)
 * - PN532 RSTO -> D3   (optionale Reset-Leitung)
 * - PN532 VCC  -> 5V (oder 3.3V je nach Modul)
 * - PN532 GND  -> GND
 *
 * Board-Hinweise fuer SDA/SCL:
 * - Arduino UNO/Nano: SDA=A4, SCL=A5
 * - Arduino Mega: SDA=20, SCL=21
 * - ESP32 (typisch): SDA=21, SCL=22
 *
 * Hinweis: Die eigentliche Datenkommunikation erfolgt ueber SDA/SCL (I2C).
 */

#include <Wire.h>
#include <Adafruit_PN532.h>

const int PN532_IRQ = 2;
const int PN532_RESET = 3;

const int RED_PIN = 13;
const int YELLOW_PIN = 12;
const int GREEN_PIN = 11;

Adafruit_PN532 nfc(PN532_IRQ, PN532_RESET);

//------------------------------------------------------------------------------
// Konvertiert eine UID (Byte-Array) in einen unsigned long Wert.
//------------------------------------------------------------------------------
unsigned long uidToUlong(const uint8_t* uid, uint8_t uidLength) {
  unsigned long value = 0;
  uint8_t bytesToUse = uidLength;

  // unsigned long ist auf Arduino 32-bit, daher max. 4 Bytes verwenden.
  if (bytesToUse > 4) {
    bytesToUse = 4;
  }

  for (uint8_t i = 0; i < bytesToUse; i++) {
    value = (value << 8) | uid[i];
  }

  return value;
}

//------------------------------------------------------------------------------
// Schaltet die drei Ampel-LEDs entsprechend der uebergebenen Zustandswerte.
//------------------------------------------------------------------------------
void setLights(bool redOn, bool yellowOn, bool greenOn) {
  digitalWrite(RED_PIN, redOn ? HIGH : LOW);
  digitalWrite(YELLOW_PIN, yellowOn ? HIGH : LOW);
  digitalWrite(GREEN_PIN, greenOn ? HIGH : LOW);
}

//------------------------------------------------------------------------------
// Prueft, ob ein Geraet auf dem I2C-Bus an der angegebenen Adresse antwortet.
//------------------------------------------------------------------------------
bool isDeviceOnI2C(uint8_t address) {
  Wire.beginTransmission(address);
  return (Wire.endTransmission() == 0);
}

//------------------------------------------------------------------------------
// Führt einen I2C-Scan durch und gibt gefundene Geräte auf der seriellen
// Schnittstelle aus.
//------------------------------------------------------------------------------
void printI2CScan() {
  Serial.println("I2C-Scan gestartet...");

  bool foundAny = false;
  for (uint8_t address = 1; address < 127; address++) {
    Wire.beginTransmission(address);
    uint8_t error = Wire.endTransmission();

    if (error == 0) {
      foundAny = true;
      Serial.print("Gefundenes I2C-Geraet bei 0x");
      if (address < 16) {
        Serial.print("0");
      }
      Serial.println(address, HEX);
    }
  }

  if (!foundAny) {
    Serial.println("Keine I2C-Geraete gefunden.");
  }
}

//------------------------------------------------------------------------------
// Arduino-Setup-Funktion: wird einmalig beim Start ausgefuehrt.
//------------------------------------------------------------------------------
void setup() {
  // Serielle Schnittstelle für Debug-Ausgaben starten
  Serial.begin(115200);
  while (!Serial) {
    delay(10);
  }

  // LED-Pins der Ampel als Ausgaenge konfigurieren.
  pinMode(RED_PIN, OUTPUT);
  pinMode(YELLOW_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  setLights(false, true, false); // Gelb als Startzustand

  // I2C-Bus starten (SDA/SCL) und scannen
  Wire.begin();
  Serial.println("PN532 RFID Reader startet...");
  printI2CScan();

  if (!isDeviceOnI2C(0x24)) {
    Serial.println("PN532 nicht auf I2C-Adresse 0x24 gefunden.");
  }

  nfc.begin();

  uint32_t versionData = nfc.getFirmwareVersion();
  if (!versionData) {
    Serial.println("Kein PN532 gefunden. Bitte Verkabelung pruefen.");
    while (true) {
      delay(1000);
    }
  }

  nfc.SAMConfig();
  Serial.println("PN532 bereit. Karte an den Reader halten.");
}

//------------------------------------------------------------------------------
// Arduino-Hauptschleife: wird nach setup() immer wieder ausgefuehrt.
//------------------------------------------------------------------------------
void loop() {
  uint8_t uid[7];
  uint8_t uidLength = 0;

  bool success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength, 100);

  if (success) {
    unsigned long uidAsUlong = uidToUlong(uid, uidLength);
    Serial.print("UID: ");
    Serial.println(uidAsUlong);
    if (uidAsUlong == 2254495488) {
      setLights(false, false, true); // Grün einschalten
    } else {
      setLights(true, false, false); // Rot einschalten
    }
  } else {
    // keine Karte gefunden
    setLights(false, true, false); // Gelb einschalten
  }

  Serial.println();

  delay(500);
}
