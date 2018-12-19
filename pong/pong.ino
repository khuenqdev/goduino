/**
 * Khue Q. Nguyen
 * December 18th, 2018
 * Pong clone for ODRDOID-GO
 */
#include <odroid_go.h>

#define BGCOLOR       0x0000 // Black background color
#define BALLCOLOR     0xFFFF // Ball color
#define PADDLECOLOR   0xFFFF // Paddle color
const int SWIDTH    = TFT_HEIGHT; // Screen width (ODROID-GO has portrait screen, need to flip it around)
const int SHEIGHT   = TFT_WIDTH; // Screen height

// Configuration parameters
int gamestate = 0;
int justpressed = 0;

// Ball properties
int ballx = TFT_HEIGHT / 2;
int balloldx = ballx;
int bally = TFT_WIDTH / 2;
int balloldy = bally;
int ballsize = 16;
int ballright = 1;
int balldown = 1;

// Paddle properties
int paddlewidth = 8;
int paddleheight = 64;
int playerx = 0;
int playeroldx = 0;
int playery = 0;
int playeroldy = 0;

void setup() {
  GO.begin();

  // Seed the random number generator
  srand(7 / 8);

  // Set framerate
  GO.lcd.setFrameRate(60);
  
  GO.lcd.clear();
}

void loop() {
  if (!GO.lcd.nextFrame()) {
    return;
  }
  
  switch (gamestate) {
    case 0:
      // Title screen
      GO.lcd.setCursor(0, 0);
      GO.lcd.println("Title Screen");

      if (GO.BtnA.isPressed() and justpressed == 0) {
        justpressed = 1;
        gamestate = 1;
        GO.lcd.clear();
      }
      
      break;
    case 1: // Gameplay screen
      // Draw the ball
      drawBall(ballx, bally, balloldx, balloldy);

      // Remember last position
      balloldx = ballx;
      balloldy = bally;
      
      // Move the ball right
      if (ballright == 1) {
        ballx = ballx + 1;
      }

      // Move the ball left
      if (ballright == -1) {
        ballx = ballx - 1;
      }

      // Reflect the ball off of the left side of the screen
      if (ballx == 0) {
        ballright = 1;
      }

      // Reflect the ball off of the right side of the screen
      if (ballx + ballsize == SWIDTH - 1) {
        ballright = -1;
      }

      // Move the ball down
      if(balldown == 1) {
        bally = bally + 1;
      }
      
      // Move the ball up
      if(balldown == -1) {
        bally = bally - 1;
      }
      
      // Reflect the ball off of the top of the screen
      if(bally == 0) {
        balldown = 1;
      }
      
      // Reflect the ball off of the bottom of the screen
      if(bally + ballsize == SHEIGHT - 1) {
        balldown = -1;
      }

      // Draw the player's paddle
      drawPaddle(playerx, playery, playeroldx, playeroldy);

      // Remember last position
      playeroldx = playerx;
      playeroldy = playery;
      
      // If the player presses Up and the paddle is not touching the top of the screen, move the paddle up
      if(GO.JOY_Y.isAxisPressed() == 2 and playery > 0) {
        playery = playery - 1;
      }
      
      // If the player presses down and the paddle is not touching the bottom of the screen, move the paddle down
      if(GO.JOY_Y.isAxisPressed() == 1 and playery + paddleheight < SHEIGHT) {
        playery = playery + 1;
      }

      // Change the game state
      if (GO.BtnA.isPressed() and justpressed == 0) {
        justpressed = 1;
        gamestate = 2;
      }
      
      break;
    case 2:
      // Win screen
      GO.lcd.setCursor(0, 0);
      GO.lcd.println("Win Screen");

      if (GO.BtnA.isPressed() and justpressed == 0) {
        justpressed = 1;
        gamestate = 3;
        GO.lcd.clear();
      }
      
      break;
    case 3:
      // Game over screen
      GO.lcd.setCursor(0, 0);
      GO.lcd.println("Game Over Screen");

      if (GO.BtnA.isPressed() and justpressed == 0) {
        justpressed = 1;
        gamestate = 0;
        GO.lcd.clear();
      }

      break;
  }

  if (!GO.BtnA.isPressed()) {
    justpressed = 0;
  }
  
  GO.update();
}

void drawBall(int x, int y, int old_x, int old_y) {
  GO.lcd.fillRect(old_x, old_y, ballsize, ballsize, BGCOLOR); // First fill in the last position with background color
  GO.lcd.fillRect(x, y, ballsize, ballsize, BALLCOLOR); // Then draw ball at the new position
}

void drawPaddle(int x, int y, int old_x, int old_y) {
  GO.lcd.fillRect(old_x, old_y, paddlewidth, paddleheight, BGCOLOR);
  GO.lcd.fillRect(x, y, paddlewidth, paddleheight, PADDLECOLOR);
}
