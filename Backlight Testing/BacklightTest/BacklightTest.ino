/**
 * ErgoDOX RGB Backlight test
 *
 * Copyright 2013 Jarl Stefansson
 * This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 * 
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 * 
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#include "Wire.h"

const int redLedPin = 10;
const int greenLedPin = 11;
const int blueLedPin = 12;

// Default I2C address of the LM3549 LED Driver is 36 hex or 54 decimal
const int driverAddress = 54;

// Configure initial register and storage for register data
int currentRegister = 0;
word currentRegisterValue;

// Initialize
void setup()
{
    // Enable PWM pins for each LED
    pinMode(redLedPin, OUTPUT);
    pinMode(greenLedPin, OUTPUT);
    pinMode(blueLedPin, OUTPUT);
    
    // By default turn all LEDs off (if current is to high in stored settings we might burn out the LED
    digitalWrite(redLedPin, LOW);    // set the LED off
    digitalWrite(greenLedPin, LOW);  // set the LED off
    digitalWrite(blueLedPin, LOW);   // set the LED off
    
    // Start I2C
    Wire.begin();
    Wire.beginTransmission(driverAddress);
    Wire.send(0);
    Wire.endTransmission();
    Wire.requestFrom(driverAddress, 1);
    
    while( Wire.available() == 0);
      currentRegisterValue = Wire.receive();
    
    // Start Serial console
    Serial.begin(9600);
    Serial.println("Starting ErgoDOX Backlight testing application");
    Serial.println("Reading all registers");
    Serial.print("Register ");
    Serial.print(currentRegister, DEC);
    Serial.print(" contains ");
    Serial.print(currentRegisterValue, DEC);
    Serial.print("\n");

    Serial.println("Completed Intialization");
    
    // Save Config
    // To store current register values to EEPROM user needs to first write 04h to register 40h (EE init bit to “1”) followed by 02h to register 40h (EE prog bit to “1”).
    // Once EEPROM programming is completed LM3549 sets EE_ready bit to 1
    // EEPROM programming should always be done in standby mode
    //--------------------------------------------------------------------------------
    // Set Current
    // There are three 10 bit current settings for each driver. 10 bits are divided into two eight bit registers. First register
    // holds the eight least significant bits (LSB) and the second register holds the two most significant bits (MSB).
    // These settings are grouped into three banks. IR0, IG0 and IB0 form a bank0; IR1, IG1 and IB1 form a bank1 and
    // IR2, IG2 and IB2 form a bank2. For example IR0_MSB holds the two MSB for red on bank0 and IR0_LSB the
    // eight LSB for red on bank0. Bank is selected with BANK_SEL register (00 = bank0, 01 = bank1 and 10 or 11 = bank2).
    
    // For currents up to 550 mA current setting can be calculated using formula
    // ISET = (target current in mA - 100 mA)/(650mA/1024)
    
    // For currents between 550mA and 700mA current setting can be calculated using formula:
    // ISET = (target current in mA - 550 mA)/0.479 mA + 710
    //---------------------------------------------------------------------------------
    // Set Brightness
    // In case of PWM input brightness control (BRC) is the positive duty cycle of the input signal.
    // In case of FADER register brightness is MASTER FADER[7:0]/255

    // MFE  PWM  Brightness Control
    // 0    0    No brightness control
    // 0    1    PWM input
    // 1    0    FADER register
    // 1    1    PWM input
    
    // Brightness control keeps the ratio of
    // the driver currents constant and adjusts the output currents based on the highest current setting. Driver currents
    // can be adjusted between 100 mA to the maximum current set in the registers
    
    // ISET1 =highest current setting
    // ISET2 =current setting 2
    // ISET3 =current setting 3
    // R1 =(ISET2/ISET1), ratio of current 2 and the highest current
    // R2 =(ISET3/ISET1), ratio of current 3 and the highest current
    // BRC =brightness control
    // I1 = ISET1 x BRC)
    // I2 = I1 x R1
    // I3 = I1x R2
    
    // To ensure that control takes effect for the next color time from PWM
    // change to next enable needs to be greater than timeout time (300 μs typical)
    
    //----------------------------------------------------------------------------------
    // Fault Detection
    
    // LED OPEN FAULT
    // Open fault is generated when at the end of color VOUT is at maximum and no current is flowing through driver
    // (VDx = 0V). Also OCP fault needs to be low. Open fault can be generated by broken LED or a soldering defect.
    
    // LED SHORT FAULT
    // Short fault is detected when VOUT < 1.0V at the end of a color. Short fault is generated when VOUT is shorted
    // to driver by soldering defect or faulty LED. Driver current limit limits the maximum current. Depending on output
    // current and positive limit settings, LED short can also generate OCP fault to fault register.
    
    // TSD FAULT
    // Thermal shutdown (TSD) fault is generated if junction temperature rises above TSD level. TSD engages at TJ=
    // 150°C (typ) and disengages at TJ = 140°C (typ). TSD sets device to standby mode. Occasionally a false TSD
    // fault is generated to Fault register when device goes from shutdown mode to standby mode. It is good practice to
    // reset the fault register by reading it every time after device is set from shutdown mode to standby mode.
    
    // UVLO FAULT
    // Under voltage lock out (UVLO) fault is generated if VIN drops below UVLO level (~2.5V). UVLO sets device to
    // standby mode. When VIN rises back above the 2.5V device exits UVLO. If control register values were changed
    // from EEPROM defaults they need to be rewritten to registers because UVLO condition can generate EEPROM
    // read sequence.
    
    // OVER CURRENT PROTECTION FAULT
    // Over current protection (OCP) fault is generated when positive current limit is active at the end of a color. It is
    // important to notice that OCP fault is not always set when positive current limit is activated. Positive current limit
    // can activate during normal operation when buck-boost is adjusting the output voltage to a higher level. OCP can
    // be caused by short from VOUT to GND, short from driver to GND or if too low positive current limit value is set
    // for desired output current.

    //-----------------------------------------------------------------------------------
    // Register Map
    // ADDR NAME      D7      D6      D5      D4      D3      D2       D1        D0       DEFAULT     NOTE
    // ----|--------|-----|------|--------|--------|-------|-------|--------|--------|-------------|--------
    // 00H  BANK_SEL  -------------------------------------------- |   Bank_sel[1:0] |    00H         EEPROM
    // 01H  IR0_LSB   ----------------------- Red 0 [7:0] -------------------------- |    81H         EEPROM
    // 02H  IR0_MSB   ------------------- N/A -------------------- |   Red 0 [9:8]   |    01H         EEPROM
    // 03H  IG0_LSB   ----------------------- Green 0 [7:0] -------------------------|    81H         EEPROM
    // 04H  IG0_MSB   ------------------- N/A ---------------------|   Green 0 [9:8] |    01H         EEPROM
    // 05H  IB0_LSB   ----------------------- Blue 0 [7:0] --------------------------|    81H         EEPROM
    // 06H  IB0_MSB   ------------------- N/A ---------------------|   Blue 0 [9:8]  |    01H         EEPROM
    // 07H  IR1_LSB   ----------------------- Red 1 [7:0] ---------------------------|    E7H         EEPROM
    // 08H  IR1_MSB   ------------------- N/A ---------------------|   Red 1 [9:8]   |    00H         EEPROM
    // 09H  IG1_LSB   ----------------------- Green 1 [7:0] -------------------------|    E7H         EEPROM
    // 0AH  IG1_MSB   ------------------- N/A ---------------------|   Green 1 [9:8] |    00H         EEPROM
    // 0BH  IB1_LSB   ----------------------- Blue 1 [7:0]  -------------------------|    E7H         EEPROM
    // 0CH  IB1_MSB   ------------------- N/A ---------------------|   Blue 1 [9:8]  |    00H         EEPROM
    // 0DH  IR2_LSB   ----------------------- Red 2 [7:0] ---------------------------|    4DH         EEPROM
    // 0EH  IR2_MSB   ------------------- N/A ---------------------|   Red 2 [9:8]   |    00H         EEPROM
    // 0FH  IG2_LSB   ----------------------- Green 2 [7:0] -------------------------|    4DH         EEPROM
    // 10H  IG2_MSB   ------------------- N/A ---------------------|   Green 2 [9:8] |    00H         EEPROM
    // 11H  IB2_LSB   ----------------------- Blue 2 [7:0] --------------------------|    4DH         EEPROM
    // 12H  IB2_MSB   ------------------- N/A ---------------------|   Blue 2 [9:8]  |    00H         EEPROM
    // 13H  FADER     ----------------------- MASTER FADER [7:0] --------------------|    FFH         EEPROM
    // 14H  CTRL      --- N/A ---| SOFT START[1:0] | TIME OUT[1:0] |  MFE   |   PWM  |    00H         EEPROM
    // 15H  ILIMIT    --- N/A ---| POS_LIMIT[1:0]  |   N/A         |  NEG_LIMIT[1:0] |    11H         EEPROM
    // 16H  F_MASK    -------- N/A -------| SHORT  | OPEN  | UVLO  |  TSD   |   OCP  |    00H         EEPROM
    // 17H  FAULT     N/A | SHORT[1:0]    |    OPEN[1:0]   | UVLO  |  TSD   |   OCP  |    00H         Read Only
    // 19H  USR1      ----------------------- User Register1[7:0] -------------------|    00H         EEPROM
    // 1AH  USR2      ------------------------User Register2[7:0] -------------------|    00H         EEPROM
    // 40H  EEPROM    EE  | -------------------------------|  EE   |   EE   |   EE   |    00H         R/W  
    //      CONTROL  ready| -------------------------------| init  |  prog  |  read  |
    // ----|--------|-----|------|--------|--------|-------|-------|--------|--------|-------------|--------
    // ADDR NAME      D7      D6      D5      D4      D3      D2       D1        D0       DEFAULT     NOTE


}  

// standard Arduino loop()
void loop() 
{
  Serial.println("Cycling LEDs for stress test");
  delay(300);
}
