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

volatile uint16_t OUTPUT_VOLTAGE_ARRAY[50];
volatile uint16_t OUTPUT_VOLTAGE;
volatile uint16_t ADC_VALUE;
volatile uint8_t ADCLOW;
volatile uint16_t SETPOINT;
volatile uint16_t CURRENT_ARRAY[50];
volatile uint16_t CURRENT;
volatile uint8_t DUTY = 0;
volatile uint8_t TIMER_DELAY = 0;
volatile uint8_t PAGE = 0;

// LiquidCrystal lcd(RS, E, D4, D5, D6, D7);
LiquidCrystal lcd(12, 11, 9, 8, 7, 4);

void voltage_avg(void){
  switch (i) {
    case 50:
      for (i=0; i<50; i++){
       volt_sum+= OUTPUT_VOLTAGE_ARRAY[i];
      }
      OUTPUT_VOLTAGE = volt_sum/(i+1);
      volt_sum = 0;
      i = 0;
      break;
    default:
      break;
  }
}

void curr_avg(void){
  switch (j) {
    case 50:
      for (j=0; j<50; j++){
       curr_sum+= CURRENT_ARRAY[j];
      }
      CURRENT = curr_sum/(j+1);
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
      voltage_avg();
      curr_avg();
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
      CURRENT_ARRAY[j] = ADC_VALUE;
      j++;
      break;
  }
  ADCSRA |= (1<<ADSC); // start new conversion
}

ISR(TIMER2_COMPA_vect)
{
  TIMER_DELAY++;
  if (TIMER_DELAY >= 30){

    if (PAGE == 0){
      lcd.print("SETPOINT: "+String(float(SETPOINT*VOLT_DIV)));
      lcd.setCursor(0, 2);
      lcd.print("OUTPUT: "+String(float(OUTPUT_VOLTAGE*VOLT_DIV)));
    }
    else {
      if ((ACS_MIN <= CURRENT) && (CURRENT < ACS_OFFSET)){
        lcd.print("CURRENT: -"+String(float((ACS_OFFSET-CURRENT)*BIT_DIV/CURR_DIV)));
      }
      else if (CURRENT == ACS_OFFSET){
        lcd.print("CURRENT: "+String(float(0)));
      }
      else {
        lcd.print("CURRENT: "+String(float((CURRENT-ACS_OFFSET)*BIT_DIV/CURR_DIV)));
      }
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
