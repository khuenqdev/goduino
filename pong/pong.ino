/**
 * Khue Q. Nguyen
 * December 18th, 2018
 * Pong clone for ODRDOID-GO
 */
#include <odroid_go.h>

#define BGCOLOR       0x0000 // Black background color
#define BALLCOLOR     0xFFFF // Ball color
#define PADDLECOLOR   0xFFFF // Paddle color
#define BALLSIZE      16
#define PADDLEWIDTH   8
#define PADDLEHEIGHT  64
#define WINSCORE      3
#define FPS           120

// Configuration parameters
int gamestate = 0;
int apressed = 0, menupressed = 0, startpressed = 0;

// Ball properties
int ballx, balloldx, bally, balloldy, ballright, balldown;

// Player properties
int playerx, playeroldx, playery, playeroldy;

// Computer properties
int computerx, computeroldx, computery, computeroldy;

// Scores
int playerscore, computerscore;

void setup() {
  GO.begin();
  GO.lcd.setTextSize(2);
  initialize();
  GO.clear();
}

void loop() {
  if (!GO.nextFrame()) {
    return;
  }
  
  switch (gamestate) {
    case 0:
      // Title screen
      GO.setCursor(SCREEN_WIDTH / 2 - 24, SCREEN_HEIGHT / 2 - 16);
      GO.lcd.println("PONG");
      GO.setCursor(SCREEN_WIDTH / 2 - 76, SCREEN_HEIGHT / 2 - 16 + 20);
      GO.lcd.println("[PRESS START]");

      if (GO.pressed(START_BUTTON) and startpressed == 0) {
        startpressed = 1;
        gamestate = 1;
        initialize();
        GO.clear();
      }
      
      break;
    case 1: // Gameplay screen
      // Draw scores
      drawScores();
      
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

      // Handle scores score changes
      handleScores();

      // Change the game state
      if (GO.pressed(MENU_BUTTON) and menupressed == 0) {
        menupressed = 1;
        gamestate = 0;
        GO.clear();
      }
      break;
    case 2:
      // Win screen
      GO.setCursor(SCREEN_WIDTH / 2 - 48, SCREEN_HEIGHT / 2 - 16);
      GO.lcd.print("YOU WIN!");

      if (GO.pressed(A_BUTTON) and apressed == 0) {
        apressed = 1;
        gamestate = 3;
        GO.clear();
      }
      
      break;
    case 3:
      // Game over screen
      GO.setCursor(SCREEN_WIDTH / 2 - 56, SCREEN_HEIGHT / 2 - 16);
      GO.lcd.print("GAME OVER");

      if (GO.pressed(A_BUTTON) and apressed == 0) {
        apressed = 1;
        gamestate = 0;
        GO.clear();
      }

      break;
  }

  if (GO.notPressed(A_BUTTON)) {
    apressed = 0;
  }

  if (GO.notPressed(MENU_BUTTON)) {
    menupressed = 0;
  }
  
  if (GO.notPressed(START_BUTTON)) {
    startpressed = 0;
  }
  
  GO.update();
}

/**
 * Initialize the game
 */
void initialize() {
  // Seed the random number generator
  srand(7 / 8);

  // Set framerate
  GO.setFrameRate(FPS);

  // Scoring
  playerscore = 0;
  computerscore = 0;
  
  // Ball properties
  ballx = SCREEN_WIDTH / 2 - BALLSIZE / 2;
  balloldx = ballx;
  bally = SCREEN_HEIGHT / 2 - BALLSIZE / 2;
  balloldy = bally;
  ballright = 1;
  balldown = 1;
  
  // Player properties
  playerx = 0;
  playeroldx = 0;
  playery = SCREEN_HEIGHT / 2;
  playeroldy = playery;
  
  // Computer properties
  computerx = SCREEN_WIDTH - PADDLEWIDTH;
  computeroldx = computerx;
  computery = 0;
  computeroldy = 0;
}

/**
 * Draw the scores
 */
void drawScores()
{
  // Draw player scores
  GO.setCursor(SCREEN_WIDTH / 2 - 30, 10);
  GO.lcd.print(playerscore);

  // Draw computer scores
  GO.setCursor(SCREEN_WIDTH / 2 + 20, 10);
  GO.lcd.print(computerscore);

  // Screen separator
  GO.lcd.drawFastVLine(SCREEN_WIDTH / 2, 0, SCREEN_HEIGHT, WHITE);
}

/**
 * Draw the ball
 */
