///////////////////////////////////////////////////////////////////////////
////      Librería para controlar servomotores con PWM discreto        ////
////                                                                   ////
////   *Nota: Este driver recurre a delay_us() para generar los pulsos ////
////          y hace uso exclusivamente del PUERTO (B) en adelante     ////
////          siendo limitado por el modelo del PIC a utilizar.        ////
////                                                                   ////
////   Edite las lineas:                                               ////
////           - #define REFRESH_INTERVAL                              ////
////           - #define MAX_PULSE_WIDTH                               ////
////           - #define DEFAULT_PULSE_WIDTH                           ////
////           - #define MIN_PULSE_WIDTH                               ////
////           - #define PIN_CONTROL_MIN                               ////
////                                                                   ////
////   Para variar la duración de los pulsos en alto y bajo que        ////
////   controlará la posición en grado de los servomotores.            ////
////                                                                   ////
////   Antes de incluir la librería <servo.h> se debe declarar         ////
////   obligatoriamente haciendo el uso de #define para asignar nombre ////
////   y ubicación específica del PIN a controlar.                     ////
////                                                                   ////
////   Ejemplo:                                                        ////
////     #define NOMBRE  NUMERO_DEL_PIN                                ////
////                                                                   ////
////   NOMBRE: es cualquier identificación        #define PIN_B0  48   ////
////           que desee.                         #define PIN_B1  49   ////
////                                              #define PIN_B2  50   ////
////   NUMERO_DEL_PIN: es la ubicación reservada  #define PIN_B3  51   ////
////                   de los pines de los        #define PIN_B4  52   ////
////                   puertos que indica el      #define PIN_B5  53   ////
////                   compilador CCS.            #define PIN_B6  54   ////
////                                              #define PIN_B7  55   ////
////                                                                   ////
////                                                                   ////
////   void Servo_init(int pin_control_max)                            ////
////     Esta funcición se utiliza para ubicar los servos definidos    ////
////     en una posición inicial, la cual es 90°.                      ////
////                                                                   ////
////   Ejemplo:                                                        ////
////     Servo_init(50); Este llamado hará que los pines RB0 a RB2     ////
////                     envien ordenes para ubicar los servos a 90°   ////
////                     inicialmente.                                 ////
////                                                                   ////
////   long map(long value, long in_min, long in_max, long out_min,    ////
////            long out_max)                                          ////
////     Se usa para mapear la salida en grados en función al valor    ////
////     leido de entrada.                                             ////
////                                                                   ////
////   Ejemplo:                                                        ////
////     map(valor leido, 0, 1023, 0, 180); Lee valores de (0 - 1023)  ////
////                                        y devuelve valores entre   ////
////                                        (0 - 180).                 ////
////                                                                   ////
////   void Servo_PWM(long pin_control, long out_interval)             ////
////        Esta función se encarga de interactuar con el PIN donde    ////
////        estará conectado el servo en intervalos de modulación de   ////
////        ancho de pulso para posicionarlo en el ángulo indicado.    ////
////                                                                   ////
////   Ejemplo:                                                        ////
////     Servo_PWM(PIN, Grados); Al realizar este llamado ubicará al   ////
////                             PIN definido en la posición que tenga ////
////                             el registro "Grados"                  ////
////                                                                   ////
////   La librería se puede utilizar tanto para 8 como 10 bit de       ////
////   resolución del ADC.                                             ////
////                                                                   ////
///////////////////////////////////////////////////////////////////////////
////                       Jesús E. Marín L.                           ////
////                  Venezuela - Sucre - Cumaná                       ////
///////////////////////////////////////////////////////////////////////////

#ifndef servo_h
#define servo_h

#define REFRESH_INTERVAL     20000 // duración total del pulso equivalente a 50Hz
#define MAX_PULSE_WIDTH      2350  // valor de tiempo equivalente a 180°
#define DEFAULT_PULSE_WIDTH  1500  // valor de tiempo equivalente a 90°
#define MIN_PULSE_WIDTH      535   // valor de tiempo equivalente a 0°
#define PIN_CONTROL_MIN      48    // Pin de control minimo por defecto

long    value_new = 0, result = 0, angle_max = 0, angle_min = 0;

///////////////////////////////////////////////////////////////////////////
////                   Inicialización de los servos                    ////
///////////////////////////////////////////////////////////////////////////
void Servo_init(int pin_control_max){
  delay_ms(2000);
  for(int number_pin = PIN_CONTROL_MIN; number_pin <= pin_control_max; number_pin++){
    for(int duration_time = 0; duration_time < 150; duration_time++){
      output_high(number_pin);
      delay_us(DEFAULT_PULSE_WIDTH);
      output_low(number_pin);
      delay_us(REFRESH_INTERVAL - DEFAULT_PULSE_WIDTH);
    }
  }
}

///////////////////////////////////////////////////////////////////////////
////                   Lectura Analogo-Digital                         ////
///////////////////////////////////////////////////////////////////////////
long getc_adc(){
  result = 0;
  for(int i = 0; i <= 10; i++){
    value_new = read_adc();
    result = result + value_new;
    delay_ms(1);
  }
  return result/10;
}

///////////////////////////////////////////////////////////////////////////
////      Ajusta el rango de salida en función de la entrada           ////
///////////////////////////////////////////////////////////////////////////
long map(long value, long in_min, long in_max, long out_min, long out_max){

  angle_max = out_max;
  angle_min = out_min;

  if(value < in_min){
    return out_min;
  }
  else if(value > in_max){
    return out_max;
  }

  else if((in_max - in_min) > (out_max - out_min)){
    return (value - in_min) * (float)(out_max - out_min + 1) / (in_max - in_min + 1) + out_min;
  }
  else{
    return (value - in_min) * (float)(out_max - out_min) / (in_max - in_min) + out_min;
  }
}

///////////////////////////////////////////////////////////////////////////
////                  Modulación del Ancho del Pulso                   ////
///////////////////////////////////////////////////////////////////////////
void Servo_PWM(long pin_control, long out_interval){
  out_interval = MIN_PULSE_WIDTH + (float)(MAX_PULSE_WIDTH - MIN_PULSE_WIDTH) / (angle_max - angle_min) * (out_interval);
  output_high(pin_control);
  delay_us(out_interval);
  output_low(pin_control);
  delay_us(REFRESH_INTERVAL - out_interval);
}
#endif