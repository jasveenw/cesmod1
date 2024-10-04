#include <TFT_eSPI.h> 
#include <SPI.h>
#define GGREEN 0x1642
#define BREAD 0xB142 // i cant name colors
// #define CREAM 0xFFFA unused color

TFT_eSPI tft = TFT_eSPI(); 

const int16_t num = 45; // number of leaves
int16_t leavesx[num]; 
int16_t leavesy[num]; //coordinates of each leaf
int16_t speeds[num]; // fall distance per cycle
int16_t summer; //pixel height for "summer" colors
int16_t transition; //pixel height for transition colors

void setup() {
  tft.init();
  tft.setRotation(1);   // 1 = landscape
  tft.fillScreen(TFT_BLACK);
  summer = tft.height() / 4; //top 1/4 of screen is summer colors
  transition = tft.height() / 2; //area between 1/4 and 1/2 of screen is transition, below 1/2 is fall

  for (int16_t i = 0; i < num; i++){ // initialize leaf positions
    leavesx[i] = random(tft.width()); //random x coordinate 
    leavesy[i] = 0; //always start at top
    speeds[i] = random(1, 5); // initialize leaf speeds per leaf
  }
}

void loop() {
  fall();
}

void fall() {
  tft.fillScreen(TFT_BLACK); // deletes old leaves !! 
  uint16_t summercols[] = {TFT_GREEN, TFT_DARKGREEN, GGREEN, TFT_OLIVE}; //summer leaf colors
  uint16_t transitioncols[] = {TFT_YELLOW, TFT_ORANGE, TFT_GREENYELLOW, TFT_GOLD}; //transition leaf colors
  uint16_t fallcols[] = {TFT_RED, TFT_BROWN, TFT_MAROON, BREAD}; //fall leaf colors
  for (int16_t i = 0; i < num; i++){ //for each leaf
    uint16_t col = random(4); //random color option
    int16_t x = leavesx[i]; //defined to reduce array calls within function for speed
    int16_t y = leavesy[i]; //^
    int16_t color; //sets color from season and random option within season
    if (y < summer){
      color = summercols[col];
    }
    else if (y < transition){
      color = transitioncols[col];
    }
    else{
      color = fallcols[col];
    }

    tft.fillTriangle(x, y, x - 2, y + 3, x + 2, y + 3, color); //each leaf represented as triange

    leavesy[i] += speeds[i]; //increase y by "speed" of leaf movement
    int16_t movement = random(3); //if movement is 1 leaf doesnt go left or right
    if (movement == 0){ //leaf moves to the left by speed value
      leavesx[i] -= speeds[i];
      if (leavesx[i] < 0){ //sets leaf to left edge if goes too far
        leavesx[i] = 0;
      }
    }
    else if (movement == 2){ //leaf moves to right by speed value
      leavesx[i] += speeds[i];
      if (leavesx[i] > tft.width()){ //sets leaf to right edge if goes too far
        leavesx[i] = tft.width();
      }
    }

    if (leavesy[i] > tft.height()){ 
      leavesy[i] = 0; // restart from top when reaching the bottom
    }
  }
  delay(50);
}