byte calibrationEx[5] = {0, 0, 0, 0, 0};
byte input[5] = {0, 0, 0, 0, 0};
unsigned short calibration_pot;

unsigned long calibrationUpdatedAt;
unsigned short calibrationUpdateInterval;

String potStr;
byte val, index;
void initCalibration() {
  potStr.reserve(9);
  lcd.setCursor(0, 0);
  lcd.print("CALIBRATION MODE");
  calibration_pot = random(1, 100)*20;
  for (int i=0; i<5; i++) { //fill calibration array
    calibrationEx[i] = random(0, 4);
  }
  showCalibrationValues();
  calibrationUpdateInterval = 300;
  calibrationUpdatedAt = 0;
  index = 0;
}

void showCalibrationValues() {
  if ((pot == calibration_pot) && (index >= 5)) {
    lcd.setCursor(0, 1);
    lcd.print("CALIBRATION DONE");
    if (calibration != 0) {
      updateCalibration(0);
    }
    calibration = 0;
    return;
  }
  pot = treatPotValue(analogRead(PIN_POT));
  if (pot != calibration_pot) {
    if (digitalRead(PIN_JOY_SW) == LOW) {
      potStr = String(calibration_pot);
    } else {
      potStr = String(100*20 - pot);
    }
  } else {
    potStr = "OK";
  }
  for (int i=potStr.length(); i<5; i++) {//this is for properly showing small values
      potStr+=" ";
  }
  potStr = "FREQ:" + potStr;
  lcd.setCursor(0, 1);
  lcd.print(potStr);
  for (int i=0; i<5; i++) { //show calibration array
    lcd.setCursor(potStr.length() + i, 1);
    lcd.write(byte(calibrationEx[i]));
  }
}

void processCalibration() {
  if ((calibrationUpdateInterval > 0) && (millis() - calibrationUpdatedAt > calibrationUpdateInterval)) { //time to update calibration
    showCalibrationValues();
    if (pot == calibration_pot) {
      val = 255; //default value
      joyX = treatJoystikValue(analogRead(PIN_JOY_VRX));
      joyY = treatJoystikValue(1023 - analogRead(PIN_JOY_VRY)); //1023-val is because joystik is upside-down placed
      switch(joyY) {
        case -1: {
          val = ARROW_LEFT;
          break;
        }
        case 1: {
          val = ARROW_RIGHT;
          break;
        }
      }
      switch(joyX) {
        case -1: {
          val = ARROW_UP;
          break;
        }
        case 1: {
          val = ARROW_DOWN;
          break;
        }
      }
      if (val < 255) { //invert entered value
        val = 3 - val;
      }

      if (val == calibrationEx[index]) {
        calibrationEx[index] = CHECK;
        index++;
      }
    }
  }
}
