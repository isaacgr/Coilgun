#ifndef DEFINES_H
#define DEFINES_H
#include <avr/io.h>

#define MAX_DUTY 255
#define MIN_DUTY 255*0.01
#define DIVIDER 5.94*5.0/1023.0

// temporary variables that needed to be global
volatile uint8_t i = 0;
volatile uint16_t sum = 0;

#endif
