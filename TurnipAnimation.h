#ifndef TurnipAnimation_h
#define TurnipAnimation_h

#include <Arduino.h>
#include <Servo.h>
#include <Adafruit_NeoPixel.h>

class TurnipAnimation
{
 public:
  TurnipAnimation(Servo& turnip, Adafruit_NeoPixel& strip) : _turnip(turnip), _strip(strip) {};

  void start();
  bool loop();
  void end();

 private:
  void outputCurrentFrame();
  bool stepAnimation();
  Servo& _turnip;
  Adafruit_NeoPixel& _strip;
};

#endif
