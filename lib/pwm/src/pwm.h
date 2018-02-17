/**
 * File: pwm.h
 * Author: Isaac Rowell
 * Date:
 */

#ifndef PWM_H
#define PWM_H
#include "stdint.h"

int pwm_set(uint16_t DUTY); // set the duty cycle
int pwm_init(void); // initialize the pwm modue, return 0 on success

#endif
