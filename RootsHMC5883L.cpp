/* ----------------------------------------------------------------------------

File:	RootsPID.cpp
Author:	Guilherme Gomes & Lorenzo Giordano
Date:	07/Jun/2017

Description:
  An API to HMC5883L magnetometer.


Commands:


---------------------------------------------------------------------------- */

#include <Arduino.h>
#include <Wire.h>
#include <RootsHMC5883L.h>



RootsHMC5883L::RootsHMC5883L() {

  Wire.begin();
  i2cAddress = HMC5883L_I2C_ADDRESS;
}


RootsHMC5883L::RootsHMC5883L(uint8_t ready) {

  RootsHMC5883L();
  readyPin = ready;
  pinMode(readyPin, INPUT);
}


void RootsHMC5883L::setGain(uint8_t gain) {

  i2cWrite(HMC5883L_CONFIG_REGISTER_B, (gain & 0x07) << 5);
}



void RootsHMC5883L::setSamplingRate(uint8_t rate) {

  i2cWrite(HMC5883L_CONFIG_REGISTER_A, (rate & 0x07) << 2);
}



void RootsHMC5883L::setSamplingMode(uint8_t sampling_mode) {

  i2cWrite(HMC5883L_MODE_REGISTER, sampling_mode & 0x03);
  samplingMode = sampling_mode & 0x03;
}



float RootsHMC5883L::getMeasure() {

  return z_angle * 180.0/ROOTS_PI; 
}


float RootsHMC5883L::getOffsetMeasure() {

  return 180.0/ROOTS_PI * getDeltaRadians(z_angle, z_offset); 
}



#ifdef __MEASURE_X
float RootsHMC5883L::getXAngle() {

  return x_angle * 180.0/ROOTS_PI; 
}
#endif



#ifdef __MEASURE_Y
float RootsHMC5883L::getYAngle() {

  return y_angle * 180.0/ROOTS_PI; 
}
#endif



void RootsHMC5883L::updateMeasures() {

  readAxes();
  
  // Z axis rotation, azimuth, heading....
  z_angle = atan2(sample.y, sample.x);

  // X axis rotation
  #ifdef __MEASURE_X
  x_angle = atan2(sample.z, sample.y);
  #endif

  // Y axis rotation
  #ifdef __MEASURE_Y
  y_angle = atan2(sample.z, sample.x);
  #endif
}


void RootsHMC5883L::updateOffset() {

  z_offset = z_angle;

  #ifdef __MEASURE_X
  x_offset = x_angle;
  #endif

  #ifdef __MEASURE_Y
  y_offset = y_angle;
  #endif  
}


float RootsHMC5883L::getDeltaRadians(float angle1, float angle2) {

  angle1 -= angle2;

  if (angle1 <= - ROOTS_PI) {
    angle1 += 2.0 * ROOTS_PI;
  }
  else if (angle1 > ROOTS_PI) {
    angle1 -= 2.0 * ROOTS_PI;
  }

  return angle1;
}


float RootsHMC5883L::getDeltaDegrees(float angle1, float angle2) {

  return getDeltaRadians((angle1 * ROOTS_PI/180.0), (angle2 * ROOTS_PI/180.0)) * (180.0/ROOTS_PI);
}


void RootsHMC5883L::readAxes() {
	
  uint8_t buffer[6];
  
  if(samplingMode & MODE_SINGLE) {    
    
    i2cWrite(HMC5883L_MODE_REGISTER, MODE_SINGLE);  
    
    // ready pin listener
    do {
      delay(50);
    } while(!digitalRead(readyPin));
  }
  
  i2cRead(HMC5883L_DATA_REGISTER, buffer, 6);
  
  sample.x = (buffer[0] << 8) | buffer[1];  
  sample.z = (buffer[2] << 8) | buffer[3];
  sample.y = (buffer[4] << 8) | buffer[5];
}


void RootsHMC5883L::i2cWrite(uint8_t register_address, uint8_t data) {
  
  Wire.beginTransmission(i2cAddress);
  Wire.write(register_address);
  Wire.write(data);
  Wire.endTransmission();
}


uint8_t RootsHMC5883L::i2cRead(uint8_t register_address, uint8_t buffer[], uint8_t length) {
	
  Wire.beginTransmission(i2cAddress);
  Wire.write(register_address);
  Wire.endTransmission();
  
  Wire.requestFrom(i2cAddress, length);

  if(Wire.available() == length) {

    for(uint8_t i = 0; i < length; i++) {
      buffer[i] = Wire.read();
    }
    
    return length;
  }

  return 0;
}
