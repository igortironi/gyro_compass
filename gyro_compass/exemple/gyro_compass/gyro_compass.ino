#include <gyro_compass.h>


gyro_compass sensor;


void setup() {
  // put your setup code here, to run once:

  sensor.begin();
  sensor.gyro_filter(4);
  sensor.gyro_scale(0);
  
  Serial.begin(115200);

  
  //Serial.print("Gyro offset: ");
  //Serial.println(sensor.gyro_get_offset());
  /*
  delay(100);
  sensor.compass_get_offset();
  Serial.print("X offset: ");
  Serial.print(sensor.compass_xoffset());
  Serial.print(" --- Y offset: ");
  Serial.print(sensor.compass_yoffset());
  while(1);
  */
  
  sensor.gyro_set_offset(-250.58);
  sensor.compass_set_xoffset(13);
  sensor.compass_set_yoffset(-18);
  //sensor.mixing_ratio(0.01);
  
  

}

void loop() {
    //static float gyro = 0;
    //gyro = sensor.gyro_angle(gyro);
    Serial.print("angulo composto: ");
    Serial.println(sensor.angle());
    
}
