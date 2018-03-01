#ifndef DEFINES_H
#define DEFINES_H
#include <avr/io.h>

#define MAX_DUTY 255
#define MIN_DUTY 255*0.01
#define VOLT_DIV 5.94*BIT_DIV
#define ACS_OFFSET 2.49  //acs712 voltage offset
#define CURR_DIV 0.066 // mV/A
#define BIT_DIV 5.0/1023.0

// temporary variables that needed to be global
volatile uint8_t i = 0;
volatile uint8_t j = 0;
volatile uint16_t sum = 0;
volatile uint16_t curr_sum = 0;

#endif
