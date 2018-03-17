/**
 * File: main.cpp
 * Author: Isaac Rowell
 * Date:
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <Arduino.h>
#include <timer.h>
#include <defines.h>
#include <adc.h>
#include <board.h>
#include <LiquidCrystal.h>

/* volitile variables */
volatile uint16_t OUTPUT_VOLTAGE_ARRAY[voltage_arr];
volatile uint16_t ADC_VALUE;
volatile uint8_t ADCLOW;
volatile uint16_t CURRENT_RAW[curr_arr];
volatile float CURRENT;
volatile uint8_t TIMER_DELAY = 0;
volatile uint8_t PAGE = 0;

volatile uint8_t DUTY;
volatile float OUTPUT_VOLTAGE;
volatile float SETPOINT;

/*PID Variables*/
float kp, ki;
float errSum, lastErr;
unsigned long lastTime;

// LiquidCrystal lcd(RS, E, D4, D5, D6, D7);
LiquidCrystal lcd(12, 11, 9, 8, 7, 4);

void compute_pid()
{
   /*How long since we last calculated*/
   unsigned long now = millis();
   float timeChange = (float)(now - lastTime);

   /*Compute all the working error variables*/
   float error = SETPOINT - OUTPUT_VOLTAGE;
   errSum += (error * timeChange);

   /*Compute PID Output*/
   map(error, 0, 1023.0, 0, 255);
   DUTY = kp * error + ki * errSum;

   /*Remember some variables for next time*/
   lastErr = error;
   lastTime = now;
}

void set_pid(float KP, float KI)
{
  kp = KP;
  ki = KI;
}

/* initialize VCC dependant variables */
void set_vcc_vars(void)
{
  VCC = readVcc()/1000.0;
  BIT_DIV = VCC/1023;
  ACS_OFFSET = VCC/2.13;
}

void voltage_avg(void)
{
  switch (i) {
    case voltage_arr:
      for (i=0; i<voltage_arr; i++){
       volt_sum += OUTPUT_VOLTAGE_ARRAY[i];
      }
      OUTPUT_VOLTAGE = volt_sum/(i+1);
      volt_sum = 0;
      i = 0;
      break;
    default:
      break;
  }
}

void curr_avg(void)
{
  switch (j) {
    case curr_arr:
      for (j=0; j<curr_arr; j++){
       curr_sum += CURRENT_RAW[j];
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
  board_init();
  adc_init();
  set_vcc_vars();
  pwm_init();
  pwm_set(DUTY);
  set_pid(1, 0.1);

  /* initialize the lcd */
  lcd_timer_init();
  lcd.begin(16,2);
  lcd.home();

  sei();

  /* infinite loop for main program */
  while(1)
  {
    /* check averages after each conversion */
    if (ADIF){
      voltage_avg();
      curr_avg();
    }

    /* PID Controller for maintaining the output voltage
    Need to maintain an overdamped system so the current limiting
    does not kick in and drastically reduce the output
    */
    compute_pid();
    pwm_set(DUTY);
    if (DUTY >= MAX_DUTY){
      pwm_set(MAX_DUTY);
    }
    else if (DUTY <= MIN_DUTY){
      pwm_set(MIN_DUTY);
    }
  }
}

/* ADC interrupt to cycle through ADMUX and get analog values */
ISR(ADC_vect)
{
  static uint8_t tmp;
  tmp = ADMUX;
  tmp &= 0X0F;

  ADCLOW = ADCL;
  ADC_VALUE = ADCH<<8 | ADCLOW; // store the conversion value

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
      CURRENT_RAW[j] = ADC_VALUE;
      j++;
      break;
  }
  ADCSRA |= (1<<ADSC); // start new conversion
}

ISR(TIMER2_COMPA_vect)
{
  TIMER_DELAY++;
  if (TIMER_DELAY >= 30){ // divide the timer interrupt by 30
    if (PAGE == 0){
      lcd.print("SETPOINT: "+String(float(SETPOINT*VOLT_DIV)));
      lcd.setCursor(0, 2);
      lcd.print("OUTPUT: "+String(float(OUTPUT_VOLTAGE*VOLT_DIV)));
    }
    else {
      set_vcc_vars();
      if ((ACS_MIN <= CURRENT) && (CURRENT < (ACS_OFFSET/BIT_DIV))){
        lcd.print("CURRENT: -"+String((ACS_OFFSET-(CURRENT*BIT_DIV))/SENSITIVITY));
      }
      else if (CURRENT == (ACS_OFFSET/BIT_DIV)){
        lcd.print("CURRENT: "+String(float(0)));
      }
      else {
        lcd.print("CURRENT: "+String((((CURRENT*BIT_DIV)-ACS_OFFSET))/SENSITIVITY));
      }
      lcd.setCursor(0, 2);
      lcd.print("VCC: "+String(VCC));
    }
    lcd.home();
    TIMER_DELAY = 0;
  }
}

ISR(INT0_vect)
{
  lcd.clear();
  PAGE = !PAGE; // change the lcd page
}
