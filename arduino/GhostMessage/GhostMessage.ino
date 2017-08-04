// testshapes demo for Adafruit RGBmatrixPanel library.
// Demonstrates the drawing abilities of the RGBmatrixPanel library.
// For 16x32 RGB LED matrix:
// http://www.adafruit.com/products/420

// Written by Limor Fried/Ladyada & Phil Burgess/PaintYourDragon
// for Adafruit Industries.
// BSD license, all text above must be included in any redistribution.

#include <Adafruit_GFX.h>   // Core graphics library
#include <RGBmatrixPanel.h> // Hardware-specific library

#define CLK 8  // MUST be on PORTB! (Use pin 11 on Mega)
#define LAT 10
#define OE  9
#define A   A0
#define B   A1
#define C   A2
RGBmatrixPanel matrix(A, B, C, CLK, LAT, OE, false);

const unsigned char PROGMEM ghost[] =
{
B00000111,B10000000,
B00011111,B11100000,
B00111111,B11110000,
B01111111,B11111000,
B01111111,B11111000,
B01111111,B11111000,
B01111111,B11111000,
B11111111,B11111100,
B11111111,B11111100,
B11111111,B11111100,
B11111111,B11111100,
B11111111,B11111100,
};

const unsigned char PROGMEM skirtA[] =
{
B11011100,B11101100,
B10001100,B11000100
};

const unsigned char PROGMEM skirtB[] =
{
B11110111,B10111100,
B01100011,B00011000
};


const unsigned char PROGMEM eyeWhite[] = {
  B01100000,
  B11110000,
  B11110000,
  B11110000,
  B01100000
};

const unsigned char PROGMEM pupil[] = {
  B11000000,
  B11000000
};

//const uint16_t BLUE = matrix.Color444(0,0,120);
const uint16_t WHITE = matrix.ColorHSV(0, 0, 90, true);
const uint16_t RED = matrix.ColorHSV(0, 100, 100, true);

const uint16_t GREEN = 0x0080; //matrix.ColorHSV(0, 120, 90, true);
const uint16_t BLUE = 0x0002;

const uint16_t PINK = matrix.Color333(6,1,1);
const uint16_t ORANGE = matrix.Color333(4,1,0);
const uint16_t CYAN = matrix.Color333(0,2,1);

bool toggle = true;
bool dir = true;
int ghostx = -14;
int ghosty = 1;
int messageWidth = 0;

unsigned long previousMillis = 0;
long interval = 10000;

String lastMessage = "";
String sentMessage = "";
String ghostColor = "orange";

uint16_t GHOSTC = ORANGE;

void drawGhost(int x, int y, uint16_t color, bool direction, bool skirt){

  matrix.drawBitmap(x,y,ghost,14,14,color);

  if(direction){
    matrix.drawBitmap(x+3,y+3,eyeWhite,4,5,WHITE);
    matrix.drawBitmap(x+5,y+5,pupil,2,2,BLUE);

    matrix.drawBitmap(x+9,y+3,eyeWhite,4,5,WHITE);
    matrix.drawBitmap(x+11,y+5,pupil,2,2,BLUE);
  }else{
    matrix.drawBitmap(x+1,y+3,eyeWhite,4,5,WHITE);
    matrix.drawBitmap(x+1,y+5,pupil,2,2,BLUE);

    matrix.drawBitmap(x+7,y+3,eyeWhite,4,5,WHITE);
    matrix.drawBitmap(x+7,y+5,pupil,2,2,BLUE);
  }


  if(skirt){
    matrix.drawBitmap(x,y+12,skirtA,14,2,color);
  }else{
    matrix.drawBitmap(x,y+12,skirtB,14,2,color);
  }
}


void drawMessage(int x, int y, String message){ //char *message
  matrix.setCursor(x, y);
  matrix.setTextColor(GREEN);
  matrix.print(message);
}

void setup() {
  Serial.begin(9600);
  matrix.begin();
  matrix.setTextWrap(false);
  matrix.setTextSize(1);

  matrix.setTextColor(WHITE);

  matrix.setCursor(0,0);
  matrix.print("Ghost");
  matrix.setCursor(0,9);
  matrix.print("Message");
  drawGhost(18,ghosty,GREEN,false,true);

  delay(4000);
  matrix.fillScreen(0);

}

void loop() {

  unsigned long currentMillis = millis();

  while (Serial.available() > 0) {
     sentMessage = Serial.readStringUntil(';');
     ghostColor = Serial.readString();
    Serial.print("Received Message: ");
    Serial.print(sentMessage);
    Serial.print("Color: ");
    Serial.println(ghostColor);

    if(ghostColor == "red"){
      GHOSTC = RED;
    }else if(ghostColor == "pink"){
      GHOSTC = PINK;
    }else if(ghostColor == "orange"){
      GHOSTC = ORANGE;
    }else if(ghostColor == "cyan"){
      GHOSTC = CYAN;
    }else{
      GHOSTC = GREEN;
    }

  }

  if(sentMessage != lastMessage){

    messageWidth = sentMessage.length() * -6;
    matrix.fillScreen(0);
    toggle = (toggle) ? false: true;
    drawGhost(ghostx,ghosty,GHOSTC,dir,toggle);

    if(dir == true){
      ghostx++;
    }else if(dir == false){
      ghostx--;
    }

    if(!dir){
      drawMessage(ghostx + 15, 4, sentMessage);
    }

    if(ghostx > matrix.width() && dir == true){
      dir = false;
    }else if(ghostx < messageWidth - 14 - matrix.width() && dir == false) {
      dir = true;
      ghostx = -14;
      lastMessage = sentMessage;
    }
    delay(50);
  }


  if(previousMillis + interval > currentMillis){

  }

}
