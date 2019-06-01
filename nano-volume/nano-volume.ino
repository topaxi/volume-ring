#include <Adafruit_NeoPixel.h>

#define IDLE_DELAY   30
#define WIRE_ADDRESS  8
#define NEO_PIN       6
#define NUMPIXELS    12

Adafruit_NeoPixel pixels(NUMPIXELS, NEO_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  pixels.begin();
  Serial.begin(9600);
}

void loop() {
  if (!Serial.available()) {
    delay(IDLE_DELAY);
    return;
  }

  uint8_t volume = Serial.read();

  Serial.print("I received: ");
  Serial.println(volume, DEC);

  uint8_t px = NUMPIXELS / 255.0 * volume;

  pixels.clear();

  for (uint8_t i = 0; i < NUMPIXELS; i++) {
    if (px > i) {
      pixels.setPixelColor(i, pixels.Color(8, 24, 20));
    } else {
      pixels.setPixelColor(i, pixels.Color(0, 0, 0));
    }
  }

  pixels.show();
}
