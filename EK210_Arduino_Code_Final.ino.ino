#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "RTClib.h"
RTC_DS1307 RTC;

#include "Adafruit_FONA.h"
#define FONA_RX 2
#define FONA_TX 3
#define FONA_RST 4 //sms
#include <SoftwareSerial.h>
SoftwareSerial fonaSS = SoftwareSerial(FONA_TX, FONA_RX);
SoftwareSerial *fonaSerial = &fonaSS;
Adafruit_FONA fona = Adafruit_FONA(FONA_RST);

#define phoneNumber "18455218631"
#define message "Warning! Patient failed to take scheduled pill!"

//Create Motor Shield Objects
Adafruit_MotorShield AFMSbottom = Adafruit_MotorShield(0x60);
Adafruit_MotorShield AFMStop = Adafruit_MotorShield(0x61);

//Connect Motors
Adafruit_StepperMotor *Motor1 = AFMSbottom.getStepper(513, 1);
Adafruit_StepperMotor *Motor2 = AFMSbottom.getStepper(513, 2);
Adafruit_StepperMotor *Motor3 = AFMStop.getStepper(513, 1);

//Assign digital pin 9 to buzzer output
const int buzzer = 9;

void setup () {
  Serial.begin(9600);
  Wire.begin();
  //General Setup Code (LED, Buzzer, Button)
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(buzzer, OUTPUT);

  //Stepper Motor Setup Code
  AFMSbottom.begin();
  AFMStop.begin();
  Motor1->setSpeed(5);
  Motor2->setSpeed(5);
  Motor3->setSpeed(5);



  //FONA Setup Code
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  Serial.println("SMS Messages Sender");

  fonaSerial->begin(4800);
  if (! fona.begin(*fonaSerial)) {
    Serial.println(F("Couldn't find FONA"));
    while (1);
  }
  Serial.println(F("FONA is OK"));
  Serial.print(F("Found "));

  
  //RTC Setup Code
  RTC.begin();
  if (! RTC.isrunning()) {
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    RTC.adjust(DateTime(__DATE__, __TIME__));
  }
}

void loop () {
  DateTime now = RTC.now();

  //First pill push arbitrary time - one from 1 & 2, 2 from 3
  if(now.hour() == 13 && now.minute() == 55 && now.second()== 00){
      digitalWrite(LED_BUILTIN, HIGH);
      Motor1->step(300, FORWARD, SINGLE);
      delay(2000);
      Motor1->step(300, BACKWARD, SINGLE); 
      delay(2000);
      Motor1->step(300, FORWARD, SINGLE);
      delay(2000);
      Motor1->step(300, BACKWARD, SINGLE); 
      delay(2000);
      digitalWrite(LED_BUILTIN, LOW);
      tone(buzzer, 1000);
      delay(1000);
      noTone(buzzer);
      delay(1000);
      fona.sendSMS(phoneNumber, message);
  }
}


  

