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

#define MAX_DUTY OCR0A

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
    _delay_ms(1);
    if (OUTPUT_VOLTAGE > SETPOINT){
      DUTY++;
      pwm_set(DUTY);
    }
    else if (OUTPUT_VOLTAGE < SETPOINT){
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
