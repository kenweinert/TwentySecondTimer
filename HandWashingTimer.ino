

#define FASTLED_ESP8266_D1_PIN_ORDER
#include <FastLED.h>

// definitions for the WS2812 color wheel
#define LED_PIN     5
#define NUM_LEDS    12
#define BRIGHTNESS  32
#define LED_TYPE    WS2812
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];

CRGBPalette16 redPalette;
CRGBPalette16 greenPalette;
CRGBPalette16 blackPalette;
CRGBPalette16 currentPalette;

TBlendType    currentBlending;

#define UPDATES_PER_SECOND 100

// info for the ultrasound trigger
#define TRIGGER 3
#define ECHO    4

long duration;
int distance;


// trying a switch this time
#define SWITCH  4

int buttonState = LOW;


void setup() {
  // color setup
  delay( 3000 ); // power-up safety delay
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(  BRIGHTNESS );

  fill_solid(blackPalette, 16, CRGB::Black);
  fill_solid(redPalette, 16, CRGB::Red);
  fill_solid(greenPalette, 16, CRGB::Green);

  currentPalette = blackPalette;
  fillLEDsFromPaletteColors(0);
  
  currentBlending = LINEARBLEND;
/*
  // trigger setup
  pinMode(TRIGGER, OUTPUT);
  pinMode(ECHO, INPUT);
  Serial.begin(9600);
*/  

  pinMode(SWITCH, INPUT);
}

void loop()
{
  currentPalette = blackPalette;
  fillLEDsFromPaletteColors(0);
  FastLED.show();

  /*
  digitalWrite(TRIGGER, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGGER, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER, LOW);

  duration = pulseIn(ECHO, HIGH);

  distance = (duration * 0.0343) / 2;

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  delay(100);

  if (distance == 0) {
    runTimer();
  }
  */

  buttonState = digitalRead(SWITCH);

  if (buttonState == HIGH) {
    buttonState = LOW;
    runTimer();
  } else {
    delay(1000);
  }
}

void runTimer()
{
  currentPalette = blackPalette;
  fillLEDsFromPaletteColors(0);
  FastLED.show();
  FastLED.delay(1000);
  

  for(int i = 1; i < 11; i++) {
    if (i%2) {
      leds[0] = CRGB::Black;
      leds[11] = CRGB::Blue;
    } else {
      leds[0] = CRGB::Blue;
      leds[11] = CRGB::Black;
    }
    
    leds[i] = redPalette[i];
    FastLED.show();
    FastLED.delay(1000);
  }

  for (int i = 10; i > 0; i--) {
    if (i%2) {
      leds[0] = CRGB::Blue;
      leds[11] = CRGB::Black;
    } else {
      leds[0] = CRGB::Black;
      leds[11] = CRGB::Blue;
    }

    leds[i] = greenPalette[i];
    FastLED.show();
    FastLED.delay(1000);
  }

    setupStripedPalette();
    currentBlending = LINEARBLEND;

  for (int i = 0; i < 500; i++) {
    //setupRandomPalette();
    static uint8_t startIndex = 0;
    fillLEDsFromPaletteColors(startIndex);
    startIndex++;
    
    FastLED.show();
    FastLED.delay(1000 / UPDATES_PER_SECOND);
  }

}

void setupStripedPalette()
{
  int choice = random8() % 3;

  switch (choice) {
    case 0: fill_solid( currentPalette, 16, CRGB::Red); break;
    case 1: fill_solid( currentPalette, 16, CRGB::Blue); break;
    case 2: fill_solid( currentPalette, 16, CRGB::Green); break;
    
  }
    // 'black out' all 16 palette entries...
    
    // and set every fourth one to white.
    currentPalette[0] = CRGB::White;
    currentPalette[4] = CRGB::White;
    currentPalette[8] = CRGB::White;
    currentPalette[12] = CRGB::White;
    
}


void fillLEDsFromPaletteColors( uint8_t colorIndex)
{
    uint8_t brightness = 255;
    
    for( int i = 0; i < NUM_LEDS; i++) {
        leds[i] = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending);
        colorIndex += 3;
    }
}

// This function fills the palette with totally random colors.
CHSV randomColor()
{
  return CHSV( random8(), 255, random8());
}

void setupRandomPalette()
{
  for (int i = 0; i < 12; i++) {
    currentPalette[i] = CHSV( random8(), random8(), random8());
  }
}

// This example shows how to set up a static color palette
// which is stored in PROGMEM (flash), which is almost always more
// plentiful than RAM.  A static PROGMEM palette like this
// takes up 64 bytes of flash.
const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM =
{
    CRGB::Red,
    CRGB::Gray, // 'white' is too bright compared to red and blue
    CRGB::Blue,
    CRGB::Black,
    
    CRGB::Red,
    CRGB::Gray,
    CRGB::Blue,
    CRGB::Black,
    
    CRGB::Red,
    CRGB::Red,
    CRGB::Gray,
    CRGB::Gray,
    CRGB::Blue,
    CRGB::Blue,
    CRGB::Black,
    CRGB::Black
};
