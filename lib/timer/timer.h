/**
 * File: pwm.h
 * Author: Isaac Rowell
 * Date:
 */

#ifndef TIMER_H
#define TIMER_H
#include "stdint.h"

int pwm_set(uint16_t DUTY); // set the duty cycle
int pwm_init(void); // initialize the pwm modue, return 0 on success
int lcd_timer_init(void);
void millisec_init(void); // get the number of milliseconds

#endif
