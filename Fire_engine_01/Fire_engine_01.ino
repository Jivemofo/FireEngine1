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
int CurrentFlashCount = 0;

//Headlights - non flashing

int ledHeadlight_Right = 0;  // Location of LED in strip1
int ledHeadlight_Left = 3; // Location of LED in strip1



/* Painful way of writing out each LED, I used a few array's instead. Leaving this in as a reference now

//Headlights

//Headlight Flashers - White
int ledHeadlightFlashW_Right = 1; //Location of LED in strip1
int ledHeadlightFlashW_Left = 2;  //Location of LED in Strip1

//Headlight Flashers - Red
int ledHeadlightFlashR_Right = 4; //Location of LED in strip1
int ledHeadlightFlashR_Left = 5: //Location of LED in strip1

//Light Bar Flashers locations- Left - strip 2

int ledLightbarFlashLeftYellow0 = 0;  //Treat as one Yellow Flasher
int ledLightbarFlashLeftYellow1 = 1;
int ledLightbarFlashLeftYellow2 = 2;

int ledLightbarFlashLeftRed3 = 3;  // Treat as one Red Flasher
int ledLightbarFlashLeftRed4 = 4;

int ledLightbarFlashLeftRed5 = 5; //Treat as one Red pulsing, always on
int ledLightbarFlashLeftRed6 = 6;

int ledLightbarFlashLeftRed7 = 7;  //treat as one Red Flasher
int ledLightbarFlashLeftRed8 = 8;


int ledLightbarFlashLeftRed9 = 9; //treat as one Red Flasher
int ledLightbarFlashLeftRed10 = 10;

int ledLightbarFlashLeftRed11 = 11; //treat as one Red Flasher
int ledLightbarFlashLeftRed12 = 12;



int ledLightbarMiddleYellow = 13; // Always on Running light


//light bar flashers locations - Right - strip 2



int ledLightbarFlashRightRed14 = 14;  //treat as one Red Flasher
int ledLightbarFlashRightRed15 = 15;

int ledLightbarFlashRightRed16 = 16;  //treat as one Red Flasher
int ledLightbarFlashRightRed17 = 17;

int ledLightbarFlashRightRed18 = 18;  //treat as one Red Flasher
int ledLightbarFlashRightRed19 = 19;

int ledLightbarFlashRightRed20 = 20;  //treat as one Red pulsing, always on
int ledLightbarFlashRightRed21 = 21;

int ledLightbarFlashRightRed22 = 22;  //treat as one Red Flasher
int ledLightbarFlashRightRed23 = 23;

int ledLightbarFlashRightYellow24 = 24;  //Treat as one Yellow Flasher
int ledLightbarFlashRightYellow25 = 25;
int ledLightbarFlashRightYellow26 = 26;

*/




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
const int Tpixel1 = 10;


void setup() {
  // declare the ledPin as an OUTPUT:
  Serial.begin(115200);
  Serial.println("Fire Engine Begins");
  pinMode(ledPin, OUTPUT);
  pinMode(switch1, INPUT_PULLUP);

  //create some arrays for the light bar
 int arrayLightbarPINS[Tpixel1]; // may be unnecessary as LED 0,1,2,3 will be used in other array's as sizeof().
 byte arrayLightbarState[Tpixel1]; //high or low depending on button state
 int arrayLightbarFlashCount[Tpixel1]; // will be used to figure out how many flashes have occured, ideally 4
 memset (arrayLightbarFlashCount, 0, sizeof arrayLightbarFlashCount); //fill FlashCount Array with intial val 0


  strip1.begin(); //intialize led strip
  strip1.show(); //write to strip, should be blank
}

void loop() {

  currentMillis = millis();  //capture the latest value of millis
  //this is the equivalent to noting the time from a clock
  //use the same time for all LED flashes to keep them synchronized
  updateLed_Headlight_State();
//update Lightbar states
//update headlight flashers states
//update tail light flasher states

  output_Led_headlights(); //fix to update all strips, not just headlights or add separate function

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




  // write to led
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
