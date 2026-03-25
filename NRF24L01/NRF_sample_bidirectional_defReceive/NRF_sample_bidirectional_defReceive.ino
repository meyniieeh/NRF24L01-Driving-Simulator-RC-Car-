#include <SPI.h>
#include <RF24.h>

#define CE_PIN   9
#define CSN_PIN  10
#define SCK_PIN  12
#define MISO_PIN 13
#define MOSI_PIN 11

RF24 radio(CE_PIN, CSN_PIN);

uint8_t address[][6] = {"NODE1", "NODE2"};

struct Payload {
  int counter;
  float value;
};

Payload rxData;
Payload txData;

void setup() {
  Serial.begin(115200);

  SPI.begin(SCK_PIN, MISO_PIN, MOSI_PIN, CSN_PIN);

  radio.begin();
  radio.setDataRate(RF24_250KBPS);
  radio.setChannel(76);

  radio.openWritingPipe(address[0]);    // send back to Node A
  radio.openReadingPipe(1, address[1]); // receive from Node A

  radio.startListening();
}

void loop() {

  if (radio.available()) {

    radio.read(&rxData, sizeof(rxData));

    Serial.print("Received: ");
    Serial.print(rxData.counter);
    Serial.print(", ");
    Serial.println(rxData.value);

    // Create reply (dummy modification)
    txData.counter = rxData.counter + 100;
    txData.value = rxData.value + 0.5;

    // Send back
    radio.stopListening();
    radio.write(&txData, sizeof(txData));
    radio.startListening();
  }
}