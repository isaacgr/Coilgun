/**
 * File: adc.cpp
 * Author: Isaac Rowell
 * Date:
 */
#include <avr/io.h>
#include <util/delay.h>
#include "adc.h"

int adc_init(void)
{
  // adc will run in free running single conversion mode
  // a new conversion will have to be started after one finishes

  ADMUX = 0;  // use ADC0
  DIDR0 |= (1<<ADC0D) | (1<<ADC1D); // disable the digial inputs on the adc
  ADMUX |= (1<<REFS0);  // AVcc as the reference
  ADMUX |= (0<<ADLAR);  // left justified, 10bit resolution

  ADCSRA |= (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0); // 128 prescaler
  //ADCSRA |= (1<<ADATE); // enable auto trigger

  ADCSRB = 0; // free running mode

  ADCSRA |= (1<<ADIE); // enable interrupts
  ADCSRA |= (1<<ADEN); // enable adc
  ADCSRA |= (1<<ADSC); // start conversion

  return 0;
}

long readVcc(void)
{
  // Read 1.1V reference against AVcc
  // set the reference to Vcc and the measurement to the internal 1.1V reference
  uint8_t tmp;
  tmp = ADMUX;

  ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  _delay_ms(2); // wait for vref to settle
  ADCSRA |= _BV(ADSC); // Start conversion
  while (bit_is_set(ADCSRA,ADSC)); // measuring

  uint8_t low  = ADCL; // must read ADCL first - it then locks ADCH
  uint8_t high = ADCH; // unlocks both

  long result = (high<<8) | low;

  result = 1125300L / result; // Calculate Vcc (in mV); 1125300 = 1.1*1023*1000

  ADMUX = tmp;  // reset ADMUX to value set by adc_init()
  ADCSRA |= (1<<ADSC); // start conversion

  return result; // Vcc in millivolts
}
