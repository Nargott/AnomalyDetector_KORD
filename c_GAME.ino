char game_map[2][16] = {
  {"                "}, 
  {"                "}
};

MapItem playerPos, targetPos, curPos;

TargetUpdateMillis targetUpdateParams;

unsigned long mapUpdatedAt, targetUpdatedAt, ledUpdatedAt, playerUpdatedAt;
unsigned short mapUpdateInterval, targetUpdateInterval, ledUpdateInterval, playerUpdateInterval;

unsigned short score = 0;

void setMap(MapItem* item) {
  game_map[item->y][item->x] = item->symbol;
}

MapItem* getMap(int x, int y) {
  MapItem* item;
  item->symbol = game_map[y][x];
  item->x = x; item->y = y;
  return item;
}

void gameSetup() {
  //game prefs
  mapUpdateInterval = 500;
  targetUpdateInterval = 2000;
  ledUpdateInterval = 100;
  playerUpdateInterval = 200;

  targetUpdateParams.min = MIN_TARGET_UPDATE_DEFAULT;
  targetUpdateParams.max = MAX_TARGET_UPDATE_DEFAULT;
  
  //fill the map for rand numbers
  for (int y=0; y<2; y++) {
    for (int x=0; x<16; x++) {
      game_map[y][x] = 48 + random(0, 9);
    }
  }
}

void gameStart() {
  //set player
  playerPos.x = 2;
  playerPos.y = 1;
  playerPos.symbol = 188;
  setMap(&playerPos);

  //set target
  targetPos.x = 10;
  targetPos.y = 0;
  targetPos.symbol = 239;
  setMap(&targetPos);
  
  displayFullMap(game_map);
}

void replaceTarget(bool isReplace = true) {
  if (isReplace) { //remove the old target
    curPos.x = targetPos.x;
    curPos.y = targetPos.y;
    curPos.symbol = 48 + random(0, 9); //48 is for char '0'
    setMap(&curPos);
    displayMapItem(&curPos);
  }
  
  targetPos.x = random(0, 16);
  targetPos.y = random(0, 2);
  while ((targetPos.x == playerPos.x && targetPos.y == playerPos.y) || //do not place on player
         (curPos.x == targetPos.x && curPos.y == targetPos.y)) { //or on old target place
    targetPos.x = random(0, 16);
    targetPos.y = random(0, 2);
  }
  setMap(&targetPos);
  displayMapItem(&targetPos);

  targetUpdateInterval = random(targetUpdateParams.min, targetUpdateParams.max);

  targetUpdatedAt = millis();
}

signed char joyX, joyY;
void controlPlayer() {
    curPos.x = playerPos.x;
    curPos.y = playerPos.y;
    joyX = treatJoystikValue(analogRead(PIN_JOY_VRX));
    joyY = treatJoystikValue(1023 - analogRead(PIN_JOY_VRY)); //1023-val is because joystik is upside-down placed
    if (((joyX < 0) && (playerPos.x > 0)) || //go left
        ((joyX > 0) && (playerPos.x < 15))) { //go right
      playerPos.x = playerPos.x + joyX;
    }
    if (((joyY < 0) && (playerPos.y > 0)) || //go up
        ((joyY > 0) && (playerPos.y < 1))) { //go down
      playerPos.y = playerPos.y + joyY;
    }
    if ((curPos.x != playerPos.x) || (curPos.y != playerPos.y)) { //player moved
      curPos.symbol = 48 + random(0, 9); //48 is for char '0'
      setMap(&curPos);
      displayMapItem(&curPos);
      setMap(&playerPos);
      displayMapItem(&playerPos);
    }

    //check player/target collision
    if ((targetPos.x == playerPos.x) && (targetPos.y == playerPos.y)) {
      score++;
      beep();
      updateHiScore(score);
      setTargetUpdateByScore();
      replaceTarget(false);
    }

    playerUpdatedAt = millis();
}

void gameLoop() {
  if (viewState == VIEW_GAME) {
    if ((playerUpdateInterval > 0) && (millis() - playerUpdatedAt > playerUpdateInterval)) { //time to update player
      controlPlayer();
    }
    
    if ((mapUpdateInterval > 0) && (millis() - mapUpdatedAt > mapUpdateInterval)) { //time to random!
      curPos.x = random(0, 16);
      curPos.y = random(0, 2);
      while ((curPos.x == playerPos.x && curPos.y == playerPos.y) || //do not place on player
             (curPos.x == targetPos.x && curPos.y == targetPos.y)) { //or target
        curPos.x = random(0, 16);
        curPos.y = random(0, 2);
      }
      curPos.symbol = 48 + random(0, 9); //48 is for char '0'
      
      setMap(&curPos);
      displayMapItem(&curPos);
  
      mapUpdateInterval = random(100, 500);
      mapUpdatedAt = millis();
    }
  
    if ((targetUpdateInterval > 0) && (millis() - targetUpdatedAt > targetUpdateInterval)) { //time to update target
      if (score > 0) {
        score--;
        setTargetUpdateByScore();
      }
      //beep();
      replaceTarget();
    }
    
    if ((ledUpdateInterval > 0) && (millis() - ledUpdatedAt > ledUpdateInterval)) { //time to led
      analogWrite(PIN_LED_GREEN, map((millis() - targetUpdatedAt), 0, targetUpdateInterval, 0, 255) );
      ledUpdatedAt = millis();
    }
  } else { //to prevent counting this on other screens (simulate game paused)
    targetUpdatedAt = millis();
    ledUpdatedAt = millis();
  }
}

void setTargetUpdateByScore() {
  if ((score % 10 == 0) && (targetUpdateParams.max > targetUpdateParams.min)) {
    targetUpdateParams.max = MAX_TARGET_UPDATE_DEFAULT - score*100; //degradation by 1000
  }
  if (targetUpdateParams.max < targetUpdateParams.min) targetUpdateParams.max = targetUpdateParams.min;

  if (score > 25) {
    targetUpdateParams.min = 1000; //minimal degradation
  }
}
