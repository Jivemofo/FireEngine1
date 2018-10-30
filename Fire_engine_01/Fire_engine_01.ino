//Headlights work off of a state machine, and update according to a switch position

#include <Adafruit_NeoPixel.h>

#define switch1 2
int maxBrightness = 255;
int minBrightness = 0;
int ledPin = 17;      // select the pin for the LED
int redLedMax = maxBrightness;
int redLedMin = minBrightness;
int greenLedMax = maxBrightness;
int greenLedMin = minBrightness;
int blueLedMax = maxBrightness;
int blueLedMin = minBrightness;
int potVal1 = 255;  // placeholder value until pot function works

//Headlights

int ledHeadlight_Right = 0;  // Location of LED in strip1
int ledHeadlight_Left = 2; // Location of LED in strip1




byte switchState1 = LOW;

Adafruit_NeoPixel strip1 = Adafruit_NeoPixel(10, ledPin, NEO_GRB + NEO_KHZ800);



// ------------------Variables

unsigned long currentMillis = 0;    // stores the value of millis() in each iteration of loop()
unsigned long previousOnBoardLedMillis = 0;   // will store last time the LED was updated
unsigned long previousLed_A_Millis = 0;
unsigned long previousLed_B_Millis = 0;
unsigned long previousButtonMillis = 0; // time when button press last checked



//-------------------Constants

const int led_Flash_C_Interval = 500; // number of millisecs between blinks
const int led_Flash_A_Interval = 2500;
const int led_Flash_B_Interval = 4500;
const int led_fast_interval = 50;

const int blinkDuration = 500; // number of millisecs that Led's are on - all three leds use this
const int blinkFastDuration = 50;

const int buttonInterval = 50; // number of millisecs between button readings


void setup() {
  // declare the ledPin as an OUTPUT:
  Serial.begin(115200);
  Serial.println("Fire Engine Begins");
  pinMode(ledPin, OUTPUT);
  pinMode(switch1, INPUT_PULLUP);
  strip1.begin();
  strip1.show();
}

void loop() {

  currentMillis = millis();  //capture the latest value of millis
  //this is the equivalent to noting the time from a clock
  //use the same time for all LED flashes to keep them synchronized

  updateLed_Headlight_State();
  output_Led_headlights();

}

  void updateLed_Headlight_State(){
// this only reads the button state after the button interval has elapsed
    //  this avoids multiple flashes if the button bounces
    // every time the button is pressed it changes buttonLed_State causing the Led to go on or off
    // Notice that there is no need to synchronize this use of millis() with the flashing Leds

    if (millis() - previousButtonMillis >= buttonInterval) {

        switchState1 = digitalRead(switch1);
        previousButtonMillis += buttonInterval;
        }

}




  // write to led?
 void output_Led_headlights() {
  if (switchState1 == HIGH) {
  both_headlights();
  strip1.show();
  }
  else if (switchState1 == LOW) {
    strip1.setPixelColor (ledHeadlight_Right,0,0,0);
    strip1.setPixelColor (ledHeadlight_Left,0,0,0);
    strip1.show();

  }
  }

  void both_headlights() {
    strip1.setPixelColor (ledHeadlight_Right,potVal1,potVal1,potVal1);
    strip1.setPixelColor (ledHeadlight_Left,potVal1,potVal1,potVal1);
  }
