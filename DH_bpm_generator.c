#include <avr/io.h>
#include "DH_bpm_generator.h"

void timer_init(){
  TCCR1A = 0;
  TCCR1B =  (1 << WGM12) | (1 << CS12) | (1 << CS10);
  TIMSK1 |= (1 << OCIE1A);
  

}