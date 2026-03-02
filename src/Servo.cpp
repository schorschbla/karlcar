#include "Servo.h"

const uint32_t CycleLengthMicros = 20000;

Servo::Servo(uint8_t pin, uint32_t pulseMinMicros, uint32_t pulseMaxMicros) : pin(pin), pulseMinMicros(pulseMinMicros), pulseMaxMicros(pulseMaxMicros), timer(nullptr), pulseMicros(0), currentPulseMicros(0)
{
}

void Servo::begin()
{
    pinMode(pin, OUTPUT);

    timer = timerBegin(1000000);
    timerAttachInterruptArg(timer, &onTimerInterruptArg, this);
    timerAlarm(timer, CycleLengthMicros, false, 0);
}

void Servo::setValue(double value)
{
    pulseMicros = pulseMinMicros + constrain(value * (pulseMaxMicros - pulseMinMicros), 0, pulseMaxMicros - pulseMinMicros);
}

void Servo::onTimerInterrupt()
{
    uint64_t alarmValue;
    if (currentPulseMicros != 0 || pulseMicros == 0)
    {
        digitalWrite(pin, 0);
        alarmValue = CycleLengthMicros - pulseMicros;
        currentPulseMicros = 0;
    }
    else
    {
        digitalWrite(pin, 1);
        alarmValue = pulseMicros;
        currentPulseMicros = pulseMicros;
    }
    timerRestart(timer);
    timerAlarm(timer, alarmValue, false, 0);
}


void Servo::onTimerInterruptArg(void *arg)
{
    static_cast<Servo*>(arg)->onTimerInterrupt();
}
