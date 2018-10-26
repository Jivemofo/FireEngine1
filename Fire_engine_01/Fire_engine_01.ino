// Fire Engine 1
//========================================

// ----------LIBRARIES--------------

#include <Adafruit_NeoPixel.h>
#include <TrueRandom.h> //Uses Analog 0

//----------Definitions---------

#define PIN_A 2
#define PIN_B 3
#define PIN_C 4
#define PIN_D 5
#define TPIXEL1 2 //The total amount of pixel's/led's in your connected strip/stick (Default is 60)
#define TPIXEL2 4 //The total amount of pixel's/led's in your connected strip/stick (Default is 60)
#define TPIXEL3 6 //The total amount of pixel's/led's in your connected strip/stick (Default is 60)
#define TPIXEL4 27 //The total amount of pixel's/led's in your connected strip/stick (Default is 60)
#define BUTTONPIN_A 2 // the number of the pushbutton pin
#define BUTTONPIN_B  // the number of the pushbutton pin
#define PotPIN_A  // number of the potentiometer for headlight Brightness
#define PotPIN_B // number of the potentiometer for flasher Brightness


//-------Variables

int PotVAL_A = 0; //Sets an intial value for potentiometer
int PotVAL_B = 0; //Sets an intial value for potentiometer

// --------CONSTANTS (won't change)---------------

//const int onBoardLedPin =  13;      // the pin numbers for the LEDs
//const int led_A_Pin = 12;
//const int led_B_Pin = 11;
//const int buttonLed_Pin = 10;

//const int buttonPin = 7; // the pin number for the button

//const int servoPin = 5; // the pin number for the servo signal

const int led_slow_interval = 5000; // number of millisecs between blinks
const int led_Flash_A_Interval = 200;
const int led_Flash_B_Interval = 200;
const int led_fast_interval = 50;

const int blinkDuration = 500; // number of millisecs that Led's are on - all three leds use this
const int blinkFastDuration = blinkDuration/4;

const int buttonInterval = 300; // number of millisecs between button readings

//const int servoMinDegrees = 20; // the limits to servo movement
//const int servoMaxDegrees = 150;


//------------ VARIABLES (will change)---------------------

byte led_Headlight_State = LOW;             // used to record whether the LEDs are on or off
byte flasher_A_State = LOW;           //   LOW = off
byte flasher_B_State = LOW;
byte buttonLed_State = LOW;

//Servo myservo;  // create servo object to control a servo

//int servoPosition = 90;     // the current angle of the servo - starting at 90.
//int servoSlowInterval = 80; // millisecs between servo moves
//int servoFastInterval = 10;
//int servoInterval = servoSlowInterval; // initial millisecs between servo moves
//int servoDegrees = 2;       // amount servo moves at each step
//    will be changed to negative value for movement in the other direction

unsigned long currentMillis = 0;    // stores the value of millis() in each iteration of loop()
unsigned long previousHeadlightMillis = 0;   // will store last time the LED was updated
unsigned long previousLed_A_Millis = 0;
unsigned long previousLed_B_Millis = 0;

unsigned long previousButtonMillis = 0; // time when button press last checked

//unsigned long previousServoMillis = 0; // the time when the servo was last moved

