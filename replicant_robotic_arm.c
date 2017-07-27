#include <16F873A.h>
#device  adc = 10
#fuses   XT
#use     delay(clock = 4M)

long     sensor_pulgar = 0;

#define SERVO_DEDO_PULGAR  48 //Servo que controla el dedo pulgar

#include <servo.h>

void main(){
  set_tris_a(0x01);
  set_tris_b(0x00);
  setup_adc(adc_clock_internal);
  setup_adc_ports(AN0);
  Servo_init(48); //Coloca a +-90° los servos declarados

  while(true){
    set_adc_channel(0); //RA0 sensor del dedo pulgar
    delay_us(20);
    sensor_pulgar = getc_adc();
    sensor_pulgar = map(sensor_pulgar, 511, 1023, 0, 180);
    Servo_PWM(SERVO_DEDO_PULGAR, sensor_pulgar);
  }
}