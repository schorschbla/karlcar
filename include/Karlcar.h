#include <BLEDevice.h>

#include "Servo.h"

class Karlcar : public BLECharacteristicCallbacks, BLEServerCallbacks 
{
public:
    Karlcar(uint8_t pinServo, uint8_t pinMotorPwm, uint8_t pinMotorForward, uint8_t pinMotorReverse, uint8_t pinServoLights, uint8_t pinLightsPwm);

    void begin();
    void loop();

    void onConnect(BLEClient* pclient);
    void onWrite(BLECharacteristic *pCharacteristic);
    void onDisconnect(BLEClient* pclient);

private:
    Servo servo;
    Servo servoLights;
    uint8_t pinMotorPwm;
    uint8_t pinMotorForward;
    uint8_t pinMotorReverse;
    uint8_t pinServoLights;
    uint8_t pinLightsPwm;

    uint32_t cycles;
    uint32_t lastStop;

    int16_t throttleValue;
    uint32_t throttleValueLastSet;

    int16_t steeringValue;
    uint32_t steeringValueLastSet;

    BLECharacteristic *throttleCharacteristic;
    BLECharacteristic *steeringCharacteristic;

    BLEServer *bleServer;
    BLEService *bleService;
};