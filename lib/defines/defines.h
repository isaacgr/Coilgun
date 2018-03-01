#ifndef DEFINES_H
#define DEFINES_H
#include <avr/io.h>

#define MAX_DUTY 255
#define MIN_DUTY 255*0.01
#define VOLT_DIV 5.94*5.0/1023.0
#define ACS_OFFSET 2.5  //acs712 voltage offset
#define CURR_DIV ((5.0-ACS_OFFSET)/(1023.0*0.066)) // mV/A

// temporary variables that needed to be global
volatile uint8_t i = 0;
volatile uint16_t sum = 0;

#endif
