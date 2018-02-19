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
volatile uint16_t ADC_VALUE;
volatile uint8_t ADCLOW;

volatile uint8_t DUTY = 0;
volatile uint16_t SETPOINT;

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
      if (DUTY <= MIN_DUTY){
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
  static uint8_t firstTime = 1;

  ADCLOW = ADCL;
  ADC_VALUE = ADCH<<8 | ADCLOW;

  if (firstTime == 1){
    firstTime = 0;
  }
  else if (ADMUX == 64){
    SETPOINT = ADC_VALUE;
    ADMUX = 65;
  }
  else if (ADMUX == 65){
    OUTPUT_VOLTAGE = ADC_VALUE;
    ADMUX = 64;
  }
}
