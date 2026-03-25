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

Payload txData;
Payload rxData;

void setup() {
  Serial.begin(115200);

  SPI.begin(SCK_PIN, MISO_PIN, MOSI_PIN, CSN_PIN);

  radio.begin();
  radio.setDataRate(RF24_250KBPS);
  radio.setChannel(76);

  radio.openWritingPipe(address[1]);   // send to Node B
  radio.openReadingPipe(1, address[0]); // receive from Node B

  radio.stopListening();
}

void loop() {

  // Prepare dummy data
  txData.counter++;
  txData.value = txData.counter * 1.1;

  // Send
  bool ok = radio.write(&txData, sizeof(txData));

  if (ok) {
    Serial.print("Sent: ");
    Serial.print(txData.counter);
    Serial.print(" | ");

    // Switch to RX mode
    radio.startListening();
    delay(5);

    if (radio.available()) {
      radio.read(&rxData, sizeof(rxData));
      Serial.print("Reply: ");
      Serial.print(rxData.counter);
      Serial.print(", ");
      Serial.println(rxData.value);
    } else {
      Serial.println("No reply");
    }

    radio.stopListening();

  } else {
    Serial.println("Send failed");
  }

  delay(1000);
}