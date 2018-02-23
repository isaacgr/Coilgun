/**
 * File: main.cpp
 * Author: Isaac Rowell
 * Date:
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <Arduino.h>
#include <timer.h>
#include <adc.h>
#include <defines.h>
#include <LiquidCrystal.h>

volatile uint16_t OUTPUT_VOLTAGE;
volatile uint16_t ADC_VALUE;
volatile uint8_t ADCLOW;
volatile uint16_t SETPOINT;
volatile uint8_t DUTY = 0;
volatile uint8_t TIMER_DELAY = 0;

// LiquidCrystal lcd(RS, E, D4, D5, D6, D7);
LiquidCrystal lcd(12, 11, 7, 4, 3, 2);

int main(void)
{
  Serial.begin(115200);
  pwm_init();
  pwm_set(DUTY);
  adc_init();
  lcd_timer_init();
  lcd.begin(16,2);               // initialize the lcd
  lcd.home();                   // go home

  sei();

  while(1) // infinite loop
  {
    if (OUTPUT_VOLTAGE > SETPOINT){
      if (DUTY >= MAX_DUTY){
        pwm_set(MAX_DUTY);
        continue;
      }
      DUTY++;
      pwm_set(DUTY);
    }
    else if (OUTPUT_VOLTAGE < SETPOINT){
      if (DUTY <= MIN_DUTY){
        pwm_set(MIN_DUTY);
        continue;
      }
      DUTY--;
      pwm_set(DUTY);
    }
    else {
      continue;
    }
  }
}

ISR(ADC_vect)
{
  static uint8_t tmp;
  tmp = ADMUX;
  tmp &= 0X0F;

  ADCLOW = ADCL;
  ADC_VALUE = ADCH<<8 | ADCLOW;

  switch (tmp) {
    case 0:
      ADMUX++;
      OUTPUT_VOLTAGE = ADC_VALUE;
      break;
    case 1:
      ADMUX &= 0XF8;
      SETPOINT = ADC_VALUE;
      break;
  }
  ADCSRA |= 1<<ADSC; // start new conversion
}

ISR(TIMER2_COMPA_vect)
{
  TIMER_DELAY++;
  if (TIMER_DELAY >= 30){
    lcd.print("SETPOINT: "+String(float(SETPOINT*DIVIDER)));
    lcd.setCursor(0, 2);
    lcd.print("OUTPUT: "+String(float(OUTPUT_VOLTAGE*DIVIDER)));
    lcd.home();
    TIMER_DELAY = 0;
  }
}
