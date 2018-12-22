#include <odroid_go.h>

// Gamepad shortcuts (Arduboy's style)
#define A_BUTTON BUTTON_A_PIN
#define B_BUTTON BUTTON_B_PIN
#define LEFT_BUTTON 342
#define RIGHT_BUTTON 341
#define UP_BUTTON 352
#define DOWN_BUTTON 351
#define MENU_BUTTON BUTTON_MENU
#define SELECT_BUTTON BUTTON_SELECT
#define START_BUTTON BUTTON_START
#define VOLUME_BUTTON BUTTON_VOLUME

// Screen size shortcuts
#define SCREEN_WIDTH TFT_HEIGHT
#define SCREEN_HEIGHT TFT_WIDTH

// Game constants
#define BGCOLOR       0x0000 // Black background color
#define BALLCOLOR     0xFFFF // Ball color
#define PADDLECOLOR   0xFFFF // Paddle color
#define BALLSIZE      12
#define PADDLEWIDTH   8
#define PADDLEHEIGHT  48
#define WINSCORE      3
#define FPS           120

// Game states
#define STATE_TITLE     0
#define STATE_GAMEPLAY  1
#define STATE_WIN       2
#define STATE_GAMEOVER  3

// Framerate control variables
uint8_t eachFrameMillis;
uint8_t frameRate;
uint16_t frameCount = 0;
long lastFrameStart;
long nextFrameStart = 0;
uint8_t lastFrameDurationMs;
bool post_render = false;

// Configuration parameters
int gamestate = 0, apressed = 0, menupressed = 0, startpressed = 0;

// Ball properties
int ballx, balloldx, bally, balloldy, ballright, balldown;

// Player properties
int playerx, playeroldx, playery, playeroldy;

// Computer properties
int computerx, computeroldx, computery, computeroldy;

// Scores
int playerscore, computerscore;

// Framerate control functions
void setFrameRate(uint8_t rate), idle();
bool nextFrame();

// Gamepad state checking functions
bool pressed(int button), notPressed(int button);

// Game functions
void  initialize(),
      showTitleScreen(),
      showGameplayScreen(),
      showGameEndScreen(),
      checkFunctionalButtonStates(),
      drawScores(),
      drawBall(),
      drawPlayer(),
      drawComputer(),
      handleBallMovements(),
      handlePlayerControls(),
      handleComputerMovements(),
      handleScores();

/**
 * Initialize the game
 */
void initialize() {
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
 * Show the title screen
 */
void showTitleScreen() {
  GO.lcd.setCursor(SCREEN_WIDTH / 2 - 24, SCREEN_HEIGHT / 2 - 16);
  GO.lcd.println("PONG");
  GO.lcd.setCursor(SCREEN_WIDTH / 2 - 76, SCREEN_HEIGHT / 2 - 16 + 20);
  GO.lcd.println("[PRESS START]");
  
  if (pressed(START_BUTTON) and startpressed == 0) {
    startpressed = 1;
    gamestate = STATE_GAMEPLAY;
    initialize();
    GO.lcd.clear();
  }
}

/**
 * Show the game play screen
 */
void showGameplayScreen() {
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
  if (pressed(MENU_BUTTON) and menupressed == 0) {
    menupressed = 1;
    gamestate = STATE_TITLE;
    GO.lcd.clear();
  }
}

/**
 * Show any of the game end screen
 */
void showGameEndScreen() {
  switch (gamestate) {
    case STATE_WIN:
      // Win screen
      GO.lcd.setCursor(SCREEN_WIDTH / 2 - 48, SCREEN_HEIGHT / 2 - 16);
      GO.lcd.print("YOU WIN!");
      break;
    case STATE_GAMEOVER:
      GO.lcd.setCursor(SCREEN_WIDTH / 2 - 56, SCREEN_HEIGHT / 2 - 16);
      GO.lcd.print("GAME OVER");
      break;
  }

  if (pressed(A_BUTTON) and apressed == 0) {
    apressed = 1;
    gamestate = STATE_TITLE;
    GO.lcd.clear();
  }
}

/**
 * Check functional buttons (A, MENU and START) states
 */
void checkFunctionalButtonStates() {
  if (notPressed(A_BUTTON)) {
    apressed = 0;
  }

  if (notPressed(MENU_BUTTON)) {
    menupressed = 0;
  }
  
  if (notPressed(START_BUTTON)) {
    startpressed = 0;
  }
}

/**
 * Draw the scores
 */
void drawScores()
{
  // Draw player scores
  GO.lcd.setCursor(SCREEN_WIDTH / 2 - 30, 10);
  GO.lcd.print(playerscore);

  // Draw computer scores
  GO.lcd.setCursor(SCREEN_WIDTH / 2 + 20, 10);
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
    GO.Speaker.tone(300, 100);
    ballright = 1;
  }

  // Reflect the ball off of the right side of the screen
  if (ballx + BALLSIZE == computerx and computery < bally + BALLSIZE and computery + PADDLEHEIGHT > bally) {
    GO.Speaker.tone(500, 100);
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
    GO.Speaker.tone(100, 100);
    balldown = 1;
  }
  
  // Reflect the ball off of the bottom of the screen
  if(bally + BALLSIZE == SCREEN_HEIGHT - 1) {
    GO.Speaker.tone(700, 100);
    balldown = -1;
  }
}