Adafruit_NeoPixel strip1 = Adafruit_NeoPixel(TPIXEL1, PIN_A, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip2 = Adafruit_NeoPixel(TPIXEL2, PIN_B, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip3 = Adafruit_NeoPixel(TPIXEL3, PIN_C, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip4 = Adafruit_NeoPixel(TPIXEL4, PIN_D, NEO_GRB + NEO_KHZ800);

//========================================

void setup() {

    Serial.begin(9600);
    Serial.println("Fire Engine Sketch Starting");  // so we know what sketch is running

    // set the Led pins as output:
    //pinMode(onBoardLedPin, OUTPUT);
    pinMode(PIN_A, OUTPUT);
    pinMode(PIN_B, OUTPUT);
    pinMode(PIN_C, OUTPUT);
    pinMode(PIN_D, OUTPUT);

    // set the button pin as input with a pullup resistor to ensure it defaults to HIGH
    pinMode(BUTTONPIN_A, INPUT_PULLUP);
    pinMode(BUTTONPIN_B, INPUT_PULLUP);

    //myservo.write(servoPosition); // sets the initial position
    //myservo.attach(servoPin);

}

//========================================

void loop() {

    // Notice that none of the action happens in loop() apart from reading millis()
    //   it just calls the functions that have the action code

    currentMillis = millis();   // capture the latest value of millis()
    //   this is equivalent to noting the time from a clock
    //   use the same time for all LED flashes to keep them synchronized
    readPots();
    readButton();               // call the functions that do the work
    updateled_Headlight_State();
    updateflasher_A_State();
    updateflasher_B_State();
    writetoLeds();
    switchLeds();  //writes to led's, questionable

}

//========================================

void updateled_Headlight_State() {

    if (led_Headlight_State == LOW) {
        // if the Led is off, we must wait for the interval to expire before turning it on
        if (currentMillis - previousHeadlightMillis >= led_slow_interval) {
            // time is up, so change the state to HIGH
            led_Headlight_State = HIGH;
            // and save the time when we made the change
            previousHeadlightMillis += led_slow_interval;
            // NOTE: The previous line could alternatively be
            //              previousHeadlightMillis = currentMillis
            //        which is the style used in the BlinkWithoutDelay example sketch
            //        Adding on the interval is a better way to ensure that succesive periods are identical

        }
    }
    else {  // i.e. if led_Headlight_State is HIGH

        // if the Led is on, we must wait for the duration to expire before turning it off
        if (currentMillis - previousHeadlightMillis >= blinkDuration) {
            // time is up, so change the state to LOW
            led_Headlight_State = LOW;
            // and save the time when we made the change
            previousHeadlightMillis += blinkDuration;
        }
    }
}

//========================================

void updateflasher_A_State() {

    if (flasher_A_State == LOW) {
        if (currentMillis - previousLed_A_Millis >= led_Flash_A_Interval) {
            flasher_A_State = HIGH;
            previousLed_A_Millis += led_Flash_A_Interval;
        }
    }
    else {
        if (currentMillis - previousLed_A_Millis >= blinkDuration) {
            flasher_A_State = LOW;
            previousLed_A_Millis += blinkDuration;
        }
    }
}

//========================================

void updateflasher_B_State() {

    if (flasher_B_State == LOW) {
        if (currentMillis - previousLed_B_Millis >= led_Flash_B_Interval) {
            flasher_B_State = HIGH;
            previousLed_B_Millis += led_Flash_B_Interval;
        }
    }
    else {
        if (currentMillis - previousLed_B_Millis >= blinkDuration) {
            flasher_B_State = LOW;
            previousLed_B_Millis += blinkDuration;
        }
    }
}

//========================================


//This part needs fixing, should be strip.show for all strips
void switchLeds() {
    // this is the code that actually switches the LEDs on and off

    digitalWrite(onBoardLedPin, led_Headlight_State);
    digitalWrite(led_A_Pin, flasher_A_State);
    digitalWrite(led_B_Pin, flasher_B_State);
    digitalWrite(buttonLed_Pin, buttonLed_State);
}

//========================================

void writetoLeds(){
    //Updates each strip
    //Check LED state for each pixel
    for (int i = 0; i< strip.numPixels()< 7 ;i++) {

    }

}



//========================================


//Need to read headlight button and flashers button

void readButton() {

    // this only reads the button state after the button interval has elapsed
    //  this avoids multiple flashes if the button bounces
    // every time the button is pressed it changes buttonLed_State causing the Led to go on or off
    // Notice that there is no need to synchronize this use of millis() with the flashing Leds

    if (millis() - previousButtonMillis >= buttonInterval) {

        if (digitalRead(buttonPin) == LOW) {
            buttonLed_State = ! buttonLed_State; // this changes it to LOW if it was HIGH
            //   and to HIGH if it was LOW
            previousButtonMillis += buttonInterval;
        }
    }

}

//======================================

void readPots(){   //Read Both potentiometers and Set Brightness

    // this only reads the button state after the button interval has elapsed
    //  this avoids multiple flashes if the button bounces
    // every time the button is pressed it changes buttonLed_State causing the Led to go on or off
    // Notice that there is no need to synchronize this use of millis() with the flashing Leds

    if (millis() - previousButtonMillis >= buttonInterval) {

        PotVal_A = analogRead PotPIN_A;
        PotVal_A = map(PotVal_A, 0, 1023, 0, 255);
        strip1.setBrightness(PotVal_A);
        PotVal_B = analogRead PotPIN_B;
        PotVal_B = map(PotVal_B, 0, 1023, 0, 255);
        strip2.setBrightness(PotVal_B);

    }

}