void drawBall() {
  // Draw lines with background color to replace old ball positions
  // BALLSIZE + 1 eliminates the extra pixel at the down-right corner of the ball when it moves diagonally up 
  if (ballx > balloldx) {
    GO.lcd.drawFastVLine(balloldx, balloldy, BALLSIZE + 1, BGCOLOR);
  } else {
    GO.lcd.drawFastVLine(balloldx + BALLSIZE, balloldy, BALLSIZE + 1, BGCOLOR);
  }

  if (bally > balloldy) {
    if (balloldx < 0) {
      GO.lcd.drawFastHLine(0, balloldy, BALLSIZE + balloldx + 1, BGCOLOR);
    } else {
      GO.lcd.drawFastHLine(balloldx, balloldy, BALLSIZE + 1, BGCOLOR);
    }
  } else {
    if (balloldx < 0) {
      GO.lcd.drawFastHLine(0, balloldy + BALLSIZE, BALLSIZE + balloldx + 1, BGCOLOR);
    } else {
      GO.lcd.drawFastHLine(balloldx, balloldy + BALLSIZE, BALLSIZE + 1, BGCOLOR);
    }
  }

  if (ballx < 0) {
    GO.lcd.fillRect(0, bally, BALLSIZE + ballx, BALLSIZE, BALLCOLOR); // Then draw ball at the new position
  } else {
    GO.lcd.fillRect(ballx, bally, BALLSIZE, BALLSIZE, BALLCOLOR); // Then draw ball at the new position
  }
  
  // Remember last position
  balloldx = ballx;
  balloldy = bally;
}

/**
 * Draw the player
 */
void drawPlayer() {
  if (playery > playeroldy) {
    GO.lcd.drawFastHLine(playeroldx, playeroldy, PADDLEWIDTH, BGCOLOR);
  } else {
    GO.lcd.drawFastHLine(playeroldx, playeroldy + PADDLEHEIGHT, PADDLEWIDTH, BGCOLOR);
  }
  
  GO.lcd.fillRect(playerx, playery, PADDLEWIDTH, PADDLEHEIGHT, PADDLECOLOR);

  // Remember last position
  playeroldx = playerx;
  playeroldy = playery;
}

/**
 * Draw the computer
 */
void drawComputer() {
  if (computery > computeroldy) {
    GO.lcd.drawFastHLine(computeroldx, computeroldy, PADDLEWIDTH, BGCOLOR);
  } else {
    GO.lcd.drawFastHLine(computeroldx, computeroldy + PADDLEHEIGHT, PADDLEWIDTH, BGCOLOR);
  }
  
  GO.lcd.fillRect(computerx, computery, PADDLEWIDTH, PADDLEHEIGHT, PADDLECOLOR);

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
  if (ballx == playerx + PADDLEWIDTH and playery < bally + BALLSIZE and playery + PADDLEHEIGHT > bally) {
    ballright = 1;
  }

  // Reflect the ball off of the right side of the screen
  if (ballx + BALLSIZE == computerx and computery < bally + BALLSIZE and computery + PADDLEHEIGHT > bally) {
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
  if(bally + BALLSIZE == SCREEN_HEIGHT - 1) {
    balldown = -1;
  }
}

/**
 * Handle player controls
 */
void handlePlayerControls() {
  // If the player presses Up and the paddle is not touching the top of the screen, move the paddle up
  if(GO.pressed(UP_BUTTON) and playery > 0) {
    playery = playery - 1;
  }
  
  // If the player presses down and the paddle is not touching the bottom of the screen, move the paddle down
  if(GO.pressed(DOWN_BUTTON) and playery + PADDLEHEIGHT < SCREEN_HEIGHT) {
    playery = playery + 1;
  }
}

/**
 * Handle computer movements
 */
void handleComputerMovements() {
  // Computer only reacts when ballx is at half of the board width or if there's a certain chance it happens
  if (ballx >= SCREEN_WIDTH / 2 or rand() % 20 == 1) {
    //If the ball is higher than the computer's paddle, move the computer's paddle up
    if (bally < computery) {
      computery = computery - 1;
    }
    
    //If the bottom of the ball is lower than the bottom of the computer's paddle, move the comptuer's paddle down
    if (bally + BALLSIZE > computery + PADDLEHEIGHT) {
      computery = computery + 1;
    }
  }
}

void handleScores() {
  if(ballx < 0 - (BALLSIZE + 10)) {
    ballx = SCREEN_WIDTH / 2 - BALLSIZE / 2;
    bally = SCREEN_HEIGHT / 2 - BALLSIZE / 2;
    computerscore = computerscore + 1;
    GO.clear();
  }
  
  if(ballx > SCREEN_WIDTH + BALLSIZE + 10) {
    ballx = SCREEN_WIDTH / 2 - BALLSIZE / 2;
    bally = SCREEN_HEIGHT / 2 - BALLSIZE / 2;
    playerscore = playerscore + 1;
    GO.clear();
  }

  if (playerscore == WINSCORE) {
    gamestate = 2;
  }
  
  if (computerscore == WINSCORE) {
    gamestate = 3;
  }
}
