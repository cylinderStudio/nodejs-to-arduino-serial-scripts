#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define PIN 6
#define PIXELS 384

String inData;
boolean running = false;

/*
Parameter 1 = number of pixels in strip
Parameter 2 = Arduino pin number (most are valid)
Parameter 3 = pixel type flags, add together as needed:
    NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
    NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
    NEO_GRB     Pixels are wiRED for GRB bitstream (most NeoPixel products)
    NEO_RGB     Pixels are wiRED for RGB bitstream (v1 FLORA pixels, not v2)
*/

void setup() {
  Serial.begin(2000000);
  // Serial.println("Waiting for NodeJS to send a signal...\n");

  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

void loop() {
  while (Serial.available()) {
    // incoming = Serial.readString();

    char received = Serial.read();
    inData += received;

    // Process message when new line character is received
    if (received == '\n') {
      Serial.println("Arduino Received: ");
      Serial.println(inData);

      inData = ""; // Clear received buffer

      if (running) {
        running = false;
      } else {
        running = true;
      }
    }
  }

  if (running) {
    // theaterChase(strip.Color(127, 127, 127), 50); // White
    strip.setPixelColor(47, 127, 127, 127);
    strip.show();
  } else {
    // colorWipe(strip.Color(255, 0, 0), 50); // Red
    strip.setPixelColor(47, 255, 0, 0);
    strip.show();
  }
  
  /* if (incoming == "Red") {
    colorWipe(strip.Color(255, 0, 0), 50); // Red
  } else if (incoming == "Blue") {
    theaterChase(strip.Color(127, 127, 127), 50); // White
  } */
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, c);    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

