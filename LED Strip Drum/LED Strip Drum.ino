//Including the libraries needed to run this sketch.
#include <PololuLedStrip.h>
#include <Adafruit_NeoPixel.h>
#include "WS2812_Definitions.h"

//Definitions
#define PIN 4
#define LED_COUNT 300

bool lights[LED_COUNT];
int tolerance = 50;
bool touchReset = true;

unsigned long colour[32] = {BLUE, GREEN, TEAL, SPRINGGREEN, LIME, AQUA, CYAN, SEAGREEN, LIMEGREEN, TURQUOISE, 
                          ROYALBLUE, INDIGO, PURPLE, SKYBLUE, MEDIUMPURPLE, YELLOWGREEN, CRIMSON, LAVENDER,
                          VIOLET, AZURE, SALMON, RED, MAGENTA, FUCHSIA, ORANGERED, HOTPINK, CORAL, ORANGE,
                          PINK, GOLD, YELLOW, LEMONCHIFFON};

//Declaration of variable used to sense state of button.
int sensorValue=0;

// Create an instance of the Adafruit_NeoPixel class called "leds".
// That'll be what we refer to from here on.
Adafruit_NeoPixel leds = Adafruit_NeoPixel(LED_COUNT, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
 //Configure pin 13 as OUTPUT and pin 7 as INPUT.
  pinMode(13, OUTPUT);
  digitalWrite(13,HIGH);
  
  Serial.begin(9600);
  
  for (int i = 0; i < LED_COUNT; i++) {
    lights[i] = false;
  }
  
  leds.begin();  // Call this to start up the LED strip.
  clearLEDs();   // This function, defined below, turns all LEDs off
  leds.show();   // but the LEDs don't actually update until you call this.
  Serial.println("Finished Setup");
}

void loop() {
  // put your main code here, to run repeatedly:
  sensorValue=analogRead(A1);          //Read in button state into variable.
  if (sensorValue != 0) { 
    Serial.println(sensorValue);
  }
  clearLEDs(); 
  
  if(sensorValue>=tolerance) {//If the button was one AND the counter is divisible by ten, run the code below.
    lights [0] = true;
    touchReset = false;
  }

  for (int i = LED_COUNT; i > -1; i--)
  {
    if (lights[i]){
      leds.setPixelColor(i, CYAN);
      lights [i] = false;
      if (i != LED_COUNT)
        lights [i+1] = true;
    }
  }

  leds.show();
  delay(10);  
}

void clearLEDs()
{
  //Cycle through all LEDs and turn each one of them to off individually.
  for (int i=0; i<LED_COUNT; i++)
  {
    leds.setPixelColor(i, 0);
  }
}
