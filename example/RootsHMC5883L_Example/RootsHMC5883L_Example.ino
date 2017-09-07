/* ----------------------------------------------------------------------------

File:  RootsHMC5883L_Example.ino
Author: Guilherme Gomes & Lorenzo Giordano
Date: 02/Sep/2017

Description:
  Example of using the RootsHMC5883L API.


Commands:


---------------------------------------------------------------------------- */

#include <Arduino.h>
#include <RootsHMC5883L.h> // will include <Wire.h>


//RootsHMC5883L compass(7); // creates compass instance with ready pin listener (single ready mode)
RootsHMC5883L compass; // creates compass instance for continuos read mode
int count; // time counter
float ref, delta; // angle variables

void setup()
{
  Serial.begin(9600);
  
  compass.setSamplingMode(MODE_CONTINUOUS);
  //compass.setSamplingMode(MODE_SINGLE);
  compass.setGain(GAIN_09); // precision

  count = 0;
  ref = 0;
}

void loop()
{
   compass.updateMeasures();
   
   float heading = compass.getOffsetMeasure();
   //float incl = Compass.getXAngle();

   delta = compass.getDeltaDegrees(ref, heading);
   
   Serial.print("Heading: \t");
   Serial.print( heading );
   Serial.print("\t Delta: \t");
   Serial.println( delta );      
   delay(1000);
   
   count++;
   if(count == 5) compass.updateOffset();

   if(count == 20) ref = 30;
   if(count == 40) ref = -30;
   if(count == 60) ref = 0;
   
}
