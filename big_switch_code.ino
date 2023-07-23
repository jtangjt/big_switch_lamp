#include <FastLED.h>
#include <PinButton.h>
#include <MultiButton.h>

#define NUM_LEDS 24
#define LED_PIN 2
#define MAX_BRIGHTNESS 100
#define NUM_ANIMATIONS 3
// Animation 0 is white
// Animation 1 is rainbowspin
CRGB led[NUM_LEDS];
PinButton myButton(5);
int animationState = 0;
int ledsOn = 1;
bool buttonState = false;
bool currentButtonState = false;
unsigned long buttonPressStartTime = 0;
bool longPressActive = false;
void setup() {
  FastLED.addLeds<NEOPIXEL, LED_PIN>(led, NUM_LEDS);
  FastLED.setBrightness(MAX_BRIGHTNESS);
  for (int i = 0; i < NUM_LEDS; i++) {
    led[i] = CRGB(255, 255, 255);
  }
  FastLED.show();
//  Serial.begin(9600);
}

void toggle() {
  if (ledsOn == 1) {
    FastLED.setBrightness(0);
    FastLED.show();
    ledsOn = 0;
  } else if (ledsOn == 0) {
    FastLED.setBrightness(MAX_BRIGHTNESS);
    FastLED.show();
    ledsOn = 1;
  }
}

void rainbowSpin() {
  static  byte *c;
  static  uint16_t i, j;
  j = (j+1) % 255;
  i = (i+1) % NUM_LEDS;
  c = Wheel(((i * 256 / NUM_LEDS) + j) & 255);
  led[NUM_LEDS - 1 - i].setRGB(*c, *(c+1), *(c+2));
  FastLED.show();
}

void white(){
  for (int i = 0; i < NUM_LEDS; i++) {
    led[i] = CRGB(255, 255, 255);
  }
  FastLED.show();
}
  
void rainbowCycle() {
  static uint8_t cycleIndex = 0;
  uint8_t numSteps = 255; // You can adjust this value to control the smoothness of the color transition
  uint8_t speed = 1;     // You can adjust this value to control the speed of the color transition

  cycleIndex = (cycleIndex + speed) % 256; // Update the cycleIndex with the speed

  // Calculate the color step size based on the number of steps
  uint8_t stepSize = 256 / numSteps;

  // Set the entire LED array to the current color based on the cycleIndex
  for (int i = 0; i < NUM_LEDS; i++) {
    led[i].setHSV((cycleIndex + i * stepSize) & 255, 255, 255);
  }

  FastLED.show();
  delay(25);
}

void loop() {
  
  myButton.update();
  if (myButton.isClick()) {
    currentButtonState = true;
  }
  if (myButton.isReleased()) {
    currentButtonState = false;
  }

  if (currentButtonState != buttonState) {
    // Button state changed, check if it was pressed or released
    if (currentButtonState) {
      // Button was pressed
      buttonPressStartTime = millis();
      longPressActive = false;
    } else {
      // Button was released
      unsigned long pressDuration = millis() - buttonPressStartTime;

      if (pressDuration < 500) {
        // Short press: Toggle LED on/off
        toggle();
      } else {
        // Long press: Cycle through animations
        cycleAnimation();
      }
    }
    // Update the button state variable
    buttonState = currentButtonState;
  }

  // Choose the current animation
  switch (animationState) {
    case 0:
      white();
      break;
    case 1:
      rainbowSpin();
      break;
    case 2:
      rainbowCycle();
      break;
  }
}

void cycleAnimation() {
  animationState = (animationState + 1) % NUM_ANIMATIONS;
}

byte *Wheel(byte WheelPosition) {
  static byte c[3];
 
  if(WheelPosition < 85) {
   c[0] = WheelPosition * 3;
   c[1] = 255 - WheelPosition * 3;
   c[2] = 0;
  }
  else if(WheelPosition < 170) {
   WheelPosition -= 85;
   c[0] = 255 - WheelPosition * 3;
   c[1] = 0;
   c[2] = WheelPosition * 3;
  }
  else {
   WheelPosition -= 170;
   c[0] = 0;
   c[1] = WheelPosition * 3;
   c[2] = 255 - WheelPosition * 3;
  }

  return c;
}
