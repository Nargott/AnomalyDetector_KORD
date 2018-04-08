void setup() {
  Serial.begin(9600);
  //init custom characters
  lcd.createChar(ARROW_UP, arrow_up);
  lcd.createChar(ARROW_DOWN, arrow_down);
  lcd.createChar(ARROW_LEFT, arrow_left);
  lcd.createChar(ARROW_RIGHT, arrow_right);
  lcd.createChar(CHECK, check);
  
  lcd.begin(); // initialize the LCD
  lcd.backlight(); // Turn on the blacklight

  //inputs
  pinMode(PIN_JOY_SW, INPUT);
  pinMode(PIN_UP_BTN, INPUT);
  pinMode(PIN_TOGGLE, INPUT);

  //outputs
  pinMode(PIN_LED_RED, OUTPUT);
  pinMode(PIN_LED_GREEN, OUTPUT);

  randomSeed(analogRead(ANT_PIN));

  gameSetup();

  viewState = VIEW_START;

  //show start view
  lcd.clear();

  lcd.setCursor(0, 1);
  lcd.print("STATUS: ");
  lcd.setCursor(8, 1);
  calibrationStatus = getCalibrationLevel();
  switch(calibrationStatus) {
    case CALIBRATION_OK: lcd.print("OK"); break;
    case CALIBRATION_WARNING: lcd.print("WARNING"); break;
    case CALIBRATION_ERROR: lcd.print("ERROR"); break;
    case CALIBRATION_FAIL: lcd.print("FAIL"); break;
  }
  if (calibrationStatus != CALIBRATION_FAIL) {
    lcd.setCursor(0, 0);
    lcd.print("[PRESS JOYSTICK]");
  } else {
    lcd.setCursor(0, 0);
    lcd.print("Contact Support!");
  }

  if (calibration < 254) {
    calibration++;
    updateCalibration(calibration);
  }

  hiScore = readHiScore();
  if (hiScore > 254 || hiScore < 0) { //reset hiScore on error
    updateHiScore(0);
  }
}
