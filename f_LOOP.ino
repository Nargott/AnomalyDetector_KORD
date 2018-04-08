void loop() {
  processSounds();

  if ((viewState == VIEW_GAME) || (viewState == VIEW_SCORE) || (viewState == VIEW_WARNING)) {
    checkCalibrationTime();
  }

  if ((digitalRead(PIN_JOY_SW) == LOW) && (viewState == VIEW_START)) {
    if (calibrationStatus != CALIBRATION_FAIL) {
      lcd.clear();
      gameStart();
      viewState = VIEW_GAME;
    } else {
      viewState = VIEW_FAIL;
    }
  }
  
  if ((digitalRead(PIN_TOGGLE) == HIGH) && ((viewState == VIEW_GAME) || (viewState == VIEW_WARNING) || (viewState == VIEW_CALIBRATION) || (viewState == VIEW_FAIL))) {
    if (viewState == VIEW_GAME) {
      lcd.clear();
    }
    pot = treatPotValue(analogRead(PIN_POT));
    enableSound(pot);
    if ((viewState == VIEW_FAIL) || ((calibration >= CALIBRATION_MIN) && 
        (((digitalRead(PIN_UP_BTN) == HIGH) && (pot == CALIBRATION_IN_POT_VALUE) && (digitalRead(PIN_UP_BTN) == HIGH)) || (viewState == VIEW_CALIBRATION)))) {
      if (viewState != VIEW_CALIBRATION) {
        initCalibration();
        viewState = VIEW_CALIBRATION;
      }
      processCalibration();
    } else {
      viewState = VIEW_WARNING;
      lcd.setCursor(0, 0);
      lcd.print("    WARNING!    ");
      lcd.setCursor(0, 1);
      lcd.print("FREQ: "+String(pot)+"          ");
      digitalWrite(PIN_LED_RED, HIGH);
      enableSound(pot);
    }
  } else if ((viewState == VIEW_WARNING) || (viewState == VIEW_CALIBRATION)) {
    disableSound();
    lcd.clear();
    displayFullMap(game_map);
    digitalWrite(PIN_LED_RED, LOW);
    viewState = VIEW_GAME;
  }

  if ((digitalRead(PIN_UP_BTN) == HIGH) && ((viewState == VIEW_GAME) || (viewState == VIEW_SCORE))) {
    if (viewState == VIEW_GAME) {
      lcd.clear();
    }
    viewState = VIEW_SCORE;
    lcd.setCursor(0, 0);
    lcd.print("SCORE: "+String(score));
    lcd.setCursor(0, 1);
    lcd.print("HI-SCORE: "+String(hiScore));
    if (score >= hiScore) {
      lcd.setCursor(10, 0);
      lcd.print("LEADER");
    }
  } else if (viewState == VIEW_SCORE) {
    lcd.clear();
    displayFullMap(game_map);
    viewState = VIEW_GAME;
  }
  
  gameLoop();
}
