/**
 * File: board.cpp
 * Author: Isaac Rowell
 * Date:
 */
#include <avr/io.h>
#include "board.h"

int board_init(void){

  DDRD &= ~(1<<DDD2);  // PORTB pin 5 as input

  PORTD |= (1 << PORTD2);    // turn On the Pull-up

  EICRA |= (1<<ISC01) | (1<<ISC00); // trigger on rising edge
  EIMSK |= (1<<INT0);  // enable interrupt for this pin
  EIFR |= (1<<INTF0); // enable interrupt flag

  return 0;
}
