#include "Karlcar.h"

#define SERVICE_UUID                    "fdddb4fa-cce0-4ec9-bdb0-fa658612e1e9"
#define THROTTLE_CHARACTERISTIC_UUID    "48ec335b-ce42-4b18-9cfd-c2c51223a5cc"
#define STEERING_CHARACTERISTIC_UUID    "d316f06f-9cf5-4562-90fe-85cc68fa678c"

#define BLE_MAX_IDLE_TIME_MILLIS    3000

const float ServoLightDownValue = 0.55;
const float ServoLightUpValue = 0.46;


Karlcar::Karlcar(uint8_t pinServo, uint8_t pinMotorPwm, uint8_t pinMotorForward, uint8_t pinMotorReverse, uint8_t pinServoLights, uint8_t pinLightsPwm) : 
    servo(pinServo, 500, 2500),
    servoLights(pinServoLights, 500, 2500),
    pinMotorPwm(pinMotorPwm),
    pinMotorForward(pinMotorForward),
    pinMotorReverse(pinMotorReverse),
    pinServoLights(pinServoLights),
    pinLightsPwm(pinLightsPwm),
    cycles(0),
    lastStop(0),
    throttleValue(NAN),
    throttleValueLastSet(0),
    steeringValue(NAN),
    steeringValueLastSet(0),
    bleServer(nullptr),
    bleService(nullptr),
    throttleCharacteristic(nullptr),
    steeringCharacteristic(nullptr)
{
}

void Karlcar::begin()
{
    analogWriteFrequency(pinMotorPwm, 20000);
    pinMode(pinMotorForward, OUTPUT);
    pinMode(pinMotorReverse, OUTPUT);

    pinMode(pinLightsPwm, OUTPUT);

    servo.begin();
    servo.setValue(0.5);

    servoLights.begin();
    servoLights.setValue(ServoLightDownValue);

    BLEDevice::init("Karlcar");
    bleServer = BLEDevice::createServer();
    bleServer->setCallbacks(this);
    bleService = bleServer->createService(SERVICE_UUID);
    throttleCharacteristic = bleService->createCharacteristic(THROTTLE_CHARACTERISTIC_UUID, BLECharacteristic::PROPERTY_WRITE);
    throttleCharacteristic->setCallbacks(this);
    steeringCharacteristic = bleService->createCharacteristic(STEERING_CHARACTERISTIC_UUID, BLECharacteristic::PROPERTY_WRITE);
    steeringCharacteristic->setCallbacks(this);

    bleService->start();
    
    BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(SERVICE_UUID);
    pAdvertising->setScanResponse(true);
    BLEDevice::startAdvertising();
}

void Karlcar::onConnect(BLEClient* pclient)
{
}

static bool getShort(BLECharacteristic *characteristic, int16_t &value)
{
    if (characteristic->getLength() == 2)
    {
        uint8_t *data = characteristic->getData();
        value = data[0] | (data[1] << 8);
        return true;
    }
    return false;
}

void Karlcar::onWrite(BLECharacteristic *characteristic)
{
    uint32_t now = millis();
    if (characteristic == throttleCharacteristic)
    {
        if (getShort(throttleCharacteristic, throttleValue))
        {
            throttleValueLastSet = now;
        }
    }
    else if (characteristic == steeringCharacteristic)
    {
        if (getShort(steeringCharacteristic, steeringValue))
        {
            steeringValueLastSet = now;
        }
    }
}

void Karlcar::onDisconnect(BLEClient* pclient)
{

}

void Karlcar::loop()
{
    servo.setValue(-steeringValue / (float)INT16_MAX + 0.5);

    if (millis() - throttleValueLastSet > BLE_MAX_IDLE_TIME_MILLIS)
    {
        throttleValue = 0;
    }

    uint8_t motorPwmValue = 0;

    if (throttleValue == 0)
    {
        digitalWrite(pinMotorForward, LOW);
        digitalWrite(pinMotorReverse, LOW);
        analogWrite(pinMotorPwm, 0);
        if (lastStop == 0) {
            lastStop = cycles;
        } else if (cycles > lastStop + 250) {
            servoLights.setValue(ServoLightDownValue);
            digitalWrite(pinLightsPwm, LOW);
        }
    }
    else
    {
        digitalWrite(throttleValue > 0 ? pinMotorReverse : pinMotorForward, LOW);
        digitalWrite(throttleValue < 0 ? pinMotorReverse : pinMotorForward, HIGH);
        motorPwmValue = (uint8_t)(abs(throttleValue) / (float)INT16_MAX * 95.0 + 160.0);
        analogWrite(pinMotorPwm, motorPwmValue);
        lastStop = 0;
        servoLights.setValue(ServoLightUpValue);
        digitalWrite(pinLightsPwm, HIGH);
    }

    cycles++;
    if (false && cycles % 50 == 0) {
        Serial.printf("throttle vaue: %d motor power value: %d\n", throttleValue, motorPwmValue);
    }

    delay(20);
}
