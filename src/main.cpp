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
#include <defines.h>

volatile uint16_t OUTPUT_VOLTAGE;
volatile uint8_t OUTPUT_VOLTAGE_LSB;
volatile uint8_t DUTY = 0;
const uint16_t SETPOINT = 512;

int main(void)
{
  Serial.begin(115200);
  pwm_init();
  pwm_set(DUTY);
  adc_init();
  sei();

  while(1) // infinite loop
  {
    if (OUTPUT_VOLTAGE > SETPOINT){
      if (DUTY >= MAX_DUTY){
        pwm_set(MAX_DUTY);
        continue;
      }
      DUTY++;
      pwm_set(DUTY);
    }
    else if (OUTPUT_VOLTAGE < SETPOINT){
      if (DUTY
        <= MIN_DUTY){
        pwm_set(MIN_DUTY);
        continue;
      }
      DUTY--;
      pwm_set(DUTY);
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
