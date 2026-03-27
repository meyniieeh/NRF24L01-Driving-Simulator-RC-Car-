#include <Servo.h>

static const int servoPin = 5;
const int frequency = 200; // Hz

Servo servo1;

void setup() {
    Serial.begin(115200);
    servo1.attach(
    servoPin, 
    Servo::CHANNEL_NOT_ATTACHED, 
    Servo::DEFAULT_MIN_ANGLE, 
    Servo::DEFAULT_MAX_ANGLE, 
    Servo::DEFAULT_MIN_PULSE_WIDTH_US, 
    Servo::DEFAULT_MAX_PULSE_WIDTH_US, 
    frequency
);
}

void loop() {
  servo1.write(180);
  delay(1000);
  servo1.write(0);
  delay(1000);
}