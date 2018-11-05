//Headlights work off of a state machine, and update according to a switch position

#include <Adafruit_NeoPixel.h>

#define switch1 A4 //headlight switch pin location
#define switch2 A3 //flasher switch pin location
#define arrLength(x) (sizeof(x) / sizeof((x)[0]))  //macro for determining array length

int maxBrightness = 255;
int minBrightness = 0;
int ledPin2 = 2;      // select the pin for the Headlight LED
int ledPin3 = 3;      // select the pin for the tailight LED
int ledPin5 = 5;      // select the pin for the LightBar LED
int redLedMax = maxBrightness;
int redLedMin = minBrightness;
int greenLedMax = maxBrightness;
int greenLedMin = minBrightness;
int blueLedMax = maxBrightness;
int blueLedMin = minBrightness;
int potVal1 = 255;  // placeholder value until pot function works
int CurrentFlashCount = 0;

//Headlights - non flashing

int ledHeadlight_Right = 6;  // Location of LED in strip1
int ledHeadlight_Left = 0; // Location of LED in strip1
int ledTaillight_Left = 0; //location of LED in strip2
int ledTaillight_Right = 5; //location of led in strip2


byte switchState1 = LOW;
byte switchState2 = LOW;
byte isLeftON = true;
byte isRightON = false;
// ------------------Variables

unsigned long currentMillis = 0;    // stores the value of millis() in each iteration of loop()
// unsigned long previousOnBoardLedMillis = 0;   // will store last time the LED was updated
unsigned long previousLed_A_Millis = 0;
unsigned long previousLed_B_Millis = 0;
unsigned long previous_led_fast_Millis = 0;
unsigned long previousButtonMillis = 0; // time when button press last checked




//-------------------Constants

const int led_Flash_C_Interval = 500; // number of millisecs between blinks
const int led_Flash_A_Interval = 1000;
const int led_Flash_B_Interval = 4500;
const int led_fast_interval = 50;

const int blinkDuration = 500; // number of millisecs that Led's are on - all three leds use this
const int blinkFastDuration = 50;

const int buttonInterval = 100; // number of millisecs between button readings
const int Tpixel1 = 8; // number of led's in Headlight assembly
const int Tpixel2 = 6;  //number of led's in tailight assembly
const int Tpixel3 = 27;  // number of led's in Lightbar



