/**
 * File: main.cpp
 * Author: Isaac Rowell
 * Date:
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <Arduino.h>
#include <pwm.h>
#include <adc.h>

#define PWM_FREQUENCY 0X013F  //50kHZ with Timer1

volatile uint8_t OUTPUT_VOLTAGE_LSB;
volatile uint16_t OUTPUT_VOLTAGE_MSB;

int main(void)
{
  Serial.begin(115200);
  uint16_t DUTY = 0X0020;
  pwm_init();
  pwm_set_duty(DUTY, PWM_FREQUENCY);
  adc_init();
  sei();
  Serial.print("Begin");
  while (1){
    Serial.println(OUTPUT_VOLTAGE_MSB, DEC);
  }
}

ISR(ADC_vect)
{
  OUTPUT_VOLTAGE_LSB = ADCL;
  OUTPUT_VOLTAGE_MSB = ADCH<<8 | OUTPUT_VOLTAGE_LSB;
}
