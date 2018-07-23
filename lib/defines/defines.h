#ifndef DEFINES_H
#define DEFINES_H

#include <avr/io.h>
#include <adc.h>

/*****************
DEFINITIONS
******************/
#define MAX_DUTY 255
#define MIN_DUTY 1
#define VOLT_DIV 11*BIT_DIV
#define ACS_MIN 102
#define ACS_MAX 930
#define DIRECT 0
#define REVERSE 1
#define BOOST_ON 1
#define BOOST_OFF 0

/*****************
TEMPORARY VARIABLES
******************/
// temporary variables that needed to be global
volatile uint8_t i = 0;
volatile uint8_t j = 0;
uint16_t volt_sum = 0;
uint16_t curr_sum = 0;
const int voltage_arr = 16;
const int curr_arr = 16;

/*****************
VOLATILE VARIABLES
******************/
volatile uint16_t OUTPUT_VOLTAGE_ARRAY[voltage_arr];
volatile uint16_t ADC_VALUE;
volatile uint8_t ADCLOW;
volatile uint16_t CURRENT_RAW[curr_arr];
volatile float CURRENT;
volatile uint8_t TIMER_DELAY = 0;
volatile uint8_t PAGE = 0;
volatile uint16_t millisec = 0;
volatile float OUTPUT_VOLTAGE;
volatile float SETPOINT;
volatile float VIN;

/*****************
PID VARIABLES
******************/
uint8_t DUTY;
float kp, ki;
double kd;
double errSum, lastInput;
unsigned long lastTime;

/*****************
BOOST VARIABLES
******************/
bool boosting;

/*****************
VCC VARIABLES
******************/
// initialize VCC and variables that rely on it
float VCC = 0;
float ACS_OFFSET;
float SENSITIVITY = 0.066;
float BIT_DIV;

#endif
