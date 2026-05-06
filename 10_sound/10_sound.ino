/**
 * @brief Spielt den Star Wars Imperial March auf einem passiven Piezo-Lautsprecher.
 *
 * Verdrahtung (Beispiel):
 * - Piezo links Signal  -> D8
 * - Piezo rechts Signal -> D9
 * - Beide Piezo GND     -> GND
 *
 * Hinweis:
 * - Das Beispiel erzeugt zwei Kanaele direkt auf D8 und D9.
 * - Ein passiver Piezo eignet sich besser als ein aktiver Summer.
 *
 * @author Steffen Wagner <steffen@swagner.de>
 */

const int LEFT_PIEZO_PIN = 8;
const int RIGHT_PIEZO_PIN = 9;
const float TUNING_FACTOR = 1.00f;
const int NOTE_GAP_DIVISOR = 4;

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


void playStereoTones(int leftFrequency, int rightFrequency, int durationMs) {
  const unsigned long durationUs = (unsigned long)durationMs * 1000UL;
  const unsigned long startUs = micros();
  unsigned long lastToggleLeftUs = startUs;
  unsigned long lastToggleRightUs = startUs;

  bool leftState = false;
  bool rightState = false;

  const unsigned long leftHalfPeriodUs = leftFrequency > 0
    ? 500000UL / (unsigned long)leftFrequency
    : 0;
  const unsigned long rightHalfPeriodUs = rightFrequency > 0
    ? 500000UL / (unsigned long)rightFrequency
    : 0;

  digitalWrite(LEFT_PIEZO_PIN, LOW);
  digitalWrite(RIGHT_PIEZO_PIN, LOW);

  while ((micros() - startUs) < durationUs) {
    const unsigned long nowUs = micros();

    if (leftHalfPeriodUs > 0 && (nowUs - lastToggleLeftUs) >= leftHalfPeriodUs) {
      leftState = !leftState;
      digitalWrite(LEFT_PIEZO_PIN, leftState ? HIGH : LOW);
      lastToggleLeftUs = nowUs;
    }

    if (rightHalfPeriodUs > 0 && (nowUs - lastToggleRightUs) >= rightHalfPeriodUs) {
      rightState = !rightState;
      digitalWrite(RIGHT_PIEZO_PIN, rightState ? HIGH : LOW);
      lastToggleRightUs = nowUs;
    }
  }

  digitalWrite(LEFT_PIEZO_PIN, LOW);
  digitalWrite(RIGHT_PIEZO_PIN, LOW);
}


void playNote(int frequency, int durationMs, int noteIndex) {
  const int tunedFrequency = frequency > 0
    ? (int)(frequency * TUNING_FACTOR + 0.5f)
    : 0;
  const int gapMs = durationMs / NOTE_GAP_DIVISOR;
  const int playTimeMs = durationMs - gapMs;

  if ((noteIndex % 2) == 0) {
    playStereoTones(tunedFrequency, tunedFrequency > 0 ? tunedFrequency / 2 : 0, playTimeMs);
  } else {
    playStereoTones(tunedFrequency > 0 ? tunedFrequency / 2 : 0, tunedFrequency, playTimeMs);
  }

  digitalWrite(LEFT_PIEZO_PIN, LOW);
  digitalWrite(RIGHT_PIEZO_PIN, LOW);
  delay(gapMs);
}


void setup() {
  pinMode(LEFT_PIEZO_PIN, OUTPUT);
  pinMode(RIGHT_PIEZO_PIN, OUTPUT);
  digitalWrite(LEFT_PIEZO_PIN, LOW);
  digitalWrite(RIGHT_PIEZO_PIN, LOW);
}


void loop() {
  for (int i = 0; i < noteCount; i++) {
    playNote(score[i].frequency, score[i].durationMs, i);
  }

  delay(2000);
}