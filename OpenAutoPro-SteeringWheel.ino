#include "DigiKeyboard.h"

#define BUTTONS_A 0 // Had to use digital input for A buttons because digispark was resetting when using A0. This means it only supports a single button. (Mode on the Prius)
#define BUTTONS_B A1

typedef enum Command {
  NONE,
  VOL_UP,
  VOL_DOWN,
  NEXT,
  PREV,
  MODE,
  SHUTDOWN
} Command;

int commandValues[] = { 1006, 365, 648, 0, 157 };
int commandTolerance = 10;

Command lastCommand = NONE;

int modeKeys[] = { KEY_J, KEY_G, KEY_H };
int modeIndex = 0;

unsigned long lastKeyPress = 0;

Command getCommand() {
  int aVal = digitalRead(BUTTONS_A);
  int bVal = analogRead(BUTTONS_B);

  Command aCommand = NONE;
  Command bCommand = NONE;

  if (aVal == 0) {
    aCommand = MODE;
  }

  for(int i=NONE; i<=PREV; i++) {
    if (bVal >= commandValues[i] - commandTolerance && bVal <= commandValues[i] + commandTolerance) {
      bCommand = (Command)i;
    }
  }

  if (aCommand == MODE && bCommand == PREV) {
    return SHUTDOWN;
  }

  if (aCommand == NONE) {
    return bCommand;
  }
}

void setup() {
  pinMode(BUTTONS_A, INPUT_PULLUP);
  pinMode(BUTTONS_B, INPUT);
}

void loop() {
  Command c = getCommand();
  if (lastCommand != c) {
    lastKeyPress = millis();
  }

 if (lastCommand == NONE) {
    if (c == VOL_UP) {
      DigiKeyboard.sendKeyStroke(KEY_F8);
    } else if (c == VOL_DOWN) {
      DigiKeyboard.sendKeyStroke(KEY_F7);
    } else if (c == NEXT) {
      DigiKeyboard.sendKeyStroke(KEY_N);
    } else if (c == PREV) {
      DigiKeyboard.sendKeyStroke(KEY_V);
    } else if (c == MODE) {
      modeIndex++;
      if (modeIndex >= 3) {
        modeIndex = 0;
      }
      DigiKeyboard.sendKeyStroke(modeKeys[modeIndex]);
    } else if (c == SHUTDOWN) {
      DigiKeyboard.sendKeyStroke(KEY_T, MOD_CONTROL_LEFT | MOD_ALT_LEFT);
      DigiKeyboard.delay(300);
      DigiKeyboard.println("shutdown now");

    }
  } else if (millis() - lastKeyPress > 800) {
    if (c == VOL_UP) {
      DigiKeyboard.sendKeyStroke(KEY_F8);
      DigiKeyboard.delay(100);
    } else if (c == VOL_DOWN) {
      DigiKeyboard.sendKeyStroke(KEY_F7);
      DigiKeyboard.delay(100);
    }
  }

  lastCommand = c;
  DigiKeyboard.update();
}