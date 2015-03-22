#include "TurnipAnimation.h"

#include <Arduino.h>
#include <avr/pgmspace.h>

#define MID 65
#define LFT (MID + 20)
#define RGT (MID - 20)

//                      r0          r1,         r2          r3          r4,         r5,         r6,         r7, 
#define LED_OFF          0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
#define LED_RED        255,  0,  0,255,  0,  0,255,  0,  0,255,  0,  0,255,  0,  0,255,  0,  0,255,  0,  0,255,  0,  0
#define LED_GRN          0,255,  0,  0,255,  0,  0,255,  0,  0,255,  0,  0,255,  0,  0,255,  0,  0,255,  0,  0,255,  0
#define LED_BLU          0,  0,255,  0,  0,255,  0,  0,255,  0,  0,255,  0,  0,255,  0,  0,255,  0,  0,255,  0,  0,255
#define LED_YEL        255,255,  0,255,255,  0,255,255,  0,255,255,  0,255,255,  0,255,255,  0,255,255,  0,255,255,  0
#define LED_ORANGE     255, 89,  0,255, 89,  0,255, 89,  0,255, 89,  0,255, 89,  0,255, 89,  0,255, 89,  0,255, 89,  0
#define LED_PUR        255,  0,255,255,  0,255,255,  0,255,255,  0,255,255,  0,255,255,  0,255,255,  0,255,255,  0,255
#define LED_RED_GRN      0,255,  0,255,  0,  0,  0,255,  0,255,  0,  0,  0,255,  0,255,  0,  0,  0,255,  0,255,  0,  0
#define LED_RED_BLU    255,  0,  0,  0,  0,255,255,  0,  0,  0,  0,255,255,  0,  0,  0,  0,255,255,  0,  0,  0,  0,255
#define LED_GRN_BLU      0,255,  0,  0,  0,255,  0,255,  0,  0,  0,255,  0,255,  0,  0,  0,255,  0,255,  0,  0,  0,255
#define LED_WHIT1      255,255,255,255,255,255,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
#define LED_WHIT2        0,  0,  0,  0,  0,  0,255,255,255,255,255,255,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
#define LED_WHIT3        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,255,255,255,255,255,255,  0,  0,  0,  0,  0,  0
#define LED_WHIT4        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,255,255,255,255,255,255
#define LED_HALF_PUR   255,  0,255,  0,  0,  0,255,  0,255,  0,  0,  0,255,  0,255,  0,  0,  0,255,  0,255,  0,  0,  0
#define LED_HALF_WHITE 255,255,255,  0,  0,  0,255,255,255,  0,  0,  0,255,255,255,  0,  0,  0,255,255,255,  0,  0,  0

typedef struct TAFrame {
  int nextFrameMs;
  byte servoPos;
  byte leds[24];
} TAFrame;

