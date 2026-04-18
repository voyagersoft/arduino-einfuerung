# Arduino Einfuehrung

Diese Sammlung enthaelt mehrere Arduino-Beispiele fuer den Einstieg in Sensorik,
Aktoren und einfache Automatisierung.

## Voraussetzungen

- Arduino IDE (oder Arduino CLI)
- Passendes Board ausgewaehlt (z. B. Arduino Uno/Nano)
- Bei Bedarf zusaetzliche Bibliotheken aus dem Bibliotheksverwalter:
	- Adafruit PN532 (fuer RFID-Beispiele)
  - FastLED (fuer WS2812-LEDs)

## Projektuebersicht

- 01_blink
  - Einfaches Blink-Beispiel mit der internen LED.

- 02_touch
  - Liest einen TTP223-Touchsensor und schaltet die interne LED.

- 03_ampel
  - Touch-gesteuerte Ampel mit vier Zustaenden: Rot -> Rot+Gelb -> Gruen -> Gelb.

- 04_rfid
  - RFID/NFC-Leser mit PN532 (I2C), UID-Ausgabe ueber Serial und Ampel-/Relais-Logik.

- 04_rfid_scan
  - RFID/NFC-Scan mit PN532 inklusive I2C-Scan zur Diagnose.

- 05_giessanlage 
  - Einfache automatische Giesssteuerung mit Bodenfeuchtesensor und Relais

- 06_abstand
  - Abstandsmessung mit HC-SR04 und Visualisierung auf 8 WS2812-LEDs.

- 07_bewegungsmelder
  - RCWL-0516 Bewegungsmelder mit Relais und 8x WS2812-Countdown.
  - Bei Bewegung: Relais EIN und 8 LEDs an.
  - Ohne weitere Bewegung: pro Sekunde geht 1 LED aus.
  - Nach 8 Sekunden ohne neue Bewegung: Relais AUS und LEDs AUS.
  - Serielle Statusmeldungen ueber USB/UART.

- 08_minecraftlamp/  
	- [3D-Dateien fuer Gehaeuse/Modell (STEP/3MF)](https://makerworld.com/de/models/1929022-minecraft-ore-lamp-wled-esp32-d1-mini-usb-c)
	- kein Arduino-Sketch enthalten, [WLED Code](https://install.wled.me/)
	- Basisplatte optimiert (für 8x8 WS2812)

- 09_led_matrix/  
	- [3D-Datei fuer LED-Matrix-Uhr (3MF)](https://makerworld.com/de/models/898272-led-matrix-clock)
	- kein Arduino-Sketch enthalten, [WLED Code](https://install.wled.me/)

- 10_ArduTester
  [10_ArduTester auf GitHub](https://github.com/andres-garcia-alves/electronics-ardutester?tab=readme-ov-file)


