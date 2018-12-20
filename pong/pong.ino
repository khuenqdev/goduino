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
int ballx = SWIDTH / 2;
int balloldx = ballx;
int bally = SHEIGHT / 2;
int balloldy = bally;
int ballsize = 16;
int ballright = 1;
int balldown = 1;

// Paddle properties
int paddlewidth = 8;
int paddleheight = 64;

// Player properties
int playerx = 0;
int playeroldx = 0;
int playery = 0;
int playeroldy = 0;

// Computer properties
int computerx = SWIDTH - paddlewidth;
int computeroldx = 0;
int computery = 0;
int computeroldy = 0;

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
      drawBall();
      
      // Handle the ball movements
      handleBallMovements();

      // Draw the player
      drawPlayer();

      // Draw the computer
      drawComputer();
      
      // Handle player control
      handlePlayerControls();

      // Handle computer movements
      handleComputerMovements();

      // Change the game state
      if (GO.BtnA.isPressed() and justpressed == 0) {
        justpressed = 1;
        gamestate = 2;
        GO.lcd.clear();
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

/**
 * Draw the ball
 */
void drawBall() {
  // Draw lines with background color to replace old ball positions
  // ballsize + 1 eliminates the extra pixel at the down-right corner of the ball when it moves diagonally up 
  if (ballx > balloldx) {
    GO.lcd.drawFastVLine(balloldx, balloldy, ballsize + 1, BGCOLOR);
  } else {
    GO.lcd.drawFastVLine(balloldx + ballsize, balloldy, ballsize + 1, BGCOLOR);
  }

  if (bally > balloldy) {
    GO.lcd.drawFastHLine(balloldx, balloldy, ballsize + 1, BGCOLOR);
  } else {
    GO.lcd.drawFastHLine(balloldx, balloldy + ballsize, ballsize + 1, BGCOLOR);
  }
  
  GO.lcd.fillRect(ballx, bally, ballsize, ballsize, BALLCOLOR); // Then draw ball at the new position
  
  // Remember last position
  balloldx = ballx;
  balloldy = bally;
}

/**
 * Draw the player
 */
void drawPlayer() {
  if (playery > playeroldy) {
    GO.lcd.drawFastHLine(playeroldx, playeroldy, paddlewidth, BGCOLOR);
  } else {
    GO.lcd.drawFastHLine(playeroldx, playeroldy + paddleheight, paddlewidth, BGCOLOR);
  }
  
  GO.lcd.fillRect(playerx, playery, paddlewidth, paddleheight, PADDLECOLOR);

  // Remember last position
  playeroldx = playerx;
  playeroldy = playery;
}

/**
 * Draw the computer
 */
void drawComputer() {
  if (computery > computeroldy) {
    GO.lcd.drawFastHLine(computeroldx, computeroldy, paddlewidth, BGCOLOR);
  } else {
    GO.lcd.drawFastHLine(computeroldx, computeroldy + paddleheight, paddlewidth, BGCOLOR);
  }
  
  GO.lcd.fillRect(computerx, computery, paddlewidth, paddleheight, PADDLECOLOR);

  // Remember last position
  computeroldx = computerx;
  computeroldy = computery;
}

/**
 * Handle the ball movements
 */
void handleBallMovements() {
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
}

/**
 * Handle player controls
 */
void handlePlayerControls() {
  // If the player presses Up and the paddle is not touching the top of the screen, move the paddle up
  if(GO.JOY_Y.isAxisPressed() == 2 and playery > 0) {
    playery = playery - 1;
  }
  
  // If the player presses down and the paddle is not touching the bottom of the screen, move the paddle down
  if(GO.JOY_Y.isAxisPressed() == 1 and playery + paddleheight < SHEIGHT) {
    playery = playery + 1;
  }
}

/**
 * Handle computer movements
 */
void handleComputerMovements() {
  //If the ball is higher than the computer's paddle, move the computer's paddle up
  if(bally < computery) {
    computery = computery - 1;
  }
  
  //If the bottom of the ball is lower than the bottom of the computer's paddle, move the comptuer's paddle down
  if(bally + ballsize > computery + paddleheight) {
    computery = computery + 1;
  }
}
