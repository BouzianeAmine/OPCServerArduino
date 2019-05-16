#include <Keypad.h>
#include <OPC.h>
#include <Bridge.h>
#include <Ethernet.h>
#include <SPI.h>

const byte ROWS = 4; 
const byte COLS = 3; 
char Keys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};

byte rowPins[ROWS] = {9, 8, 7, 6}; 
byte colPins[COLS] = {5, 4, 3}; 

Keypad keypad = Keypad( makeKeymap(Keys), rowPins, colPins, ROWS, COLS );
byte ledPin = 13; 
OPCSerial aOPCSerial;
String code="";

int callback(const char *itemID, const opcOperation opcOP, const int value){
  return code.toInt();
}

void setup(){
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);      // sets the digital pin as output
  digitalWrite(ledPin, LOW);   // sets the LED on
  keypad.addEventListener(keypadEvent); //add an event listener for this keypad
  aOPCSerial.setup(); 
}
  
void loop(){
  char key = keypad.getKey();
  if(key && key=='#'){
    Serial.println(code);
  }
  aOPCSerial.processOPCCommands();
}

//take care of some special events
void keypadEvent(KeypadEvent key){
  switch (keypad.getState()){
    case PRESSED:
      switch (key){
        case '#':   
          aOPCSerial.addItem("password",opc_readwrite, opc_int, callback);
          Serial.println("Done here, the code you just put is : ");
          Serial.println(code);
          code="";
          break;  
        default:
          code+=key;
      }
      break;
    default :
      break;
  }
}
