/**
 * File: pwm.h
 * Author: Isaac Rowell
 * Date:
 */

#ifndef PWM_H
#define PWM_H
#include "stdint.h"

void pwm_set_duty(uint16_t duty, uint16_t frequency);
void pwm_init();

#endif
