/**
 * File: timer.cpp
 * Author: Isaac Rowell
 * Date:
 */

#include <avr/io.h>
#include "timer.h"

#define BVV(bit, val) ((val)?_BV(bit):0)

int pwm_set(uint16_t DUTY)
{
  OCR0A = DUTY; // OCR0A defines the duty cycle

  return 0;
}

int pwm_init(void)
{
	DDRD |= _BV(DDD6); //set pin 6 of PORTD for output (OC0A)
  //OCR0A = 255; // set TOP value for PWM

  // fast pwm, clear 0C0A on match
  TCCR0A |= (1<<COM0A1) | (1<<WGM00) | (1<<WGM01);
  TCCR0B |= (1<<CS00) | (0<<WGM02); // no prescaler

  return 0;
}

int timer2_init(void){

  TCCR2A |= (1<<WGM21);
  OCR2A = 0XF9;
  TIMSK2 |= (1<<OCIE2A);
  TCCR2B |= (1<<CS22) | (1<<CS21);

  return 0;
}
