//Erik Pope (Agent6)
//9/3/1018
//TicTacToe

#include <odroid_go.h>   
uint16_t BlockImage[8][12][12];   // Block
uint16_t backBuffer[64][120];     // GAME AREA
const int Length = 12;            // the number of pixels for a side of a block
const int Width  = 10;            // the number of horizontal blocks
const int Height = 20;            // the number of vertical blocks
int game_speed = 100;              // 25msec

const unsigned int BCKGRDCOL = GO.lcd.color565(0,0,0);

int8_t selectx = 1;
int8_t selecty = 1;
int8_t board[3][3]={{0,0,0},
                    {0,0,0},
                    {0,0,0}};
int8_t player = 1;
int8_t turn = 1;

const unsigned char select[] PROGMEM  = {
    0x00, 0x00, 0x3c, 0x4, 0x4, 0x4, 0x00, 0x00, 0x00, 0x00, 0x00, 0x4, 0x4, 0x4, 0x3c, 0x00, 0x00, 0x00, 0x78, 0x40, 0x40, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x40, 0x40, 0x78, 0x00,  
};

const unsigned char x[] PROGMEM  = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x30, 0xf0, 0x80, 0x80, 0xd0, 0x70, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x38, 0x2c, 0x6, 0x3, 0xf, 0x38, 0x30, 0x20, 0x00, 0x00, 0x00,  
};

const unsigned char o[] PROGMEM  = {
    0x00, 0x00, 0x00, 0x00, 0xc0, 0xe0, 0x20, 0x70, 0x50, 0x10, 0x30, 0xe0, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf, 0x1f, 0x38, 0x30, 0x20, 0x20, 0x30, 0x18, 0xf, 0x00, 0x00, 0x00, 
};

void setup() {
  Serial.begin(115200);         // SERIAL
  GO.begin();                   // M5STACK INITIALIZE
  GO.lcd.setBrightness(200);    // BRIGHTNESS = MAX 255
  GO.lcd.fillScreen(BLACK);     // CLEAR SCREEN
  GO.lcd.setRotation(1);        // SCREEN ROTATION = 0
}

void tie() {
  GO.lcd.setCursor(48, 57);
  GO.lcd.print(F("Tie!!!"));
  reset();
}

void win() {
  GO.lcd.setCursor(36, 57);
  GO.lcd.print(F("You Win!!!"));
  reset();
}

//Draw game board
void drawBoard(){
    GO.lcd.setCursor(42, 0);
    GO.lcd.print(F("Player "));
    GO.lcd.print(player);
    GO.lcd.fillRect(56, 8, 1, 48, WHITE);
    GO.lcd.fillRect(72, 8, 1, 48, WHITE);
    GO.lcd.fillRect(40, 24, 48, 1, WHITE);
    GO.lcd.fillRect(40, 40, 48, 1, WHITE);
    //This is the Game cursor
    GO.lcd.drawBitmap(selectx*16 + 40, selecty*16 + 8, (const uint8_t *)select, 16, 16, WHITE);

}

void drawMoves(){
  for(uint8_t ax=0; ax < 3; ax = ax + 1){
      for(uint8_t ay=0; ay < 3; ay = ay + 1){
        if(board[ax][ay] == 1){
          GO.lcd.drawBitmap(ax*16 + 40, ay*16 + 8, (const uint8_t *)x, 16, 16, WHITE);
        }else if(board[ax][ay] == 2){
          GO.lcd.drawBitmap(ax*16 + 40, ay*16 + 8, (const uint8_t *)o, 16, 16, WHITE);
        }
      }
    }
}


void resetBoard(){
  for(uint8_t ax=0; ax < 3; ax = ax + 1){
      for(uint8_t ay=0; ay < 3; ay = ay + 1){
        board[ax][ay]=0;       
      }
  }  
}

//Set all values back to default
void reset() {
    drawBoard();
    delay(2500);
    selectx = 1;
    selecty = 1;
    player = 1;
    turn = 1;
    resetBoard();
}

void gameInput(){
  if(GO.JOY_X.isAxisPressed() == 2) {
    GO.lcd.drawBitmap(0, 0, 120, 64, (uint16_t *)backBuffer);
      if(selectx > 0) {
        selectx -= 1;
      }else{
        selectx = 2;
      }
    }
    
    if(GO.JOY_X.isAxisPressed() == 1) {
      GO.lcd.drawBitmap(0, 0, 120, 64, (uint16_t *)backBuffer);
      if(selectx < 2) {
        selectx += 1;
      }else{
        selectx = 0;
      }
    }
    
    if(GO.JOY_Y.isAxisPressed() == 2) {
      GO.lcd.drawBitmap(0, 0, 120, 64, (uint16_t *)backBuffer);
      if(selecty > 0) {
        selecty -= 1;
      }else{
        selecty = 2;
      }
    }
    
    if(GO.JOY_Y.isAxisPressed() == 1) {
      GO.lcd.drawBitmap(0, 0, 120, 64, (uint16_t *)backBuffer);
      if(selecty < 2) {
        selecty += 1;
      }else{
        selecty = 0;
      }
    }
    
    //Place X or O if Blank
    if(GO.BtnA.isPressed() == 1) {
      GO.lcd.drawBitmap(0, 0, 120, 64, (uint16_t *)backBuffer);
        if(board[selectx][selecty] == 0){
          board[selectx][selecty] = player;
          turn = turn + 1;
          if(player == 1){
            player=2;
          }else{
            player=1;
          }
        }
    }
    
    //Not used    
    if(GO.BtnB.isPressed() == 1) {   
    }
}

void gameLogic(){
    // check rows
    for (uint8_t i = 0; i < 3; ++i)
      if(board[0][i]!=0 && board[0][i]==board[1][i] && board[0][i]==board[2][i])
        win();
    
    // check columns
    for (uint8_t i = 0; i < 3; ++i)
      if(board[i][0]!=0 && board[i][0]==board[i][1] && board[i][0]==board[i][2])
        win();
    
    // check diagonal 1
    if(board[0][0]!=0 && board[0][0]==board[1][1] && board[0][0]==board[2][2])
      win();
    
    // check diagonal 2
    if(board[2][0]!=0 && board[2][0]==board[1][1] && board[2][0]==board[0][2])
      win();
    
    // Go to Tie function after 9th turn
    if(turn>9){;
      tie();
    }
}

void loop() {
    delay(game_speed);
    gameInput();  
    drawMoves();
    gameLogic();   
    drawBoard();
    GO.update();
}
