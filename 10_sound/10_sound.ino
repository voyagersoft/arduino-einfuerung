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

struct NoteEvent {
  int frequency;
  int durationMs;
};

const NoteEvent score[] = {
  {NOTE_A4, 500},
  {NOTE_A4, 500},
  {NOTE_A4, 500},
  {NOTE_F4, 350},
  {NOTE_C5, 150},
  {NOTE_A4, 500},
  {NOTE_F4, 350},
  {NOTE_C5, 150},
  {NOTE_A4, 650},
  {NOTE_E5, 500},
  {NOTE_E5, 500},
  {NOTE_E5, 500},
  {NOTE_F5, 350},
  {NOTE_C5, 150},
  {NOTE_GS4, 500},
  {NOTE_F4, 350},
  {NOTE_C5, 150},
  {NOTE_A4, 650},
  {NOTE_A5, 500},
  {NOTE_A4, 300},
  {NOTE_A4, 150},
  {NOTE_A5, 500},
  {NOTE_GS5, 325},
  {NOTE_G5, 175},
  {NOTE_FS5, 125},
  {NOTE_F5, 125},
  {NOTE_FS5, 250},
  {0, 325},
  {NOTE_AS4, 250},
  {NOTE_DS5, 500},
  {NOTE_D5, 325},
  {NOTE_CS5, 175},
  {NOTE_C5, 125},
  {NOTE_B4, 125},
  {NOTE_C5, 250},
  {0, 325},
  {NOTE_F4, 250},
  {NOTE_GS4, 500},
  {NOTE_F4, 350},
  {NOTE_A4, 125},
  {NOTE_C5, 500},
  {NOTE_A4, 375},
  {NOTE_C5, 125},
  {NOTE_E5, 650}
};

const int noteCount = sizeof(score) / sizeof(score[0]);


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
    playNote(score[i].frequency, score[i].durationMs);
  }

  delay(2000);
}