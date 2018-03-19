/**
 * File: board.cpp
 * Author: Isaac Rowell
 * Date:
 */
#include <avr/io.h>
#include "board.h"

int board_init(void){

  /* LCD Page change pin */
  DDRD &= ~(1<<DDD2);  // PORTD pin 2 as input

  PORTD |= (1 << PORTD2);    // turn On the Pull-up

  EICRA |= (1<<ISC01) | (1<<ISC00); // trigger on rising edge
  EIMSK |= (1<<INT0);  // enable interrupt for this pin
  EIFR |= (1<<INTF0); // enable interrupt flag

  /* Boost converter enable pin */
  DDRD |= (1<<DDD5);  // PORTD pin 5 as output
  PORTD |= (0<<PORTD5); // initialize port as off

  return 0;
}

int boost_enable(int STATE)
{
  if (STATE == 1){
    PORTD |= (1<<PORTD5); // initialize port as on
    return 1;
  }
  else {
    PORTD |= (0<<PORTD5); // initialize port as off
    return 0;
  }

}