const TAFrame frames[] PROGMEM = {
  //  ms   p  r0          r1,         r2          r3          r4,         r5,         r6,         r7, 
  { 1000,MID,LED_OFF },
  { 1000,LFT,LED_RED },

  { 1000,LFT,LED_OFF },
  { 1000,RGT,LED_BLU },

  { 1000,RGT,LED_OFF },
  { 1000,LFT,LED_ORANGE },

  { 1000,LFT,LED_OFF },
  { 1000,RGT,LED_GRN },

  { 1000,RGT,LED_OFF },
  { 1000,LFT,LED_YEL },

  { 1000,LFT,LED_OFF },
  { 1000,RGT,LED_BLU },

  { 1000,RGT,LED_WHIT4 },
  { 1000,LFT,LED_WHIT3 },
  // middle dooooooo
  { 1000,LFT,LED_WHIT2 },
  { 1000,RGT,LED_WHIT1 },

  { 1000,RGT,LED_WHIT2 },
  { 1000,LFT,LED_WHIT3 },

  { 1000,LFT,LED_WHIT4 },
  // fast bit
  { 1000,RGT,LED_RED_GRN },

  { 1000,RGT,LED_GRN_BLU },
  { 1000,LFT,LED_RED_BLU },

  { 1000,LFT,LED_YEL },
  { 1000,RGT,LED_BLU },

  { 1000,RGT,LED_GRN },
  { 1000,LFT,LED_RED_BLU },

  { 1000,LFT,LED_YEL },
  { 1000,RGT,LED_PUR },

  { 1000,RGT,LED_GRN_BLU },
  { 1000,LFT,LED_PUR },

  { 1000,LFT,LED_GRN },
  // ending doooooooo
  { 1000,RGT,LED_OFF },

  { 1000,RGT,LED_OFF },
  // repeat
  { 1000,LFT,LED_RED },

  { 1000,LFT,LED_OFF },
  { 1000,RGT,LED_BLU },

  { 1000,RGT,LED_OFF },
  { 1000,LFT,LED_ORANGE },

  { 1000,LFT,LED_OFF },
  { 1000,RGT,LED_GRN },

  { 1000,RGT,LED_OFF },
  { 1000,LFT,LED_YEL },

  { 1000,LFT,LED_OFF },
  { 1000,RGT,LED_BLU },

  { 1000,RGT,LED_WHIT4 },
  { 1000,LFT,LED_WHIT3 },
  // middle dooooooo
  { 1000,LFT,LED_WHIT2 },
  { 1000,RGT,LED_WHIT1 },

  { 1000,RGT,LED_WHIT2 },
  { 1000,LFT,LED_WHIT3 },

  { 1000,LFT,LED_WHIT4 },
  // fast bit
  { 1000,RGT,LED_RED_GRN },

  { 1000,RGT,LED_GRN_BLU },
  { 1000,LFT,LED_RED_BLU },

  { 1000,LFT,LED_YEL },
  { 1000,RGT,LED_BLU },

  { 1000,RGT,LED_GRN },
  { 1000,LFT,LED_RED_BLU },

  { 1000,LFT,LED_YEL },
  { 1000,RGT,LED_PUR },

  { 1000,RGT,LED_GRN_BLU },
  { 1000,LFT,LED_PUR },

  { 1000,LFT,LED_GRN },
  // end
  { 1000,MID,LED_PUR },


  { -1 }
};

int currentFrame;
unsigned long nextFrameTime;

void TurnipAnimation::outputCurrentFrame()
{
  _turnip.write(pgm_read_byte_near(&frames[currentFrame].servoPos));
  for (int led = 0; led < 8; led++) {
    const byte* addr = &frames[currentFrame].leds[led * 3];
    byte r = pgm_read_byte_near(addr);
    byte g = pgm_read_byte_near(addr + 1);
    byte b = pgm_read_byte_near(addr + 2);
    _strip.setPixelColor(led, r, g, b);
  }
  _strip.show();
}

void TurnipAnimation::start()
{
  _turnip.write(MID);

  _strip.clear();
  _strip.show();
  _strip.show();

  currentFrame = 0;
  this->outputCurrentFrame();
  nextFrameTime = millis() + 50; // frames[currentFrame].nextFrameMs;
}

bool TurnipAnimation::stepAnimation()
{
  currentFrame++;
  if (pgm_read_word_near(&frames[currentFrame].nextFrameMs) == -1) {
    currentFrame = -1;
    return false;
  }

  this->outputCurrentFrame();
  nextFrameTime = millis() + 210; // frames[currentFrame].nextFrameMs;
  return true;
}

bool TurnipAnimation::loop()
{
  // Post-show freeze
  if (currentFrame == -1) return true;

  if ((long)(millis() - nextFrameTime) >= 0) {
    return this->stepAnimation();
  }

  return true;
}

void TurnipAnimation::end()
{
  _turnip.write(MID);
  _strip.clear();
  _strip.show();
}

