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
#include <board.h>
#include <defines.h>
#include <LiquidCrystal.h>

volatile uint16_t OUTPUT_VOLTAGE_ARRAY[30];
volatile uint16_t OUTPUT_VOLTAGE;
volatile uint16_t ADC_VALUE;
volatile uint8_t ADCLOW;
volatile uint16_t SETPOINT;
volatile uint16_t CURRENT_ARRAY[30];
volatile uint16_t CURRENT;
volatile uint8_t DUTY = 0;
volatile uint8_t TIMER_DELAY = 0;
volatile uint8_t PAGE = 0;

// LiquidCrystal lcd(RS, E, D4, D5, D6, D7);
LiquidCrystal lcd(12, 11, 9, 8, 7, 4);

void adc_avg(void){
  switch (i) {
    case 30:
      for (i=0; i<30; i++){
       sum+= OUTPUT_VOLTAGE_ARRAY[i];
       curr_sum+= CURRENT_ARRAY[i];
      }
      OUTPUT_VOLTAGE = sum/i;
      CURRENT = curr_sum/i;
      sum = 0;
      i = 0;
      curr_sum = 0;
      break;
    default:
      break;
  }
  switch (j) {
    case 30:
      for (j=0; j<30; j++){
       curr_sum+= CURRENT_ARRAY[j];
      }
      CURRENT = curr_sum/j;
      j = 0;
      curr_sum = 0;
      break;
    default:
      break;
  }
}

int main(void)
{
  Serial.begin(115200);
  pwm_init();
  pwm_set(DUTY);
  adc_init();
  lcd_timer_init();
  board_init();
  lcd.begin(16,2); // initialize the lcd
  lcd.home(); // go home

  sei();

  while(1) // infinite loop
  {
    if (ADIF){
      adc_avg();
    }

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
      OUTPUT_VOLTAGE_ARRAY[i] = ADC_VALUE;
      i++;
      break;
    case 1:
      ADMUX++;
      SETPOINT = ADC_VALUE;
      break;
    case 2:
      ADMUX &= 0XF8;
      CURRENT_ARRAY[i] = ADC_VALUE;
      j++;
      break;
  }
  ADCSRA |= (1<<ADSC); // start new conversion
}

ISR(TIMER2_COMPA_vect)
{
  TIMER_DELAY++;
  if (TIMER_DELAY >= 60){
    if (PAGE == 0){
      lcd.print("SETPOINT: "+String(float(SETPOINT*VOLT_DIV)));
      lcd.setCursor(0, 2);
      lcd.print("OUTPUT: "+String(float(OUTPUT_VOLTAGE*VOLT_DIV)));
    }
    else {
      lcd.print("CURRENT: "+String(float(((CURRENT*BIT_DIV)-ACS_OFFSET)/CURR_DIV)));
    }
    lcd.home();
    TIMER_DELAY = 0;
  }
}

ISR(INT0_vect)
{
  lcd.clear();
  PAGE = !PAGE;
}
