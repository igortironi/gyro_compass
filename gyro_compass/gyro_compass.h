#ifndef gyro_compass_h
#define gyro_compass_h

#include "arduino.h"


#define PWR_MGMT_1 0x6B
#define GYRO_CONFIG 0x1B
#define ACCEL_CONFIG 0x1C
#define CONFIG 0x1A
#define GYRO_ADDR 0x68
#define ACCEL_XOUT_H 0x3B
#define ACCEL_YOUT_H 0x3D
#define ACCEL_ZOUT_H 0x3F
#define GYRO_XOUT_H 0x43
#define GYRO_YOUT_H 0x45
#define GYRO_ZOUT_H 0x47


#define COMPASS_ADDR 0x1E

class gyro_compass{
public:
    gyro_compass();
    byte begin();
    void gyro_scale(int8_t scale);
    void gyro_filter(int8_t data);
    float gyro_get_offset();
    void gyro_set_offset(float offset);
    float gyro_angle(float angle);
    void compass_gain(int8_t value);
    void compass_get_offset();
    int compass_xoffset();
    int compass_yoffset();
    void compass_set_xoffset(int offset);
    void compass_set_yoffset(int offset);
    float compass_angle();
    float angle();
    void mixing_ratio(float mix);


private:
    byte i2c_send(int8_t addr, int8_t regis, int8_t data);
    int request_data(int8_t addr, int8_t regis);
    float Gyro_read();
    float Gyro_scale = 131.0;
    float Gyro_offset = 0;
    int Compass_x_offset = 0, Compass_y_offset = 0;
    float Mix = 0.001;

};




#endif // GYRO_COMPASS_H_INCLUDED
