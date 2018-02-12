/**
 * File: main.c
 * Author: Isaac Rowell
 * Date:
 */
#include <avr/io.h>
#include <pwm.h>

#define BVV(bit, val) ((val)?_BV(bit):0)
#define PWM_FREQUENCY 0X013F

int main (void)
{
  uint16_t DUTY = 0X0020;
  pwm_init();
  pwm_set_duty(DUTY, PWM_FREQUENCY);
}
