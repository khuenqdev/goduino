/**
 * Khue Q. Nguyen
 * December 18th, 2018
 * Sprite display demo for ODROID-GO
 */

#include <odroid_go.h>

// Initialize backgrround sprite
const unsigned char background[] PROGMEM  = {
  0x84, 0x20, 0x9, 0x00, 0x24, 0x00, 0x10, 0x80,
};

// Initialize player sprite
const unsigned char player[] PROGMEM  = {
  0x07, 0xe0, 0x18, 0x18,
  0x20, 0x04, 0x40, 0x02,
  0x48, 0x12, 0x88, 0x11,
  0x88, 0x11, 0x80, 0x01,
  0x80, 0x01, 0x90, 0x09,
  0x88, 0x11, 0x47, 0xe2,
  0x40, 0x02, 0x20, 0x04,
  0x18, 0x18, 0x07, 0xe0,
};

// Player position
int playerx = 5;
int playery = 10;

int updateScreen = 1;
const int INPUT_DELAY = 200;

void setup() {
  // put your setup code here, to run once:
  GO.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  if (updateScreen) {
    GO.lcd.clear();

    //For each column on the screen
    for( int backgroundx = 0; backgroundx < 320; backgroundx = backgroundx + 8 ) {
        //For each row in the column
        for( int backgroundy = 0; backgroundy < 240; backgroundy = backgroundy + 8 ) {
            //Draw a background tile
            GO.lcd.drawBitmap( backgroundx, backgroundy, background, 8, 8, GREEN);
        }
    }

    //Draw black square
    GO.lcd.fillRect(playerx, playery, 16, 16, RED);
    
    // Draw player sprite
    GO.lcd.drawBitmap(playerx, playery, player, 16, 16, WHITE);
    
    updateScreen = 0;
  }

  if (GO.JOY_X.isAxisPressed() == 2) { // LEFT
    delay(INPUT_DELAY);
    playerx = playerx - 5;
    updateScreen = 1;
  }
  
  if (GO.JOY_X.isAxisPressed() == 1) { // RIGHT
    delay(INPUT_DELAY);
    playerx = playerx + 5;
    updateScreen = 1;
  }
  
  if (GO.JOY_Y.isAxisPressed() == 2) { // UP
    delay(INPUT_DELAY);
    playery = playery - 5;
    updateScreen = 1;
  }
  
  if (GO.JOY_Y.isAxisPressed() == 1) { // DOWN
    delay(INPUT_DELAY);
    playery = playery + 5;
    updateScreen = 1;
  }
  
  GO.update();
}
