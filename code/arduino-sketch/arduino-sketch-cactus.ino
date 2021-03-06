/*

-----------------
Soft_Serial_Typer
-----------------

-

Software serial controlled keyboard sketch by Exploit.Agency
For 32U4 based boards

-

For individual keypresses or combinations of key presses send the following via software serial:
-"Press:X" or "Press:X+Y" or "Press:X+Y+Z" and so forth
--Expects DECIMAL Values for X,Y,Z,etc
--Sending the following via software serial "Press:131+114" with the Arduino's USB plugged into a Windows machine would output KEY_LEFT_GUI(Windows_Key)+r thus launching the run prompt
--List of modifier keys(GUI,ALT,CTRL,ETC) at https://www.arduino.cc/en/Reference/KeyboardModifiers
--ASCII table lookup at http://www.asciitable.com/

To type out strings of text send via software serial:
-"Print:XYZ" Types out "XYZ"
--Sending the following via software serial "Print:www.Exploit.Agency" would type out "www.Exploit.Agency" on the machine connected via USB

To type out strings of text send via software serial:
-"PrintLine:XYZ" Types out "XYZ" then presses enter
--Sending the following via software serial "PrintLine:www.Exploit.Agency" would type out "www.Exploit.Agency" on the machine connected via USB and press enter

To make a delay:
-"Delay"
--Sending the following via software serial "Delay" would wait for X seconds(length of delay set in esp8266 sketch) on the machine connected via USB before proceeding to next item in payload

*/

//Requires Keyboard and SoftwareSerial library
#include <Keyboard.h>
//#include <SoftwareSerial.h>

//Setup RX and TX pins to be used for the software serial connection
//const int RXpin=10;
//const int TXpin=11;
//SoftwareSerial SOFTserial(RXpin,TXpin);

//Prints debug lines via serial if set to 1
//const int debug=0;

//Used later for determining if we are ready to release a key press or a combination of key presses
int keypressdone=0;

//Start serial interfaces and HID keyboard emulator
void setup() { 

//Start up the ESP 8266
  pinMode(13, OUTPUT); 
  digitalWrite(13,HIGH); 
 
//  SOFTserial.begin(4800);
//  if(debug==1) {
//    Serial.begin(9600);
//  }
  Serial1.begin(4800);
  Keyboard.begin();
}

//Do-IT!
void loop() {  
//  while (SOFTserial.available()) {
  while (Serial1.available()) {
//    String cmd = SOFTserial.readStringUntil(':');
    String cmd = Serial1.readStringUntil(':');

    //If command equals "Press:X" or "Press:X+Y+ETC"
    if(cmd == "Press"){
      keypressdone=1;
//      String fullkeys = SOFTserial.readString();
      String fullkeys = Serial1.readString();
      int str_len = fullkeys.length()+1; 
      char keyarray[str_len];
      fullkeys.toCharArray(keyarray, str_len);
//      SOFTserial.println(fullkeys);
      char delimiter[] = "+";
      char *keypart;
      keypart = strtok(keyarray, delimiter);
      while(keypart != NULL) {
//        if(debug==1) {
//          Serial.print("Pressing: "); //Debug line
//          Serial.println(keypart); //Debug line
//        }
        int key = atoi(keypart);
        delay(25);
        Keyboard.press(key);
        keypart = strtok(NULL, delimiter);
      }
      
      if(keypressdone==1) {
        delay(25);
        Keyboard.releaseAll();
//        if(debug==1) {
//          Serial.println("Releasing all keys."); //Debug line
//        }
        keypressdone=0;
      }
    }
  
    //If command equals "Print:X"
    else if(cmd == "Print") {
//      String keycode = SOFTserial.readString();
      String keycode = Serial1.readString();
//      if(debug==1) {
//        Serial.print("Printing: "); //Debug line
//        Serial.println(keycode); //Debug line
//      }
      delay(25);
      Keyboard.print(keycode);
    }

    //If command equals "PrintLine:X"
    else if(cmd == "PrintLine") {
//      String keycode = SOFTserial.readString();
      String keycode = Serial1.readString();
//      if(debug==1) {
//        Serial.print("Printing Line: "); //Debug line
//        Serial.println(keycode); //Debug line
//      }
      delay(25);
      Keyboard.print(keycode);
      delay(25);
      Keyboard.press(KEY_RETURN);
      delay(25);
      Keyboard.release(KEY_RETURN);
    }
    
  }
}
