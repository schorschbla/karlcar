#pragma once

#include <Arduino.h>

class Servo
{
public:
    Servo(uint8_t pin, uint32_t pulseMinMicros, uint32_t pulseMaxMicros);

    void begin();
    void setValue(double value);

private:
    uint8_t pin;
    uint32_t pulseMinMicros;
    uint32_t pulseMaxMicros;
    hw_timer_t *timer;
    uint32_t pulseMicros;
    uint32_t currentPulseMicros;

    IRAM_ATTR void onTimerInterrupt();
    static IRAM_ATTR void onTimerInterruptArg(void *arg);
};