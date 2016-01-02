#include <UTFT.h>
#include <UTouch.h>
#include <UTFT_Geometry.h>
#include <UTFT_Buttons.h>
#include <Wire.h> 
#include <Encoder.h>
#include "si5351.h"

Si5351 clockgen;

const int BUTTON = 10;
long oldPosition  = -999;
long int step = 1;
long int frequency = 14000000;

Encoder myEnc(8, 9);

// Declare which fonts we will be using
extern uint8_t SmallFont[];
extern uint8_t BigFont[];
extern uint8_t Dingbats1_XL[];
UTFT myGLCD(ITDB32S,38,39,40,41);
UTFT_Geometry geo(&myGLCD);
UTouch        myTouch(6,5,4,3,2);
UTFT_Buttons  myButtons(&myGLCD, &myTouch);

void setup()
{
  randomSeed(analogRead(0));
  myGLCD.InitLCD();
  myGLCD.setFont(SmallFont);
  
  myGLCD.clrScr();
  myGLCD.fillScr(VGA_BLUE);
  myGLCD.setColor(VGA_RED);
  myGLCD.fillRoundRect(60, 70, 259, 169);
  
  myGLCD.setColor(VGA_WHITE);
  myGLCD.setBackColor(VGA_RED);
  myGLCD.print("* EI8DRB *", CENTER, 93);
  myGLCD.print("Vector Network Analyser", CENTER, 119);
  myGLCD.print("http://gerryk.com/", CENTER, 132);

  delay (1000);
  myTouch.InitTouch();
  myTouch.setPrecision(PREC_MEDIUM);
  
  myButtons.setTextFont(BigFont);
  myButtons.setSymbolFont(Dingbats1_XL);

  clockgen.init(SI5351_CRYSTAL_LOAD_8PF);
  delay (2000);
  pinMode (BUTTON, INPUT);
  digitalWrite (BUTTON, HIGH); // Pull High Restance
  clockgen.set_pll(SI5351_PLL_FIXED, SI5351_PLLA);
  clockgen.set_freq(frequency, SI5351_PLL_FIXED, SI5351_CLK0);
  myGLCD.clrScr();
  redrawScreen();
}

void loop()
{
  int but1, but2, but3, but4, butX, butY, pressed_button;
  boolean default_colors = true;
  
  but1 = myButtons.addButton( 10,  20, 300,  30, "Auto Sweep");
  but2 = myButtons.addButton( 10,  60, 300,  30, "Manual Sweep");
  myButtons.drawButtons();
  while(1) 
  {
    if (myTouch.dataAvailable() == true)
    {
      pressed_button = myButtons.checkButtons();
      if (pressed_button==but1)
        autoSweep();
      if (pressed_button==but1)
        manualSweep();
    }
  }
  if (!(digitalRead(BUTTON)))  {
    freqSet();
    myGLCD.clrScr();
    redrawScreen();
  }
}

void freqSet()  {
    myGLCD.setColor(VGA_RED);
    myGLCD.fillRoundRect(50, 70, 269, 169);
    myGLCD.setColor(VGA_WHITE);
    myGLCD.setBackColor(VGA_RED);
    myGLCD.print(">> Start Freq: 14000000 <<", CENTER, 90);
    myGLCD.print("   End Freq:   15000000   ", CENTER, 105);
    myGLCD.print("   Step:       100KHz     ", CENTER, 120);
    delay(3000);
}    

void autoSweep()  {
    myGLCD.setColor(VGA_RED);
    myGLCD.fillRoundRect(50, 70, 269, 169);
    myGLCD.setColor(VGA_WHITE);
    myGLCD.setBackColor(VGA_RED);
    myGLCD.print("AUTO SWEEP", CENTER, 50);    
    myGLCD.print(">> Start Freq: 14000000 <<", CENTER, 90);
    myGLCD.print("   End Freq:   15000000   ", CENTER, 105);
    myGLCD.print("   Step:       100KHz     ", CENTER, 120);
    delay(3000);
}    
void manualSweep()  {
    myGLCD.setColor(VGA_RED);
    myGLCD.fillRoundRect(50, 70, 269, 169);
    myGLCD.setColor(VGA_WHITE);
    myGLCD.setBackColor(VGA_RED);
    myGLCD.print("MANUAL SWEEP", CENTER, 50);    
    myGLCD.print(">> Start Freq: 14000000 <<", CENTER, 90);
    myGLCD.print("   End Freq:   15000000   ", CENTER, 105);
    myGLCD.print("   Step:       100KHz     ", CENTER, 120);
    delay(3000);
}    


void redrawScreen()  { 
  myGLCD.setColor(255, 0, 0);
  myGLCD.fillRect(0, 0, 319, 13);
  myGLCD.setColor(64, 64, 64);
  myGLCD.fillRect(0, 226, 319, 239);
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(255, 0, 0);
  myGLCD.print("* EI8DRB VNA *", CENTER, 1);
  myGLCD.setBackColor(64, 64, 64);
  myGLCD.setColor(255,255,0);
  myGLCD.print("<http://gerryk.com/>", CENTER, 227);

  myGLCD.setColor(0, 0, 255);
  myGLCD.drawRect(0, 14, 319, 225);

// Draw crosshairs
  myGLCD.setColor(0, 0, 255);
  myGLCD.setBackColor(0, 0, 0);
  myGLCD.drawLine(159, 15, 159, 224);
  myGLCD.drawLine(1, 119, 318, 119);
  for (int i=9; i<310; i+=10)
    myGLCD.drawLine(i, 117, i, 121);
  for (int i=19; i<220; i+=10)
    myGLCD.drawLine(157, i, 161, i); 
}
