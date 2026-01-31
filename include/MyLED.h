#include <Adafruit_NeoPixel.h>

Adafruit_NeoPixel strip(60, 48, NEO_GRBW + NEO_KHZ800);

void colorWipe(uint32_t color, int wait) {
  strip.clear();
  for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...
    strip.setPixelColor(i, color);         //  Set pixel's color (in RAM)
    strip.show();                          //  Update strip to match
    delay(wait);                           //  Pause for a moment
  }
}

void _initRGB(){
    strip.begin();
    strip.show();
    strip.setBrightness(10);
}

// Fill along the length of the strip in various colors...
// colorWipe(strip.Color(255,   0,   0)     , 50); // Red
// colorWipe(strip.Color(  0, 255,   0)     , 50); // Green
// colorWipe(strip.Color(  0,   0, 255)     , 50); // Blue