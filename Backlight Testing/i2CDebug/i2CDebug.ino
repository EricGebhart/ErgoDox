//#include <Wire.h>

// This program toggles the I2C pins on the Teensy every 5 seconds while printing the state
// this allows tracking of solder bridges or other defects with a multimeter

const int redLedPin = 10;
const int greenLedPin = 11;
const int blueLedPin = 12;

const int I2CClockPin = 5;
const int I2CDataPin = 6;

void setup() 
{
  Serial.begin(9600);
  pinMode(redLedPin, OUTPUT);
  pinMode(greenLedPin, OUTPUT);
  pinMode(blueLedPin, OUTPUT);
  Serial.println("Initiated Test");
  pinMode(I2CClockPin, OUTPUT);
  pinMode(I2CDataPin, OUTPUT);
}

void loop() 
{
  Serial.println("Setting color enable pins low");
  digitalWrite(redLedPin, LOW);    // set the LED off
  digitalWrite(greenLedPin, LOW);  // set the LED off
  digitalWrite(blueLedPin, LOW);   // set the LED off

  delay(5000);
  Serial.println("Clock Low");
  digitalWrite(I2CClockPin, LOW);
  delay(5000);
  Serial.println("Data Low");
  digitalWrite(I2CDataPin, LOW);
  delay(5000);
  Serial.println("Clock High");
  digitalWrite(I2CClockPin, HIGH);
  delay(5000);
  Serial.println("Data High");
  digitalWrite(I2CDataPin, HIGH);
}

