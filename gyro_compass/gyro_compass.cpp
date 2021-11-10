#include "gyro_compass.h"
#include "arduino.h"

#include <Wire.h>

gyro_compass::gyro_compass(){
}


byte gyro_compass::begin(){
    Wire.begin();

    if( i2c_send(GYRO_ADDR, PWR_MGMT_1, 0) ) return 1;

    if( i2c_send(0x1E, 0x00, 0x7C) ) return 1; //8-average, 15 Hz default, normal measurement.
    if( i2c_send(0x1E, 0x01, 0xA0) ) return 1; //Gain=2.
    if( i2c_send(0x1E, 0x02, 0x00) ) return 1; //Continuous-measurement mode.

    return 0;
}

float gyro_compass::Gyro_read(){
    float data;

    data = (float)request_data(GYRO_ADDR, GYRO_ZOUT_H) - Gyro_offset;
    return (data / Gyro_scale);
}

float gyro_compass::gyro_angle(float angle){
    static unsigned long timer = micros();

    angle += Gyro_read() * ((float)(micros() - timer) / 1000000.0);
    timer = micros();

    return angle;

}


float gyro_compass::gyro_get_offset(){
    float offset = 0;

    for(float x = 1 ; x <= 10000 ; x++){
        offset += ((float)request_data(GYRO_ADDR, GYRO_ZOUT_H) - offset) / x;
    }

    Gyro_offset = offset;

    return offset;
}


void gyro_compass::gyro_set_offset(float offset){
    Gyro_offset = offset;
}



 void gyro_compass::gyro_scale(int8_t scale){
    if(scale>=0 && scale<=3){
        i2c_send(GYRO_ADDR, GYRO_CONFIG, scale<<3);

        switch (scale){
        case 0:
            Gyro_scale = 131.0;
        break;
        case 1:
            Gyro_scale = 65.5;
        break;
        case 2:
            Gyro_scale = 32.8;
        break;
        case 3:
            Gyro_scale = 16.4;
        break;
        }
    }


 }


 void gyro_compass::gyro_filter(int8_t data){
    if(data>=0 && data<=6){
        i2c_send(GYRO_ADDR, CONFIG, data);
    }
 }


byte gyro_compass::i2c_send(int8_t addr, int8_t regis, int8_t data){

  Wire.beginTransmission(addr);
  Wire.write(regis);
  Wire.write(data);
  return(Wire.endTransmission());

}


int gyro_compass::request_data(int8_t addr, int8_t regis){

    if(Wire.available()){
      while( Wire.available() ) Wire.read();                              //checks i2c buffers for old data.
    }

    Wire.beginTransmission(addr);
    Wire.write(regis);
    Wire.endTransmission(false);
                                                                         //Sets first data register.
    Wire.requestFrom(addr, 2);                                           //request sensor data.

    unsigned long timeout = millis();
    while( (Wire.available() < 2 ) || ( (millis() - timeout) > 6) );    //waits for 6 bytes of data or timeout timer, whichever comes first.
    if(millis() - timeout > 6)return 0;

    int value = (Wire.read()<<8) | Wire.read();

    return value;
}


void gyro_compass::compass_gain(int8_t value){
    if(value >= 0 && value <= 7){
          value = 7 - value;
          i2c_send(COMPASS_ADDR, 0x01, (value << 5));
    }
}


void gyro_compass::compass_get_offset(){
    int max_x = -30000, max_y = -30000;
    int min_x = 30000, min_y = 30000;
    unsigned long timer = millis();
    int x = 0, y = 0;

    while((millis() - timer) < 10000){
        x = request_data(COMPASS_ADDR, 0x03);
        request_data(COMPASS_ADDR, 0x05);
        y = request_data(COMPASS_ADDR, 0x07);
        max_x = max(max_x, x);
        min_x = min(min_x, x);

        max_y = max(max_y, y);
        min_y = min(min_y, y);
    }

    Compass_x_offset = (max_x + min_x)/2;
    Compass_y_offset = (max_y + min_y)/2;

}
int gyro_compass::compass_xoffset(){
    return Compass_x_offset;
}

int gyro_compass::compass_yoffset(){
    return Compass_y_offset;
}



void gyro_compass::compass_set_xoffset(int offset){
    Compass_x_offset = offset;
}

void gyro_compass::compass_set_yoffset(int offset){
    Compass_y_offset = offset;
}


float gyro_compass::compass_angle(){
    int x = request_data(COMPASS_ADDR, 0x03) - Compass_x_offset;
    request_data(COMPASS_ADDR, 0x05);
    int y = request_data(COMPASS_ADDR, 0x07) - Compass_y_offset;

    float angle = degrees(atan2(x, y));

    return angle;

}


float gyro_compass::angle(){
    float compass = compass_angle();
    static float gyro = compass;
    gyro = gyro_angle(gyro);

    if(gyro > 180.0) gyro = -180.0;
    else if(gyro < -180.0) gyro = 180.0;

    if((gyro - compass) > 180.0 || (gyro - compass) < -180.0)gyro -= (compass - gyro) * Mix;
    else gyro += (compass - gyro) * Mix;

    return gyro;

}


void gyro_compass::mixing_ratio(float mix){
    Mix = mix;
}
