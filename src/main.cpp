#include <Arduino.h>
#include <BLEUtils.h>
#include <BLEServer.h>

#include "Karlcar.h"

const uint8_t PinServo = 5;
const uint8_t PinServoLights = 0;
const uint8_t PinLights = 6;
const uint8_t PinMotorPwm = 1;
const uint8_t PinMotorForward = 10;
const uint8_t PinMotorReverse = 3;

Karlcar car(PinServo, PinMotorPwm, PinMotorForward, PinMotorReverse, PinServoLights, PinLights);

void setup()
{
  Serial.begin(9600);
  car.begin();
}

void loop()
{
  car.loop();
}