/**
 * Handle player controls
 */
void handlePlayerControls() {
  // If the player presses Up and the paddle is not touching the top of the screen, move the paddle up
  if(pressed(UP_BUTTON) and playery > 0) {
    playery = playery - 1;
  }
  
  // If the player presses down and the paddle is not touching the bottom of the screen, move the paddle down
  if(pressed(DOWN_BUTTON) and playery + PADDLEHEIGHT < SCREEN_HEIGHT) {
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

/**
 * Handle score calculations
 */
void handleScores() {
  if(ballx < 0 - (BALLSIZE + 10)) {
    ballx = SCREEN_WIDTH / 2 - BALLSIZE / 2;
    bally = SCREEN_HEIGHT / 2 - BALLSIZE / 2;
    computerscore = computerscore + 1;
    GO.Speaker.tone(50, 100);
    GO.lcd.clear();
  }
  
  if(ballx > SCREEN_WIDTH + BALLSIZE + 10) {
    ballx = SCREEN_WIDTH / 2 - BALLSIZE / 2;
    bally = SCREEN_HEIGHT / 2 - BALLSIZE / 2;
    playerscore = playerscore + 1;
    GO.Speaker.tone(50, 100);
    GO.lcd.clear();
  }

  if (playerscore == WINSCORE) {
    gamestate = 2;
  }
  
  if (computerscore == WINSCORE) {
    gamestate = 3;
  }
}

/**
 * Check if a button is pressed
 */
bool pressed(int button) {
  switch (button) {
    case A_BUTTON:
      return GO.BtnA.isPressed() == 1;
    case B_BUTTON:
      return GO.BtnB.isPressed() == 1;
    case UP_BUTTON:
      return GO.JOY_Y.isAxisPressed() == 2;
    case DOWN_BUTTON:
      return GO.JOY_Y.isAxisPressed() == 1;
    case LEFT_BUTTON:
      return GO.JOY_X.isAxisPressed() == 2;
    case RIGHT_BUTTON:
      return GO.JOY_X.isAxisPressed() == 1;
    case START_BUTTON:
      return GO.BtnStart.isPressed() == 1;
    case SELECT_BUTTON:
      return GO.BtnSelect.isPressed() == 1;
    case MENU_BUTTON:
      return GO.BtnMenu.isPressed() == 1;
    case VOLUME_BUTTON:
      return GO.BtnVolume.isPressed() == 1;
  }
}

/**
 * Check if a button is not pressed
 */
bool notPressed(int button) {
  return !pressed(button);
}

/**
 * Set current framerate
 * (Copied from Arduboy library)
 */
void setFrameRate(uint8_t rate)
{
  frameRate = rate;
  eachFrameMillis = 1000 / rate; // How many miliseconds each frame should show
}

/**
 * Check if we can go to the next frame
 * (Copied from Arduboy library)
 */
bool nextFrame()
{
  long now = millis();
  uint8_t remaining;

  // post render
  if (post_render) {
    lastFrameDurationMs = now - lastFrameStart;
    frameCount++;
    post_render = false;
  }

  // if it's not time for the next frame yet
  if (now < nextFrameStart) {
    remaining = nextFrameStart - now;
    // if we have more than 1ms to spare, lets sleep
    // we should be woken up by timer0 every 1ms, so this should be ok
    if (remaining > 1)
      idle();
    return false;
  }

  // pre-render

  // technically next frame should be last frame + each frame but if we're
  // running a slow render we would constantly be behind the clock
  // keep an eye on this and see how it works.  If it works well the
  // lastFrameStart variable could be eliminated completely
  nextFrameStart = now + eachFrameMillis;
  lastFrameStart = now;
  post_render = true;
  
  return post_render;
}

/**
 * Delay everything in 1ms
 * (Modified from Arduboy library)
 */
void idle()
{
  delay(1);
}
