#include <Arduino.h>
#include <BLEUtils.h>
#include <BLEServer.h>

#include "Karlcar.h"

const uint8_t PinServo = 0;
const uint8_t PinMotorPwm = 1;
const uint8_t PinMotorForward = 10;
const uint8_t PinMotorReverse = 3;

Karlcar car(PinServo, PinMotorPwm, PinMotorForward, PinMotorReverse);

void setup()
{
  Serial.begin(9600);
  car.begin();
}

void loop()
{
  car.loop();
}

