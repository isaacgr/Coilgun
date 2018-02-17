/**
 * File: pwm.cpp
 * Author: Isaac Rowell
 * Date:
 */

#include <avr/io.h>
#include "pwm.h"

#define BVV(bit, val) ((val)?_BV(bit):0)

int pwm_set(uint16_t DUTY)
{
  OCR0B = DUTY; // OCR0B defines the duty cycle

  return 0;
}

int pwm_init(void)
{
	DDRD |= _BV(DDD5); //set pin 5 of PORTD for output (OC0B)
  OCR0A = 255; // set TOP value for PWM

  // fast pwm, clear 0C0B on match
  TCCR0A = (1<<COM0B1) | (1<<WGM00) | (1<<WGM01);
  TCCR0B = (1<<CS00) | (1<<WGM02); // no prescaler

  return 0;
}
