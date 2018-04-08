//JoyStik
#define PIN_JOY_VRX  A1                 // slider variable connecetd to analog pin 0
#define PIN_JOY_VRY  A0
#define PIN_JOY_SW  2

//controls
#define PIN_POT  A2
#define PIN_UP_BTN  4
#define PIN_TOGGLE  5
#define PIN_LED_GREEN  6
#define PIN_LED_RED  7

//common
#define ANT_PIN A3 //TODO: make some hardware noise gen as http://robseward.com/misc/RNG2/

//some constants
#define TONE_FREQUENCY 710 //1400
#define CALIBRATION_IN_POT_VALUE 1000

#define CALIBRATION_MIN 10
#define CALIBRATION_MIDDLE 20
#define CALIBRATION_FULL 30
#define CALIBRATION_MAX 50

#define CALIBRATION_OK 0
#define CALIBRATION_WARNING 1
#define CALIBRATION_ERROR 2
#define CALIBRATION_FAIL 3

const unsigned long CALIBRATION_TIME_INTERVAL = 2*60*1000; //5*60*1000; //2 mins in millis

//views
#define VIEW_START 0
#define VIEW_GAME 1
#define VIEW_WARNING 2
#define VIEW_SCORE 3
#define VIEW_CALIBRATION 4
#define VIEW_FAIL 5

//includes
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

//custom structures
#include "structs.h"

//global vars
LiquidCrystal_I2C lcd(0x27, 16, 2);
byte viewState = 0;
int pot;
byte calibration;
byte calibrationStatus;
int hiScore = 0;

//game params
int MIN_TARGET_UPDATE_DEFAULT = 2000;
int MAX_TARGET_UPDATE_DEFAULT = 10000;

//declare some functions
void displayMapItem(MapItem* item);
void setMap(MapItem* item);
MapItem* getMap(int x, int y);

//EEPROM addresses
#define ADDR_CALIBRATION 0
#define ADDR_HI_SCORE 1

//custom symbols
#define ARROW_UP 0
#define ARROW_DOWN 1
#define ARROW_LEFT 2
#define ARROW_RIGHT 3
#define CHECK 4
const byte arrow_up[8] = {
  B00100,
  B01110,
  B11111,
  B00100,
  B00100,
  B00100,
  B11111,
};
const byte arrow_down[8] = {
  B11111,
  B00100,
  B00100,
  B00100,
  B11111,
  B01110,
  B00100,
};
const byte arrow_left[8] = {
  B00000,
  B00101,
  B01101,
  B11111,
  B01101,
  B00101,
  B00000,
};
const byte arrow_right[8] = {
  B00000,
  B10100,
  B10110,
  B11111,
  B10110,
  B10100,
  B00000,
};
const byte check[8] = {
  B01110,
  B11111,
  B11111,
  B11111,
  B11111,
  B01110,
  B00000,
};
