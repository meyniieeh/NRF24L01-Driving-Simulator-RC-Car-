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
  bool Siren;
  
}


void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}
