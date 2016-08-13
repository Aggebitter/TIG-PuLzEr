


/*               -----------   TIG PuLzEr ------------
 *  I needed short on/off pulses on my tig at an adjustable pulse lenght both manual and automatic
 *  This is for welding very thin titanium sheets or other thin metals
 *  
 *  Usage:
 *  The right nobb is multi function. Function select, Pulse time and Help
 *  The left nobb sets the puls length for the arc
 *  The Trigger activate the function
 *  
 *  Right nobb:
 *  
 *  -Function select-
 *  The left range sets the TIG PuLzEr in semi automatic mode called "TRIGGER"
 *  and the arc has to be triggerd. 
 *  The right range sets the TIG PuLzEr in autmatic mode called "PULSED"     
 *  and the arc is automaticly triggerd. 
 *  In the middle range it displays a simple help screen.
 *  
 *  -Pulse time-
 *  In the left and right function range it is still possible to adjust the 
 *  length between arc pulses. 
 *  
 *  -Help- 
 *  To get a nice "space" between semi automatic and automatic mode, I 
 *  put a gap in the range for tha potentiometer and why not use it for some
 *  basic help. 
 *  
 *  Left nobb:
 *  
 *  Sets the lenght on each pulse. Notis that that the pulse lenght cant be 
 *  longer than the pulse time and the software adjust it to be in range for
 *  the pulse time
 *  
 *  -Trigger-
 *  When in semi automatic mode "TRIGGER", if the triggger pressed for a short time it 
 *  will trigger a single pulse. If pressed for a long period it will trigger
 *  pulses as long as the trigger is pressed.
 *  In automatic mode "PULSED", if the triggger pressed for a short time it will start
 *  generating pulses automatic. Holding the trigger for a long time will stop
 *  the pulses. 
 *  Stoping the pulses can also be done by turning the right nobb to  
 *  ither Help or semi automatic mode "TRIGGER" mode.
 *  
 *  The pulse lenght and puls time can be adjusted on the fly as long as you are in
 *  the range for each function selected.
 *
 *
 *  To Do !! 
 *  
 *  1, Well found a bug. In automatic mode "PULSED", pulse length can be higher than pulse time. 
 *  
 *  2, Some testing while welding if I need to tweak some timings 
 *  
 *  3, Use interupt to fetch trigger pressed events 
 *     It's is the only way to fix a realible exit from  automatic mode pulsed loop function
 *     Might be a way to do other stuff as showing a new help page
 * 
 *  4, Hook it up to my osciloscope and measure the real timings compered to displayed
 *  
 *  5, One stepper driver controller (to do a smoooth rotating welding lathe)
 *  
 *  6, pre and post gas asjustmens (for TIG's that lacks this feature) 
 *     Thats what the 3:rd and 4:th nobbs are for
 *  
 *  7, Tweak/Replace the Adafruit GFX... that one eats a lot of memory and we dont need much of it
 *  
 *  8, Find a way to detect torch contatc with the working pice so when lifted an arc is produced
 *     by ding it this way we can use the TIG as an Pulse Arc Welder 
 *     
 *     
 *     There should be some licensing stuff here but this is just a quick 'n dirty one 
 *     and I even forgot the adafruit license ... well its used and if you use my code add it ;-)
 *     
 *     //Agge
 */


 
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif


// ********** Setup ************

int PulseLengthMin = 4;         // Lowest pulse lenght in ms

// Pin usage
int PulseFQPin = 2;             // potentiometer wiper (middle terminal) connected to analog pin 3
int PulseLengthPin = 3;         // outside leads to ground and +5V                        
int TRIGGpin = 8 ;              // for arc trigg in trigger mode      
int ledPin   = 13;              // LED connected to digital pin 13
int PulsePin = 12;              // Relay connected to digital pin 12

// ********** Setup ends *********


int PulseFQ  = 0 ;              
int PulseTime = 0 ;
int PulseLength = 0;            
int FQ = 0;  
int OffTime;                   
int enableCounter = 0;          // the pulse loop exit control

boolean triggEN = false;
boolean pulseEN = false;

char MODE[10] = "Pulse TIG";

// ************* Some basic functions ***********

int ReadPulseFQSensor(){
  int i;
  int sval = 0;

  for (i = 0; i < 5; i++){
    sval = sval + analogRead(PulseFQPin);    // sensor on analog pin
  }

  sval = sval / 5;    // average
  return sval;
}

int ReadPulseLenghtSensor(){
  int i;
  int sval = 0;

  for (i = 0; i < 5; i++){
    sval = sval + analogRead(PulseLengthPin);    // sensor on analog pin
  }

  sval = sval / 5;    // average
  return sval;
}

int ReadTriggSwitch(){
  
  boolean sval = 0;
    sval = digitalRead(TRIGGpin);    // switch on digital pin
    sval = !sval; // inverting the digital read due internal pullup
  return sval;
}



void setup()   {   
               
 
  
  pinMode(ledPin, OUTPUT);      // sets the digital pin as output
  pinMode(PulsePin, OUTPUT);      // sets the digital pin as output
  pinMode(TRIGGpin, INPUT);           // set pin to input
  digitalWrite(TRIGGpin, HIGH);       // turn on pullup resistors
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)
  display.clearDisplay();
  display.setCursor(0,0);
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.println("TIG PuLzEr");
  display.println("");
  display.setTextSize(2);
  display.println(" by Agge");   
  display.setTextSize(1);
  display.println("    Ver 1.3");
  display.display();
  delay(5000);
}


