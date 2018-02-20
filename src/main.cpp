/**
 * File: main.cpp
 * Author: Isaac Rowell
 * Date:
 */

#include <avr/io.h>
#include <avr/interrupt.h>
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
    //Serial.printf("SETPOINT %d\nOUTPUT: %d\n", SETPOINT, OUTPUT_VOLTAGE);
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
  static uint8_t tmp;
  tmp = ADMUX;
  tmp &= 0X0F;

  ADCLOW = ADCL;
  ADC_VALUE = ADCH<<8 | ADCLOW;

  switch (tmp) {
    case 0:
      ADMUX++;
      OUTPUT_VOLTAGE = ADC_VALUE;
      break;
    case 1:
      ADMUX &= 0XF8;
      SETPOINT = ADC_VALUE;
      break;
  }
  ADCSRA |= 1<<ADSC;
}
