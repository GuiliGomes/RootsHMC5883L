/* ----------------------------------------------------------------------------

File:	RootsCompass.h
Author:	Guilherme Gomes & Lorenzo Giordano
Date:	21/Oct/2016

Description:
  Two zeros two poles PID controller.


Constructor paramters:
 - dT: periodo de amostragem
 - Kp:
 - Ki:
 - Kd:

---------------------------------------------------------------------------- */

#ifndef __ROOTS_HMC5883L
#define __ROOTS_HMC5883L

#define __MEASURE_X
//#define __MEASURE_Y


// math
#define ROOTS_PI 3.14


// device's address
#define HMC5883L_I2C_ADDRESS  0x3C >> 1


// config register A (sample rate)
#define HMC5883L_CONFIG_REGISTER_A 0x00

#define RATE_075 0x00
#define RATE_1_5 0x01
#define RATE_3   0x02
#define RATE_7_5 0x03
#define RATE_15  0x04
#define RATE_30  0x05
#define RATE_75  0x06
// config register A

// config register B (gain)
#define HMC5883L_CONFIG_REGISTER_B 0x01

#define GAIN_09 0x00
#define GAIN_12 0x01
#define GAIN_19 0x02
#define GAIN_25 0x03
#define GAIN_40 0x04
#define GAIN_46 0x05
#define GAIN_55 0x06
#define GAIN_79 0x07 
// config register B

// mode register (operating mode)
#define HMC5883L_MODE_REGISTER 0x02

#define MODE_CONTINUOUS 0x00
#define MODE_SINGLE     0x01
#define MODE_IDLE       0x02
// mode register

// data register
#define HMC5883L_DATA_REGISTER 0x03



class RootsHMC5883L {
	
  public:
    
    // default contructor
    RootsHMC5883L();

    // constructor with ready pin listener
    RootsHMC5883L(uint8_t ready);
    
    // methods
    void  setGain(uint8_t gain);
    void  setSamplingRate(uint8_t rate);
    void  setSamplingMode(uint8_t sampling_mode);
    float getMeasure();
    void  updateMeasures();
    void  updateOffset(); 
    float getOffsetMeasure();
    float getDeltaRadians(float angle1, float angle2);
    float getDeltaDegrees(float angle1, float angle2);
    
    #ifdef __MEASURE_Y
    float getYAngle();
    #endif

    #ifdef __MEASURE_X
    float getXAngle();
    #endif
    
    
  protected:
    
    // properties
    struct MeasureSample
    {
      int x;
      int y;
      int z;
    };

    MeasureSample sample;
    
    // default compass
    float z_angle, z_offset;

    #ifdef __MEASURE_Y
    float y_angle, y_offset;
    #endif

    #ifdef __MEASURE_X
    float x_angle, x_offset;
    #endif
    
    // methods
    void    i2cWrite(uint8_t address, uint8_t byte);
    uint8_t i2cRead(uint8_t address, uint8_t buffer[], uint8_t length);
    void    readAxes();

     
  private:

     uint8_t i2cAddress, samplingMode, readyPin;
};

#endif
