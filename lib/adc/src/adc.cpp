/**
 * File: adc.cpp
 * Author: Isaac Rowell
 * Date:
 */
#include <avr/io.h>
#include "adc.h"

int adc_init(void)
{
  ADMUX = 0;  // use ADC0
  ADMUX |= (1<<REFS0);  // AVcc as the reference
  ADMUX |= (0<<ADLAR);  // left justified, 10bit resolution

  ADCSRA |= (0<<ADPS2) | (1<<ADPS1) | (0<<ADPS0); // 4 prescaler
  ADCSRA |= (1<<ADATE); // enable auto trigger

  ADCSRB = 0; // free running mode

  ADCSRA |= (1<<ADIE); // enable interrupts
  ADCSRA |= (1<<ADEN); // enable adc
  ADCSRA |= (1<<ADSC); // start conversion

  return 0;
}
