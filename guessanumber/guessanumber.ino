/**
 * Khue Q. Nguyen
 * December 18th, 2018
 * Guess a number game for ODRDOID-GO
 */

#include <odroid_go.h>

int playerwin;
int attempts;
int guessednumber;
int randomnumber;
int upbuffer;
int downbuffer;
int abuffer;
int lastguess;
int updateScreen;
int isGameEnd;
const int NO_OF_GUESSES = 7;
const int INPUT_DELAY = 200; // miliseconds

void setup() {
  // put your setup code here, to run once:
  initialize();
}

void loop() {
  // put your main code here, to run repeatedly:
  if (playerwin == 0) {
    // Ask the player for a number and play the game
    if (attempts == NO_OF_GUESSES) {
      updateScreen = 1;
      handleGameOver();
    } else {
      // Player has more attempts
      handleNumberInput();

      // Print information
      printInfo();
    }
  } else {
    updateScreen = 1;
    handleGameWin();
  }

  updateButtonStates();
  GO.update();
}

/**
 * Initialize the game parameters
 */
void initialize() {
  GO.begin();
  GO.lcd.setTextSize(2);
  GO.lcd.clearDisplay(); // Clear the LCD display
  playerwin = 0;
  attempts = 0;
  guessednumber = 0;
  randomnumber = 0;
  srand(7 / 8);
  randomnumber = 1 + rand() % 100;
  // Prevent sticky input with buffer flag
  upbuffer = 0; 
  downbuffer = 0;
  abuffer = 0;
  lastguess = 0;
  // Flag checking whether the screen should be updated
  updateScreen = 1;
  // Check if the game is ended
  isGameEnd = 0;
}

/**
 * Handle game over case
 */
void handleGameOver() {
  // Game Over screen
  if (updateScreen == 1 and isGameEnd == 0) {
    GO.lcd.clear();
    GO.lcd.setCursor(0, 0);
    GO.lcd.println("You lost!");
    GO.lcd.print("Correct Number: ");
    GO.lcd.println(randomnumber);
    updateScreen = 0;
    isGameEnd = 1;
  }

  // When the player press A, reset the game with new random number
  if (GO.BtnA.isPressed() == 1 and abuffer == 0) {
    delay(INPUT_DELAY);
    updateScreen = 1;
    abuffer = 1;
    randomnumber = 1 + rand() % 100;
    attempts = 0;
    playerwin = 0;
    isGameEnd = 0;
  }
}

/**
 * Handle the case when player wins the game
 */
void handleGameWin() {
  // Tell the player that they won!
  if (updateScreen == 1 and isGameEnd == 0) {
    GO.lcd.clear();
    GO.lcd.setCursor(0, 0);
    GO.lcd.println("You won!");
    GO.lcd.print("Correct Number: ");
    GO.lcd.println(randomnumber);
    updateScreen = 0;
    isGameEnd = 1;
  }

  if (GO.BtnA.isPressed() == 1 and abuffer == 0) {
    delay(INPUT_DELAY);
    updateScreen = 1;
    abuffer = 1;
    randomnumber = 1 + rand() % 100;
    attempts = 0;
    playerwin = 0;
    isGameEnd = 0;
  }
}

/**
 * Handle player number input
 */
void handleNumberInput() {
  // Increase the number when UP button is pressed
  if (GO.JOY_Y.isAxisPressed() == 2 and upbuffer == 0) {
    delay(INPUT_DELAY);
    updateScreen = 1;
    upbuffer = 1;
    guessednumber = guessednumber + 1;
  }

  // Decrease the number when DOWN button is pressed
  if (GO.JOY_Y.isAxisPressed() == 1 and downbuffer == 0) {
    delay(INPUT_DELAY);
    updateScreen = 1;
    downbuffer = 1;
    guessednumber = guessednumber - 1;
  }

  // Submit guessed number with A button
  if (GO.BtnA.isPressed() == 1 and abuffer == 0) {
    delay(INPUT_DELAY);
    updateScreen = 1;
    abuffer = 1;

    if (guessednumber == randomnumber) {
      playerwin = 1;
    } else {
      attempts = attempts + 1;
      lastguess = guessednumber;
    }
  }
}

/**
 * Update button press state
 */
void updateButtonStates() {
  if (GO.BtnA.isPressed() != 1) { // A
    abuffer = 0;
  }

  if (GO.JOY_Y.isAxisPressed() != 2) { // DOWN
    downbuffer = 0;
  }

  if (GO.JOY_Y.isAxisPressed() != 1) { // UP
    upbuffer = 0;
  }
}

/**
 * Print information to the screen
 */
void printInfo() {
  if (updateScreen) {
    GO.lcd.clear();
    
    GO.lcd.setCursor(0, 0);
    GO.lcd.print("Attempt: ");
    GO.lcd.println(attempts);
    GO.lcd.print("Number to guess: ");
    GO.lcd.println(guessednumber);
  
    if (attempts == 0) {
      GO.lcd.println("Good luck!");
    } else {
      GO.lcd.print(lastguess);
  
      if (lastguess > randomnumber) {
        GO.lcd.print(" is too high");
      }
  
      if (lastguess < randomnumber) {
        GO.lcd.println(" is too low");
      }
    }

    updateScreen = 0;
  }
}
