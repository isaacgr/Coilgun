#ifndef DEFINES_H
#define DEFINES_H

#include <avr/io.h>
#include <adc.h>

#define MAX_DUTY 255
#define MIN_DUTY 1
#define VOLT_DIV 11*BIT_DIV
#define ACS_MIN 102
#define ACS_MAX 930

// temporary variables that needed to be global
volatile uint8_t i = 0;
volatile uint8_t j = 0;
volatile uint16_t volt_sum = 0;
volatile uint16_t curr_sum = 0;
const int voltage_arr = 32;
const int curr_arr = 16;


// initialize VCC and variables that rely on it
float VCC = 0;
float ACS_OFFSET;
float SENSITIVITY = 0.066;
float BIT_DIV;

#endif
