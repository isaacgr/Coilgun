/**
 * File: pwm.cpp
 * Author: Isaac Rowell
 * Date:
 */


#include <avr/io.h>
#include "pwm.h"

#define BVV(bit, val) ((val)?_BV(bit):0)

void pwm_set_duty(uint16_t duty, uint16_t frequency)
{
  ICR1 = frequency;
  OCR1A = duty;
}

void pwm_init()
{
	DDRB |= _BV(DDB1); /* set pin 6 of PORTD for output*/

  TCCR1A =
    BVV(WGM10, 0) | BVV(WGM11, 1) /* Fast PWM update on OCRA */
  | BVV(COM1A1, 1) | BVV(COM1A0, 0) /* non-inverting OC0A */
  | BVV(COM1B1, 0) | BVV(COM1B0, 0); /* OC0B not connected */

  TCCR1B =
    BVV(CS10, 1) | BVV(CS11, 0) | BVV(CS12, 0) /* F_CPU/1024 */
  | BVV(WGM12, 1) | BVV(WGM13, 1); /* Fast PWM update on OCRA */
}
