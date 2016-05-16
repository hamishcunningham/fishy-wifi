/*
 This is a simple example showing how to interface our mini I2C pH interface.
 The usage for this design is very simple, as it uses the MCP3221 I2C ADC. Although actual
 pH calculation is done offboard the analog section is very well laid out giving great results
 at varying input voltages (see vRef for adjusting this from say 5v to 3.3v).
 MinipH can operate from 2.7 to 5.5V to accommodate varying levels of system. Power VCC with 3.3v for a raspi!
 
 ADC samples at ~28.8KSPS @12bit (4096 steps) and has 8 I2C address of from A0 to A7 (Default A5)
 simply assemble the 2 BYTE registers from the standard I2C read for the raw reading.
 conversion to pH shown in code.
 
 Note: MinipH has an optional Vref(4.096V) that can be bypassed as well!
 Sparky's Widgets 2012
 http://www.sparkyswidgets.com/Projects/MiniPh.aspx
 */
//I2C Library
#include <Wire.h>
//We'll want to save calibration and configuration information in EEPROM
#include <avr/eeprom.h>
//EEPROM trigger check
#define Write_Check      0x1234

#define ADDRESS 0x4D // MCP3221 A5 in Dec 77 A0 = 72 A7 = 79)
                     // A0 = x48, A1 = x49, A2 = x4A, A3 = x4B, 
                     // A4 = x4C, A5 = x4D, A6 = x4E, A7 = x4F

//Our parameter, for ease of use and eeprom access lets use a struct
struct parameters_T
{
  unsigned int WriteCheck;
  int pH7Cal, pH4Cal;
  float pHStep;
} 
params;

float pH;
const float vRef = 4.096; //Our vRef into the ADC wont be exact
                    //Since you can run VCC lower than Vref its
                    //best to measure and adjust here
const float opampGain = 5.25; //what is our Op-Amps gain (stage 1)

void setup(){
  Wire.begin(); //initialize I2C
  Serial.begin(9600);
  //Lets read our Info from the eeprom and setup our params,
  //if we loose power or reset we'll still remember our settings!
  eeprom_read_block(&params, (void *)0, sizeof(params));
  Serial.println(params.pHStep);
  //if its a first time setup or our magic number in eeprom is wrong reset to default
  if (params.WriteCheck != Write_Check){
    reset_Params();
  }
}

void loop(){
  //This is our I2C ADC interface section
  //We'll assign 2 BYTES variables to capture the LSB and MSB(or Hi Low in this case)
  byte adc_high;
  byte adc_low;
  //We'll assemble the 2 in this variable
  int adc_result;
  
   Wire.requestFrom(ADDRESS, 2);        //requests 2 bytes
   while(Wire.available() < 2);         //while two bytes to receive
   //Set em 
   adc_high = Wire.read();           
   adc_low = Wire.read();
   //now assemble them, remembering our byte maths a Union works well here as well
   adc_result = (adc_high * 256) + adc_low;
   //We have a our Raw pH reading fresh from the ADC now lets figure out what the pH is  
   calcpH(adc_result);
   
   //Lets handle any commands here otherwise if we do prior to a fresh ADC reading
   //may end up calibrate to slightly older data (this really might not matter, handle as you will)
   if(Serial.available() ) 
   {
    char c = Serial.read();
    if(c == 'C')
    {
      //Which range?
      int calrange;
      calrange = Serial.parseInt();
      if( calrange == 4 ) calibratepH4(adc_result);
      if( calrange == 7 ) calibratepH7(adc_result);
    }
    if(c == 'I')
    {
     //Lets read in our parameters and spit out the info! 
     eeprom_read_block(&params, (void *)0, sizeof(params));
     Serial.print("pH 7 cal: ");
     Serial.print(params.pH7Cal);
     Serial.print(" | ");
     Serial.print("pH 4 cal: ");
     Serial.print(params.pH4Cal);
     Serial.print(" | ");
     Serial.print("pH probe slope: ");
     Serial.println(params.pHStep); 
    }
   }
   //Spit out some debugging/Info to show what our pH and raws are
   Serial.print("pH: ");
   Serial.print(pH);
   Serial.print(" | "); 
   Serial.println(adc_result);
   //You can delay or millis here depending on what tasks(others) you may have 
   delay(1000);
}


//Lets read our raw reading while in pH7 calibration fluid and store it
//We will store in raw int formats as this math works the same on pH step calcs
void calibratepH7(int calnum)
{
  params.pH7Cal = calnum;
  calcpHSlope();
  //write these settings back to eeprom
  eeprom_write_block(&params, (void *)0, sizeof(params)); 
}

//Lets read our raw reading while in pH4 calibration fluid and store it
//We will store in raw int formats as this math works the same on pH step calcs
//Temperature compensation can be added by providing the temp offset per degree
//IIRC .009 per degree off 25c (temperature-25*.009 added pH@4calc)
void calibratepH4(int calnum)
{
  params.pH4Cal = calnum;
  calcpHSlope();
  //write these settings back to eeprom
  eeprom_write_block(&params, (void *)0, sizeof(params));
}

//This is really the heart of the calibration process, we want to capture the
//probes "age" and compare it to the Ideal Probe, the easiest way to capture two readings,
//at known point(4 and 7 for example) and calculate the slope.
//If your slope is drifting too much from ideal (59.16) its time to clean or replace!
void calcpHSlope ()
{
  //RefVoltage * our deltaRawpH / 12bit steps *mV in V / OP-Amp gain /pH step difference 7-4
   params.pHStep = ((((vRef*(float)(params.pH7Cal - params.pH4Cal))/4096)*1000)/opampGain)/3;
}

//Now that we know our probe "age" we can calculate the proper pH Its really a matter of applying the math
//We will find our millivolts based on ADV vref and reading, then we use the 7 calibration
//to find out how many steps that is away from 7, then apply our calibrated slope to calculate real pH
void calcpH(int raw)
{
 float miliVolts = (((float)raw/4096)*vRef)*1000;
 float temp = ((((vRef*(float)params.pH7Cal)/4096)*1000)- miliVolts)/opampGain;
 pH = 7-(temp/params.pHStep);
}

//This just simply applies defaults to the params in case the need to be reset or
//they have never been set before (!magicnum)
void reset_Params(void)
{
  //Restore to default set of parameters!
  params.WriteCheck = Write_Check;
  params.pH7Cal = 2048; //assume ideal probe and amp conditions 1/2 of 4096
  params.pH4Cal = 1286; //using ideal probe slope we end up this many 12bit units away on the 4 scale
  params.pHStep = 59.16;//ideal probe slope
  eeprom_write_block(&params, (void *)0, sizeof(params)); //write these settings back to eeprom
}