void loop() {
//
display.clearDisplay(); // start the main loop with a clean display


PulseFQ = ReadPulseFQSensor();
PulseLength = ReadPulseLenghtSensor();
triggEN = ReadTriggSwitch();

// rules for pulse
if (PulseLength <= PulseLengthMin) PulseLength = PulseLengthMin; 
if (PulseFQ <= PulseLengthMin*2) PulseFQ = PulseLengthMin*2;
if (PulseLength >= PulseFQ) PulseLength = PulseFQ - PulseLengthMin; 

// some filtering for the function select
if (PulseFQ >= 400 && PulseFQ <= 474) PulseFQ = 400;
if (PulseFQ >= 526 && PulseFQ <= 600) PulseFQ = 600;
 
OffTime = PulseFQ -PulseLength;              // geting the on/off Trigg timeing
PulseTime = PulseFQ;

  display.setCursor(0,0);
  display.setTextSize(2);


  if (triggEN == 1) {
  display.setTextColor(BLACK, WHITE);
  enableCounter = ++ enableCounter;
  } 
  else {
  display.setTextColor(WHITE);
  }

  if (PulseFQ >= 600) // go to trigger mode
  {
    enableCounter = 0;
    display.setCursor(5,0);
    display.setTextColor(WHITE);
    display.println("-TRIGGER-");
    trigger();
    triggEN = 0;
    //display.clearDisplay();
  }
  
  if (PulseFQ <= 400) // go to pulse mode
  {
    
    display.setCursor(20,0);
    display.println("-PULSE-");
      pulse();
    if (enableCounter > 1){enableCounter = 0;}

  }
  
  if (PulseFQ > 475 && PulseFQ < 525) {
    
   dissable();
   }
  //display.setCursor(10,0);
  display.setTextColor(WHITE);
   
  display.println("");
  display.print(" ");
  display.setCursor(32,32);
  display.print(PulseLength);
  display.setCursor(96,32);
  display.print("ms");
  display.setCursor(32,48);
  display.print(PulseTime);
  display.setCursor(96,48);
  display.print("ms");
  display.display();

}






// *********  Arc starts here ***********

void arc(void) {

  digitalWrite(ledPin, HIGH);   // sets the LED on
  digitalWrite(PulsePin, HIGH);   // sets the relay on
  delay(PulseLength);
  digitalWrite(ledPin, LOW);    // sets the LED off
  digitalWrite(PulsePin, LOW);    // sets the relay off
  delay(OffTime);
}


// *********  Trigger starts here ***********

void trigger(void) {


triggEN = ReadTriggSwitch();
if (triggEN == 1){ 
 
  display.setCursor(5,0);
  display.print("       ");
  display.setTextColor(BLACK, WHITE);
  display.setCursor(5,0);
  display.println("-TRIGGER-");
  display.setTextColor(WHITE);
  display.println("");
  display.print(" ");
  display.setCursor(32,32);
  display.print(PulseLength);
  display.setCursor(96,32);
  display.print("ms");
  display.setCursor(32,48);
  display.print(PulseTime);
  display.setCursor(96,48);
  display.print("ms");
  display.display();
  
arc();
  
  display.setCursor(5,0);
  display.setTextColor(WHITE);
  //display.clearDisplay();
  display.println("-TRIGGER-");
  display.setTextColor(WHITE);
  display.println("");
  display.print(" ");
  display.setCursor(32,32);
  display.print(PulseLength);
  display.setCursor(96,32);
  display.print("ms");
  display.setCursor(32,48);
  display.print(PulseTime);
  display.setCursor(96,48);
  display.print("ms");
  display.display();      

  
}
}




// *********  Pulse rutine starts here ***********

void pulse(void) {

triggEN = ReadTriggSwitch();

if (enableCounter == 1){pulseEN = true;}else {pulseEN = false;}

if (pulseEN == true) { 

  display.setCursor(20,0);
  display.setTextColor(BLACK, WHITE);
  display.clearDisplay();
  display.println("-PULSE-");
  display.setTextColor(WHITE);
  display.println("");
  display.print(" ");
  display.setCursor(32,32);
  display.print(PulseLength);
  display.setCursor(96,32);
  display.print("ms");
  display.setCursor(32,48);
  display.print(PulseTime);
  display.setCursor(96,48);
  display.print("ms");
  display.display();
arc();
  
  display.setCursor(20,0);
  display.setTextColor(WHITE);
  display.clearDisplay();
  display.println("-PULSE-");
  display.setTextColor(WHITE);
  display.println("");
  display.print(" ");
  display.setCursor(32,32);
  display.print(PulseLength);
  display.setCursor(96,32);
  display.print("ms");
  display.setCursor(32,48);
  display.print(PulseTime);
  display.setCursor(96,48);
  display.print("ms");
  display.display();     
}
 
}

void dissable(void) {
 
display.setTextColor(WHITE);

String text = "  Turn RIGHT nobb left or right to select function... Press TRIGGER to start selected function...  "; // First line
const int width = 10; // width of the marquee display (in characters)

// Loop once through the string
for (int offset = 0; offset < text.length(); offset++)
  {

        // Construct the string to display for this iteration
    String t = "";
    
      for (int i = 0; i < width; i++)
      
      t += text.charAt((offset + i) % text.length());
      display.setCursor(0,0);
      display.print("TIG PuLzEr");
      display.setCursor(0,16);
      display.print(" - Help- ");
      // Print the string for this iteration
      display.setCursor(0,48); 
      display.print(t);
      display.display();

  // bail out on sensor detect
       PulseFQ = ReadPulseFQSensor();
       if (PulseFQ <= 400 || PulseFQ >= 600){      
       break;
       }
       
 // Short delay so the text doesn't move too fast
delay(200);
display.clearDisplay();

  }
}

