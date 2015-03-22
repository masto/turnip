/**
 * \file Turnip.ino
 *
 * \brief Automated turnip.
 * \remarks comments are implemented with Doxygen Markdown format
 *
 * \author Christopher Masto
 *
 * This sketch automates a turnip.
 */

#include <SPI.h>

#include <Servo.h>
#include <SdFat.h>
#include <SdFatUtil.h>
#include <SFEMP3Shield.h>
#include <Adafruit_NeoPixel.h>

#include "TurnipAnimation.h"

// Connections
#define DOOR_SWITCH_PIN 5
#define LED_STRIP_PIN 3
#define TURNIP_SERVO_PIN 10

// Overall brightness of light show (0-255)
#define LED_STRIP_BRIGHTNESS 180

// Time for debouncing door switch (ms)
#define DEBOUNCE_DELAY 50

// ms to wait between loop iterations
#define LOOP_DELAY (10)

#define DOOR_OPEN true

SdFat sd;
SFEMP3Shield MP3player;
Adafruit_NeoPixel strip = Adafruit_NeoPixel(8, LED_STRIP_PIN);
Servo turnip;
TurnipAnimation animation = TurnipAnimation(turnip, strip);

bool doorSwitch() {
  static long lastDebounceTime = 0;
  static bool switchState = false;
  static bool lastSwitchState = false;

  bool reading = digitalRead(DOOR_SWITCH_PIN) == HIGH;
  if (reading != lastSwitchState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > DEBOUNCE_DELAY) {
    switchState = reading;
  }

  lastSwitchState = reading;

  return switchState;
}

void setup() {
  Serial.begin(115200);

  pinMode(DOOR_SWITCH_PIN, INPUT_PULLUP);

  // Initialize LED strip.
  strip.begin();
  strip.setBrightness(LED_STRIP_BRIGHTNESS);
  strip.clear();
  strip.show();

  // Initialize the SdCard.
  if(!sd.begin(SD_SEL, SPI_FULL_SPEED)) sd.initErrorHalt();
  if(!sd.chdir("/")) sd.errorHalt("sd.chdir");

  // Initialize the MP3 Player Shield
  uint8_t result = MP3player.begin();
  if(result != 0 && result != 6) {
    Serial.print(F("Error code: "));
    Serial.print(result);
    Serial.println(F(" when trying to start MP3 player"));
  }

  // Now shut down until it's time to play
  MP3player.end();

  turnip.attach(TURNIP_SERVO_PIN);
  turnip.write(65);
  delay(100);
  turnip.detach();
}

bool isPlaying = false;

void loop() {
  if (isPlaying) {
    // Currently playing
    if (doorSwitch() == DOOR_OPEN) {
      if (MP3player.isPlaying() == false) {
        Serial.print(F("song end: "));
        Serial.println(millis());
      }
      if (animation.loop() == false) {
        //MP3player.end();
        turnip.detach();
        Serial.print(F("anim end: "));
        Serial.println(millis());
      };
    }
    else {
      // Stop the show
      Serial.println(millis());
      isPlaying = false;
      MP3player.end();
      animation.end();
      turnip.write(65);
      delay(100);
      turnip.detach();
    }
  }
  else {
    // Currently not playing
    if (doorSwitch() == DOOR_OPEN) {
      // Start the show
      Serial.print(F("start show: "));
      Serial.println(millis());
      turnip.attach(TURNIP_SERVO_PIN);
      isPlaying = true;
      playSong();
      animation.start();
    }
  }

  delay(LOOP_DELAY);
}

void playSong() {
  MP3player.begin();

  uint8_t result = MP3player.playTrack(1);
  if (result != 0) {
    Serial.print(F("Error code: "));
    Serial.print(result);
    Serial.println(F(" when trying to play track"));
  }
}
