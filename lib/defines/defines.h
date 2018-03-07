#ifndef DEFINES_H
#define DEFINES_H

#include <avr/io.h>
#include <adc.h>

#define MAX_DUTY 255
#define MIN_DUTY 255*0.01
#define VOLT_DIV 5.94*BIT_DIV
#define ACS_MIN 102
#define ACS_MAX 930


// temporary variables that needed to be global
volatile uint8_t i = 0;
volatile uint8_t j = 0;
volatile uint16_t volt_sum = 0;
volatile uint16_t curr_sum = 0;
float VCC = 0;
float ACS_OFFSET;
float SENSITIVITY;
float BIT_DIV;

#endif
