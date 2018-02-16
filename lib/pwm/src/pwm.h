/**
 * File: pwm.h
 * Author: Isaac Rowell
 * Date:
 */

#ifndef PWM_H
#define PWM_H
#include "stdint.h"

int pwm_set(uint16_t DUTY, uint16_t FREQUENCY); // set the duty cycle and frequency
int pwm_init(void); // initialize the pwm modue, return 0 on success

#endif
