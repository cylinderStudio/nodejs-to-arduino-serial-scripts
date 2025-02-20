#include <math.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define PIN 6
#define PIXELS 60 // 120 

/*
Parameter 1 = number of pixels in strip
Parameter 2 = Arduino pin number (most are valid)
Parameter 3 = pixel type flags, add together as needed:
    NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
    NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
    NEO_GRB     Pixels are wiRED for GRB bitstream (most NeoPixel products)
    NEO_RGB     Pixels are wiRED for RGB bitstream (v1 FLORA pixels, not v2)
*/

Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXELS, PIN, NEO_GRB + NEO_KHZ800);

const uint32_t BLACK = strip.Color(0,0,0);
const uint32_t RED = strip.Color(255,0,0);
const uint32_t GREEN = strip.Color(0,255,0);
const uint32_t BLUE = strip.Color(0,0,255);
const uint32_t WHITE = strip.Color(255,255,255);

const uint16_t BLINK_INTERVAL = 650;
const uint8_t BRIGHTNESS_LOWER = 25;
const uint8_t BRIGHTNESS_STEPS = 10;
const uint16_t BRIGHTNESS_INTERVAL = 25;
const uint8_t TRACER_PADDING = 2;
const uint8_t TRACER_WIDTH = (TRACER_PADDING * 2) + 1;
uint16_t TRACER_INTERVAL = 15;

char received;
uint32_t current_color = BLACK;
uint32_t blink_color = BLACK;
String animate = "solid";
uint64_t currentMillis;
uint64_t previousMillis = 0;
uint8_t brightness = 255;
uint8_t brightness_upper = brightness;
uint8_t mult;
uint32_t dimmed_color;
uint8_t tracer_pixel = 0;

void setup() {
  Serial.begin(57600);
  // Serial.println("Waiting for NodeJS to send a signal...\n");

  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

void loop() { 
  if (Serial.available()) { 
    received = Serial.read(); 

    switch (received) {
      case 's':
        animate = "solid";
        break;
      case 'a':
        animate = "alternate";
        break;
      case 'p':
        animate = "pulse";
        break;
      case 't':
        animate = "tracer";
        tracer_pixel = 0;
        break;
      case 'r':
        current_color = RED;
        break;
      case 'g': 
        current_color = GREEN;
        break;
      case 'b':
        current_color = BLUE;
        break;
      case 'o':
        current_color = BLACK;
        break;
      case 'w':
        current_color = WHITE;
        break;
      case '1':
        brightness = 75;
        break;
      case '2':
        brightness = 150;
        break;
      case '3':
        brightness = 255;
        break;
      case ' ':
        strip.setBrightness(brightness);
        brightness_upper = brightness;
        break; 
    }
  }

  doLights(animate);
}

void doLights(String anim) {
  if (anim == "solid") {
      setColor(current_color);
      strip.show();
      delay(100);
  } else if (anim == "alternate") {    // alternate between two colors to simulate blinking
      currentMillis = millis();
      
      if (currentMillis - previousMillis > BLINK_INTERVAL) {
        previousMillis = currentMillis;

        if (blink_color == BLACK) {
          blink_color = current_color;
        } else {
          blink_color = BLACK;
        }
        
        setColor(blink_color);
        strip.show();
      }
  } else if (anim == "pulse") { 
      currentMillis = millis();
      
      if (currentMillis - previousMillis > BRIGHTNESS_INTERVAL) {
        previousMillis = currentMillis;

        if (brightness >= brightness_upper) {mult = -1;}
        else if (brightness <= BRIGHTNESS_LOWER) {mult = 1;}
        
        brightness = brightness + (mult * (round((brightness_upper - BRIGHTNESS_LOWER) / BRIGHTNESS_STEPS)));
        setColor(current_color);
        strip.setBrightness(brightness);
        strip.show();
      }
  } else if (anim == "tracer") {
    currentMillis = millis();
      
    if (currentMillis - previousMillis > TRACER_INTERVAL) {
      previousMillis = currentMillis;

      setColor(current_color);
      dimmed_color = DimColor(current_color);

      if (tracer_pixel + TRACER_WIDTH >= PIXELS) { mult = -1; }
      else if (tracer_pixel <= 0) { mult = 1; }
      
      for (uint8_t i=tracer_pixel; i < tracer_pixel + TRACER_WIDTH; i++) {
        strip.setPixelColor(i,dimmed_color);
      }
      
      strip.show();
      tracer_pixel = tracer_pixel + (1 * mult);
    }
  }
}

// HELPERS

void setColor(uint32_t color) {
  for (int i=0; i <= PIXELS; i++) {
    strip.setPixelColor(i,color);
  }
}

uint8_t Red(uint32_t color) {
  return (color >> 16) & 0xFF;
}

// Returns the Green component of a 32-bit color
uint8_t Green(uint32_t color) {
  return (color >> 8) & 0xFF;
}

// Returns the Blue component of a 32-bit color
uint8_t Blue(uint32_t color) {
  return color & 0xFF;
}

uint32_t DimColor(uint32_t color) {
  uint32_t dimColor = strip.Color(Red(color) >> 3, Green(color) >> 3, Blue(color) >> 3);
  return dimColor;
}

