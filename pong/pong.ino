/**
 * Khue Q. Nguyen
 * December 18th, 2018
 * Pong clone for ODRDOID-GO
 */
#include <odroid_go.h>

void setup() {
  // put your setup code here, to run once:
  GO.begin();
  GO.lcd.clear();
}

void loop() {
  // put your main code here, to run repeatedly:
  GO.lcd.clear():
  GO.update();
}
