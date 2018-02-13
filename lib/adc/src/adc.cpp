/**
 * File: adc.cpp
 * Author: Isaac Rowell
 * Date:
 */
#include <avr/io.h>
#include "adc.h"

int adc_init(void)
{
  ADMUX = 0;  //USE ADC0
  ADMUX |= (1<<REFS0);  //AVcc as the reference
  ADMUX |= (0<<ADLAR);  // left justified, 10bit resolution

  ADCSRA |= (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);
  ADCSRA |= (1<<ADATE);

  ADCSRB = 0;

  ADCSRA |= (1<<ADIE);
  ADCSRA |= (1<<ADEN);
  ADCSRA |= (1<<ADSC);

  return 0;
}
