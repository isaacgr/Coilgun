/**
 * File: adc.h
 * Author: Isaac Rowell
 * Date:
 */

#ifndef ADC_H
#define ADC_H

int adc_init(void); // initialize adc module, return 0 on success
long readVcc(); // get the supply voltage

#endif
