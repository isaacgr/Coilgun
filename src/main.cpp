/**
 * File: main.cpp
 * Author: Isaac Rowell
 * Date:
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include <Arduino.h>
#include <pwm.h>
#include <adc.h>

#define PWM_FREQUENCY 0X013F  //50kHZ with Timer1
#define SETPOINT 512
#define MAX_DUTY 65535

volatile uint8_t OUTPUT_VOLTAGE_LSB;
volatile uint16_t OUTPUT_VOLTAGE;
volatile uint16_t DUTY = 0;

int main(void)
{
  Serial.begin(115200);
  pwm_init();
  pwm_set(DUTY, PWM_FREQUENCY);
  adc_init();
  sei();
  while(1) // infinite loop
  {
    if (OUTPUT_VOLTAGE < SETPOINT){
      DUTY++;
      pwm_set(DUTY, PWM_FREQUENCY);
      _delay_ms(100);
      while (DUTY >= MAX_DUTY){
        continue;
      }
    }
    else if (OUTPUT_VOLTAGE > SETPOINT){
      DUTY--;
      pwm_set(DUTY, PWM_FREQUENCY);
      _delay_ms(100);
      while(DUTY == 0){
        continue;
      }
    }
    else {
      continue;
    }
  }
}

ISR(ADC_vect)
{
  OUTPUT_VOLTAGE_LSB = ADCL;
  OUTPUT_VOLTAGE = ADCH<<8 | OUTPUT_VOLTAGE_LSB;
}
