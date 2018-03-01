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
     // PD2 is now an input with pull-up enabled
     
  EICRA |= (1<<ISC01) | (1<<ISC00);
  EIMSK |= (1<<INT0);
  EIFR |= (1<<INTF0);

  // PCICR |= (1<<PCIE0);  // enable interrupt for this pin group
  // PCIFR |= (1<<PCIF0);  // enable interrupt flag
  // PCMSK0 |= (1<<PCINT5);  // enable interrupt for this pin

  return 0;
}