Adafruit_NeoPixel strip1 = Adafruit_NeoPixel(Tpixel1, ledPin2, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip2 = Adafruit_NeoPixel(Tpixel2, ledPin3, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip3 = Adafruit_NeoPixel(Tpixel3, ledPin5, NEO_GRB + NEO_KHZ800);



void setup() {
  // declare the ledPin as an OUTPUT:
  Serial.begin(115200);
  Serial.println("Fire Engine Begins");
  pinMode(ledPin2, OUTPUT);
  pinMode(ledPin3, OUTPUT);
  pinMode(ledPin5, OUTPUT);
  pinMode(switch1, INPUT_PULLUP);
  pinMode(switch2, INPUT_PULLUP);

  //create some arrays for the light bar
 int arrayLightbarLeft[] = {3,4,7,8,9,10,11,12}; //specific pin locations, but it should be modelo return of the pixel strip (of a possible 12 light positions)..
 int arrayLightbarRight[] = {14,15,16,17,18,19,22,23};
 int arrayLightbarLeftYellow[] = {0,1,2};
 int arrayLightbarRightYellow[] = {24,25,26};
 byte arrayLightbarState[Tpixel3]; //high or low depending on button state
 byte arrayLightbarHasFlashed[Tpixel3]; //to show if all flashes are completed
 int arrayLightbarFlashCount[Tpixel3]; // will be used to figure out how many flashes have occured, ideally 4
 memset (arrayLightbarFlashCount, 0, sizeof arrayLightbarFlashCount); //fill FlashCount Array with intial val 0


  strip1.begin(); //intialize led strip
  strip1.show(); //write to strip, should be blank
  strip2.begin(); //intialize led strip
  strip2.show(); //write to strip, should be blank
  strip3.begin(); //intialize led strip
  strip3.show(); //write to strip, should be blank
}

void loop() {

  currentMillis = millis();  //capture the latest value of millis
  //this is the equivalent to noting the time from a clock
  //use the same time for all LED flashes to keep them synchronized
  update_Switch_State();

/*  simple alternating flash
   if (currentMillis - previousLed_A_Millis >= led_Flash_A_Interval){
    isLeftON = ! isLeftON;
    isRightON = ! isRightON;
    previousLed_A_Millis = currentMillis;
   }
*/

  output_Led_headlights(); //fix to update all strips, not just headlights or add separate function
  complexFlash_update();
  output_Led_Flasher();
  strip3.setPixelColor (13,80,50,0);  //set an LED on so the Battery Pack doesn't auto-off
}

  void update_Switch_State(){
// this only reads the button state after the button interval has elapsed
    //  this avoids multiple flashes if the button bounces
    // every time the button is pressed it changes buttonLed_State causing the Led to go on or off
    // Notice that there is no need to synchronize this use of millis() with the flashing Leds

    if (millis() - previousButtonMillis >= buttonInterval) {

        switchState1 = digitalRead(switch1);
        switchState2 = digitalRead(switch2);
        previousButtonMillis += buttonInterval;
        Serial.print ("switchState1 is = ");
        Serial.println (switchState1);
        Serial.print ("switchState2 is = ");
        Serial.println (switchState2);
        //delay(1000); // just for debugging


        }

}



  // write to led
 void output_Led_headlights() {
  if (switchState1 == HIGH) {
  both_headlights();
  strip1.show();
  both_taillights();
  strip2.show();
  }
  else if (switchState1 == LOW) {
    strip1.setPixelColor (ledHeadlight_Right,0,0,0);
    strip1.setPixelColor (ledHeadlight_Left,0,0,0);
    strip1.show();
    strip2.setPixelColor (ledTaillight_Right,0,0,0);
    strip2.setPixelColor (ledTaillight_Left,0,0,0);
    strip2.show();

  }
  }

 void output_Led_Flasher() {
  if (switchState2 == HIGH) {
    if (isLeftON ==true) {
      Left_Red_Lights();
      Right_Red_lights_OFF();
      strip3.show();
    }
    else if (isRightON ==true){
      Left_Red_Lights_OFF();
      Right_Red_Lights();
      strip3.show();
    }
  }

  else {
     Left_Red_Lights_OFF;
     Right_Red_lights_OFF;
     strip3.show();

  }

 }

  void Right_Red_Lights() {
    for(int i=14; i<26; i++){
      strip3.setPixelColor (i,potVal1,0,0);
    }
  }

  void Right_Red_lights_OFF(){
    for(int i=14; i<26; i++){
      strip3.setPixelColor (i,0,0,0);
    }
  }

  void Left_Red_Lights() {
    for(int i=1; i<13; i++){
      strip3.setPixelColor (i,potVal1,0,0);
    }
  }

  void Left_Red_Lights_OFF(){
    for(int i=1; i<13; i++){
      strip3.setPixelColor (i,0,0,0);
    }
  }



  void both_headlights() {
    strip1.setPixelColor (ledHeadlight_Right,potVal1,((potVal1)-20),((potVal1)-140)); //adjust color balance to be more pleasing
    strip1.setPixelColor (ledHeadlight_Left,potVal1,((potVal1)-20),((potVal1)-140));
  }

  void both_taillights() {
    strip2.setPixelColor (ledTaillight_Right,potVal1,0,0);
    strip2.setPixelColor (ledTaillight_Left,potVal1,0,0);
  }


void complexFlash_update(){
if (currentMillis - previous_led_fast_Millis >= led_fast_interval && isLeftON == true) {   //check the clock to see if its time to flash
  for (int i = 0; i< (Tpixel3); i++){                                                         //increment each index in the strip
    if (int arrayLightbarFlashCount[i] < 4 && byte arrayLightbarHasFlashed[i] == false){               //check if its flashed 4 times
      lightbar_flashstate_update (arrayLightbarLeft);                                         //update the flash state for that index
      lightbar_flashstate_update (arrayLightbarLeftYellow);                                   //update the flash state for that index
      arrayLightbarFlashCount[i]++;                                                           //increment the flash count
      previous_led_fast_Millis = currentMillis;                                               //reset the clock
    }
    else if (arrayLightbarFlashCount[i] == 4){                                                // if flashed quick 3 times, turn on flash state for long blink
      arrayLightbarHasFlashed[i] = true;
      previous_led_fast_Millis = currentMillis;


    }

}
}

  void lightbar_flashstate_update(int &x) {   //takes an array and changes its corresponding arrayState from High/LOW

for (int i = 0; i< (arrLength(x))-1, i++){  // check each element in an array
    arrayLightbarState[(x(i))] = !arrayLightbarState[(x(i))]; //check the element in the array to figure out what position in the array state to change high/low
}

}
