#include <SPI.h>
#include <RF24.h>

#define CE_PIN 9
#define CSN_PIN 10
#define SCK_PIN 12
#define MISO_PIN 13
#define MOSI_PIN 11
#define POT_PIN 1

#define JOY_Y_PIN 2
#define JOY_SW 3
#define LED_PIN 14

//servo
int center = 2048;
int deadband = 100;  // 200 total range

bool pressed = 1;

RF24 radio(CE_PIN, CSN_PIN);
uint8_t address[][6] = { "NODE1", "NODE2" };


struct Payload {
  int steer;  // Only sending steering data
  int speed;
  int dir;
  bool button;
};


Payload txData;

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  pinMode(JOY_SW, INPUT_PULLUP);
  SPI.begin(SCK_PIN, MISO_PIN, MOSI_PIN, CSN_PIN);


  if (!radio.begin()) {
    Serial.println("NRF ERROR: Check wiring!");
    while (1)
      ;
  }

  radio.setDataRate(RF24_250KBPS);
  radio.setChannel(76);

  radio.openWritingPipe(address[1]);

  radio.stopListening();
  Serial.println("Transmitter Ready - Servo Test Mode");
}






void loop() {

  int joyY = analogRead(JOY_Y_PIN);
  pressed = digitalRead(JOY_SW);
  int potVal = analogRead(POT_PIN);

  // Map 0-4095 (ESP32 ADC) to 0-180 (Servo Degrees)
  if (potVal > center + deadband) {
    txData.steer = map(potVal, center + deadband, 4095, 90, 180);
  } else if (potVal < center - deadband) {
    txData.steer = map(potVal, 0, center - deadband, 0, 90);
  } else {
    txData.steer = 90;  // dead zone (no movement)
  }

  // 3. Throttle Logic
  if (joyY > 2200) {
    txData.speed = map(joyY, 2200, 4095, 0, 255);
    txData.dir = 1;  // Forward
  } else if (joyY < 1800) {
    txData.speed = map(joyY, 1800, 0, 0, 255);
    txData.dir = 2;  // Backward
  } else {
    txData.speed = 0;
    txData.dir = 0;  // Idle
  }

  if (!pressed == 1) {
    txData.button = !txData.button;
    delay(10);
  }

  Serial.println(txData.button);

  bool ok = radio.write(&txData, sizeof(txData));

  if (ok) {
    Serial.print("Sent Steer Angle: ");
    Serial.println(txData.steer);
  } else {
    Serial.println("Send Failed: Receiver not active?");
  }
}