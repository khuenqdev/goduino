/**
 * Khue Q. Nguyen
 * December 18th, 2018
 * Pong clone for ODRDOID-GO
 */
#include <odroid_go.h>
#include "pong.h"

/**
 * Setup the sketch
 */
void setup() {
  GO.begin();
  
  // Seed the random number generator
  srand(7 / 8);

  // Set framerate
  setFrameRate(FPS);
  
  // Set display text size
  GO.lcd.setTextSize(2);

  // Set speaker volume
  GO.Speaker.setVolume(2);

  // Initialize the game
  initialize();
}

/**
 * Main game loop
 */
void loop() {
  if (!nextFrame()) {
    return;
  }
  
  switch (gamestate) {
    case STATE_TITLE:
      showTitleScreen();     
      break;
    case STATE_GAMEPLAY:
      showGameplayScreen();
      break;
    case STATE_WIN:
      showGameEndScreen();      
      break;
    case STATE_GAMEOVER:
      showGameEndScreen();
      break;
  }

  checkFunctionalButtonStates();
  
  GO.update();
}
