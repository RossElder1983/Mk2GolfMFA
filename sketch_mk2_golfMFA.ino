/*
  Mk2 golf boost gauge
  Shows a boost value from a pressure sensor onto an LCD screen

  This example code is in the public domain.

  modified 30 ~August 2015
  by Ross Elder
 */

/*
Reads boost from MPX4250AP on Analog 0.
Converts vacuum readings to inHg.
*/
#include <Wire.h>  // Comes with Arduino IDE
#include <LiquidCrystal_I2C.h>
#include <LiquidCrystal_I2C.h>

int mapsen = 0; // Set MAP sensor input on Analog port 0
int tempsen = 1; // Set Temp sensor input on Analog port 1
float TempOffset = 3.0;
float boost = 0.0; // Set boost value to 0
float mapval = 0.0; // Set raw map value to 0
volatile float peakboost = -30.0; // Set peak memory to low number so max displays correctly
float warnpsi = 20.5; // Set PSI for warning
float atmpsi = 14.273; //Scotland atmospheric pressure, 991 mbar, 1mbar = 0.01450377378psi


// initialize the library with the numbers of the interface pins
LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
// you can change the overall brightness by range 0 -> 255
int brightness =255;
int menuOption = 0;
byte lcdNumCols = 16; // -- number of columns in the LCD
byte sensorPin = 0; // -- value for this example

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin 13 as an output.
  Serial.begin(9600);
  lcd.begin(16,2);   // initialize the lcd for 16 chars 2 lines, turn on backlight
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
  pinMode(13, OUTPUT);

  // ------- Quick 3 blinks of backlight  -------------
  for(int i = 0; i< 1; i++)
  {
    lcd.backlight();
    delay(250);
    lcd.noBacklight();
    delay(250);
  }
  lcd.backlight(); // finish with backlight on  

  // NOTE: Cursor Position: (CHAR, LINE) start at 0  
  lcd.setCursor(2,0); //Start at character 4 on line 0
  lcd.print("VW Golf GTi");
  delay(500);
  lcd.setCursor(4,1);
  lcd.print("K04 20VT");
  delay(1000);
}

// the loop function runs over and over again forever
void loop() {
  delay(200); 
  
  int sensor2Value = digitalRead(3);
  if (sensor2Value != 1)
  {
    //Button is pressed, increment
    if (menuOption == 3)
    {
      menuOption = 0;
    }
    else
    {
      menuOption++;  
    }
  }
  
  if (menuOption == 0)
  {
    SetBoostValue();
    lcd.clear();
    lcd.setCursor(0,0); //Start at character 0 on line 0
    lcd.print("Boost");
    lcd.setCursor(0,1);
    lcd.print(String(boost, 1) + " psi");    
  }

  if(menuOption == 1)
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Peak Boost");
    lcd.setCursor(0,1);
    lcd.print(String(peakboost, 1) + " psi");
  }

  if(menuOption == 2)
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Lap Timer");
    lcd.setCursor(0,1);
    lcd.print("00:00:25");
  }

  if(menuOption == 3)
  {
    float TempValue = GetTempFromSensor();
    
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Temp");
    lcd.setCursor(0,1);
    lcd.print(String(TempValue, 1) + " oC");
  }  
  /*Serial.println("Pin2");
  Serial.println(sensor2Value, DEC);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Menu Option");
  lcd.setCursor(0,1);
  lcd.print(menuOption);

  int sensor3Value = digitalRead(3);
  Serial.println("Pin3");
  Serial.println(sensor3Value, DEC);*/

  /*int sensor4Value = digitalRead(4);
  Serial.println("Pin4");
  Serial.println(sensor3Value, DEC);*/
}

/*functions*/
 float GetTempFromSensor(){
    float temperatureValue;
    int tempReading = analogRead(tempsen); 
    float voltage = tempReading * 5.0;
    voltage /= 1024.0;
    float tempInDegreesC = (voltage - 0.5) * 100;
    
    return tempInDegreesC - TempOffset;
  }

  float SetBoostValue(){
    float boostValue;

    mapval= analogRead(mapsen); //Reads the MAP sensor raw value on analog port 0
    boost = ((((float)mapval/(float)1023+0.04)/.004)*.145)-atmpsi;

    return boostValue;
  }
