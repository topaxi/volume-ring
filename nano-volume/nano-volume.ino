#include <Adafruit_NeoPixel.h>

#define IDLE_DELAY   30
#define WIRE_ADDRESS  8
#define NEO_PIN       6
#define NUMPIXELS    12
#define FIVE_SECONDS_MS 5000

Adafruit_NeoPixel pixels(NUMPIXELS, NEO_PIN, NEO_GRB + NEO_KHZ800);

uint8_t currentVolume = 0;
uint16_t timeSinceChange = 0;
uint8_t ringIsOff = 0;

void setup() {
  pixels.begin();
  Serial.begin(9600);
}

void loop() {
  if (!Serial.available()) {
    if (!ringIsOff && timeSinceChange >= FIVE_SECONDS_MS) {
      fadeOut(8, 24, 20);
      ringIsOff = 1;
    } else {
      timeSinceChange += IDLE_DELAY;
    }

    delay(IDLE_DELAY);
    return;
  }

  uint8_t volume = Serial.read();

  Serial.print("I received: ");
  Serial.println(volume, DEC);

  currentVolume = pixels.numPixels() / 255.0 * volume;

  renderPixels(8, 24, 20);
  timeSinceChange = 0;
  ringIsOff = 0;
}

void renderPixels(uint8_t r, uint8_t g, uint8_t b) {
  pixels.clear();

  for (uint8_t i = 0; i < pixels.numPixels(); i++) {
    if (currentVolume > i) {
      pixels.setPixelColor(i, pixels.Color(r, g, b));
    } else {
      pixels.setPixelColor(i, pixels.Color(0, 0, 0));
    }
  }

  pixels.show();
}

// This could probably made a bit smoother with each color
// having their own relative step
void fadeOut(uint8_t r, uint8_t g, uint8_t b) {
  for(;r > 0 || g > 0 || b > 0;) {
    if (r > 0) r--;
    if (g > 0) g--;
    if (b > 0) b--;

    renderPixels(r, g, b);

    delay(64);
  }
}
