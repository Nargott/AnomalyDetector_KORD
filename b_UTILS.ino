#include <toneAC.h>
#include <EEPROMex.h>
#include <EEPROMVar.h>

signed char treatJoystikValue(int data) {
  if ((data > 480) && (data < 540)) {data = 520;} //dirty hack to set all axises to real middle value
  return map(data, 0, 1023, -1, 1); //-50, 50
}

int treatPotValue(int data) {
  return map(data, 0, 1023, 100, 1)*20;
}

byte getCalibrationLevel() {
  calibration = readCalibration();
  if (calibration < CALIBRATION_MIN) return CALIBRATION_OK;
  if ((calibration >= CALIBRATION_MIN) && (calibration < CALIBRATION_MIDDLE)) return CALIBRATION_WARNING;
  if ((calibration >= CALIBRATION_MIDDLE) && (calibration < CALIBRATION_FULL)) return CALIBRATION_ERROR;
  if (calibration >= CALIBRATION_FULL) return CALIBRATION_FAIL;

  return CALIBRATION_FAIL; //default
}

void checkCalibrationTime() {
  if (millis() % CALIBRATION_TIME_INTERVAL == 0) {
    calibration++;
    updateCalibration(calibration);
  }
}

//display
void displayMapItem(MapItem* item) {
  //MapItem i = &item;
  lcd.setCursor(item->x, item->y);
  lcd.print(item->symbol);
}

void displayFullMap(char game_map[2][16]) {
  for (int y=0; y<2; y++) {
    for (int x=0; x<16; x++) {
      lcd.setCursor(x, y);
      lcd.print(game_map[y][x]);
    }
  }
}

//sound
unsigned long SoundUpdatedAt;
unsigned short SoundRepeatFreq;

void enableSound(int rf) {
  if (SoundRepeatFreq != rf) { //means not enabled yet with this repeat frequency param
    SoundRepeatFreq = rf;
    SoundUpdatedAt = millis() - SoundRepeatFreq - 1; //for immediately play sound 
  }
}
void disableSound() {
  SoundRepeatFreq = 0;
  SoundUpdatedAt = 0;
  noToneAC();
}

void beep() {
  toneAC(TONE_FREQUENCY, 10, 50, true);
}

void processSounds() {
  if ((SoundRepeatFreq > 0) && (millis() - SoundUpdatedAt > SoundRepeatFreq)) {
    SoundUpdatedAt = millis();
    toneAC(TONE_FREQUENCY, 10, 50, true);
  }
}

//EEPROM memory operations
byte readCalibration() {
  return EEPROM.readByte(ADDR_CALIBRATION);
}
void updateCalibration(byte value) {
  EEPROM.updateByte(ADDR_CALIBRATION, value);
}
