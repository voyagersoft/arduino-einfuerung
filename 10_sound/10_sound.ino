/**
 * @brief Spielt den Star Wars Imperial March auf einem passiven Piezo-Lautsprecher.
 *
 * Verdrahtung (Beispiel):
 * - Piezo Signal -> D8
 * - Piezo GND    -> GND
 *
 * Hinweis:
 * - Das Beispiel nutzt die eingebaute Arduino-Funktion tone().
 * - Ein passiver Piezo eignet sich besser als ein aktiver Summer.
 *
 * @author Steffen Wagner <steffen@swagner.de>
 */

const int PIEZO_PIN = 8;
const float TUNING_FACTOR = 1.5f;  // Bei Bedarf z.B. 0.98f oder 1.02f setzen.

const int NOTE_C4 = 262;
const int NOTE_D4 = 294;
const int NOTE_DS4 = 311;
const int NOTE_E4 = 330;
const int NOTE_F4 = 349;
const int NOTE_FS4 = 370;
const int NOTE_G4 = 392;
const int NOTE_GS4 = 415;
const int NOTE_A4 = 440;
const int NOTE_AS4 = 466;
const int NOTE_B4 = 494;
const int NOTE_C5 = 523;
const int NOTE_CS5 = 554;
const int NOTE_D5 = 587;
const int NOTE_DS5 = 622;
const int NOTE_E5 = 659;
const int NOTE_F5 = 698;
const int NOTE_FS5 = 740;
const int NOTE_G5 = 784;
const int NOTE_GS5 = 831;
const int NOTE_A5 = 880;

const int melody[] = {
  NOTE_A4, NOTE_A4, NOTE_A4, NOTE_F4, NOTE_C5,
  NOTE_A4, NOTE_F4, NOTE_C5, NOTE_A4,
  NOTE_E5, NOTE_E5, NOTE_E5, NOTE_F5, NOTE_C5,
  NOTE_GS4, NOTE_F4, NOTE_C5, NOTE_A4,
  NOTE_A5, NOTE_A4, NOTE_A4, NOTE_A5,
  NOTE_GS5, NOTE_G5, NOTE_FS5, NOTE_F5, NOTE_FS5,
  0,
  NOTE_AS4, NOTE_DS5, NOTE_D5, NOTE_CS5, NOTE_C5,
  NOTE_B4, NOTE_C5, 0,
  NOTE_F4, NOTE_GS4, NOTE_F4, NOTE_A4,
  NOTE_C5, NOTE_A4, NOTE_C5, NOTE_E5
};

const int noteDurations[] = {
  500, 500, 500, 350, 150,
  500, 350, 150, 650,
  500, 500, 500, 350, 150,
  500, 350, 150, 650,
  500, 300, 150, 500,
  325, 175, 125, 125, 250,
  325,
  250, 500, 325, 175, 125,
  125, 250, 325,
  250, 500, 350, 125,
  500, 375, 125, 650
};

const int noteCount = sizeof(melody) / sizeof(melody[0]);


void playNote(int frequency, int durationMs) {
  if (frequency > 0) {
    const int tunedFrequency = (int)(frequency * TUNING_FACTOR + 0.5f);
    tone(PIEZO_PIN, tunedFrequency, durationMs);
  }

  delay(durationMs);
  noTone(PIEZO_PIN);
  delay(durationMs / 4);
}


void setup() {
  pinMode(PIEZO_PIN, OUTPUT);
}


void loop() {
  for (int i = 0; i < noteCount; i++) {
    playNote(melody[i], noteDurations[i]);
  }

  delay(2000);
}