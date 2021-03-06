/* THIS CODE IS FOR INTERFACE DEMO. LCD SCREEN IS CONTROLLED BY UP/DOWN AND ON/OFF BUTTONS.

*** PIN CONNECTIONS FOR LCD SCREEN ***
LCD pin#1 -->    ground
LCD pin#2 -->    Vin (5v or 3.3V)
LCD pin #3 -->   potentiometer
LCD pin#4 -->    UNO digital pin #7 --> MEGA 16
LCD pin #5 -->   ground
LCD pin #6 -->   UNO digital pin #8 --> MEGA 15
LCD pin #7 & 8 & 9 & 10 --> empty, no connections
LCD pin #11 -->  UNO digital pin #9 ~ --> MEGA 13 ~(not sure if PMW is indeed needed, will test later)
LCD pin #12 -->  UNO digital pin #10 ~ --> MEGA  12~ (not sure if PMW is indeed needed, will test later)
LCD pin #13 -->  UNO digital pin #11 ~ --> MEGA 11~ (not sure if PMW is indeed needed, will test later)
LCD pin #14 -->  UNO digital pin #12 --> MEGA 14
LCD pin #15 --> Vin (5V or 3.3V) WHEN USING ON/OFF BUTTON : MEGA 7
LCD pin #16 --> Arduino digital pin #3 ~ --> MEGA 10~
LCD pin #17 --> Arduino digital pin #5 ~ --> MEGA 9~
LCD pin #18 -->   Arduino digital pin #6 ~ --> MEGA 8~


/////////FOR UP/DOWN BUTTONS:
// Button pin 1 --> ground
// button pin 2 --> Arduino digital 4 --> MEGA 17
// button pin 3 --> Arduino digital 13 --> MEGA 18

/// FOR ON/OFF BUTTON (look at the side with the lines):
// Button pin 1 --> 3.3V
// Button pin 2 --> ground
// Button pin 3 --> 19

// FOR LEDS:
// HOTALARM: MEGA 6
// COLDALARM: MEGA 5

*/


#include <LiquidCrystal.h>
#include <Wire.h>

const int onButtonPin = 19;     // the number of the pushbutton pins
const int  lcdPin=  7;      // the number of the LED pin
// variables will chanlge:
int onButtonState = 0;         // variable for reading the pushbutton status
String onState="OFF";

const int buttonPin[]={17, 18, 0, 1};
int buttonState=-1;
float temp = 36.0;
float bassinetTemp = 36.0;
float error;
int increment;
String State= "NORMAL";
String lastState="NORMAL";
String reading="NORMAL";
unsigned long lastDebounceTime=0;
unsigned long debounceDelay= 50;

#define REDLITE 10
#define GREENLITE 9
#define BLUELITE 8

#define HOTALARM 6
#define COLDALARM 5

//LiquidCrystal lcd(7,8,9,10,11,12);
LiquidCrystal lcd(16,15,13,12,11,14);

int brightness=255;
byte celcius[8] = {
  0b11000,
  0b11000,
  0b11,
  0b100,
  0b100,
  0b100,
  0b11
};

byte celcius_symbol[8]{
  0b110,
  0b1001,
  0b1001,
  0b110,
  0b0,
  0b0,
  0b0
  
};

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  for (int x=0; x<2; x++){
    pinMode (buttonPin[x], INPUT_PULLUP);
  }
lcd.begin(16, 2);
  lcd.createChar(0,celcius);
  lcd.createChar(1,celcius_symbol);
 
  pinMode(REDLITE, OUTPUT);
  pinMode(GREENLITE, OUTPUT);
  pinMode(BLUELITE, OUTPUT);
  pinMode(lcdPin, OUTPUT);
    pinMode(onButtonPin, INPUT_PULLUP);
  
 
  brightness = 255;
}

void loop() {

    while (onState == "OFF"){
    lcd.setCursor(0,1);
    lcd.print("       ");
    digitalWrite(lcdPin, LOW);

    lcd.setCursor(10,1);
    lcd.print("       ");
    turnOnOff();
        }


  
    turnOnOff();
    calculateError();
    setTemp();
    changeBacklight();


for (int i=0; i<=increment*500; i++){
  turnOnOff();
  setTemp();
  changeBacklight();
    
    if ((i%500==0)& (i!=0)){

      if (error>0.0){
        
      bassinetTemp+=0.5;
      changeOperations();
      
      } if (error<0.0){
      bassinetTemp-=0.5;
      changeOperations();
      }}}
}



    
 void setBacklight(uint8_t r, uint8_t g, uint8_t b) {
 
  r = map(r, 0, 255, 0, brightness);
  g = map(g, 0, 255, 0, brightness);
  b = map(b, 0, 255, 0, brightness);
 
  // common anode so invert!
  r = map(r, 0, 255, 255, 0);
  g = map(g, 0, 255, 255, 0);
  b = map(b, 0, 255, 255, 0);
  analogWrite(REDLITE, r);
  analogWrite(GREENLITE, g);
  analogWrite(BLUELITE, b);
}   

void setTemp(){
     for(int x=0; x<2; x++)
  {
    //signifying the state of which the button is in by reading the appropriate pin #
    buttonState = digitalRead(buttonPin[x]);

    // check if the pushbutton on the keypad is pressed.
    // if it is, the buttonState is LOW:
    if (buttonState == LOW && buttonPin[x] == 17 && temp<40.0) {
      float debouncer = millis();
      if (debouncer > 1000){          
      temp=temp+0.5;
      delay(400);
            }
      }
    
    if (buttonState == LOW && buttonPin[x] == 18 && temp > 28.0) {
      // turn LED off:
       float debouncer = millis();
      if (debouncer > 1000){          
      temp=temp-0.5;
      delay(400);
      }

   
    }}
  writeTemp();
}

void writeTemp(){
  
    lcd.setCursor(0,1);
    lcd.print(String(bassinetTemp, 1));
    lcd.write(byte(1));
    lcd.print("C");

    lcd.setCursor(10,1);
    lcd.print(String(temp, 1));
    lcd.write(byte(1));
    lcd.print("C");
   
}

void changeBacklight(){
      if (bassinetTemp>37.5){
      setBacklight( 255, 0, 0);
      analogWrite(HOTALARM, 255);
      analogWrite(COLDALARM, 0);
      
     }
    else if (bassinetTemp<35.5){
      setBacklight( 0, 0, 255);
      analogWrite(HOTALARM, 0);
      analogWrite(COLDALARM, 255);
  }
    else{
      setBacklight( 0, 255, 0);
      analogWrite(COLDALARM, 0);
      analogWrite(HOTALARM, 0);
    }
}
    void calculateError(){
      error= temp-bassinetTemp;
       increment=abs(error/0.5);
    }







void changeOperations(){
      turnOnOff();
      writeTemp();
      changeBacklight();
      setTemp();
      calculateError();
}

void turnOnOff(){
        onButtonState = digitalRead(onButtonPin);


    // check if the pushbutton on the keypad is pressed.
    // if it is, the buttonState is LOW:
    if ( onButtonState == LOW && onButtonPin == 19) {

      float debouncer = millis();
      if (debouncer > 1000){    
      if (onState=="ON"){
        onState="OFF";
      }else{
        onState="ON";
   
      }
      delay(400);
            }
      }

     if (onState=="ON"){
      digitalWrite(lcdPin, HIGH);
      writeTemp(); 
     }else{
      analogWrite(HOTALARM,0);
      analogWrite(COLDALARM,0);
      digitalWrite(lcdPin, LOW);
      
     }
  
}
