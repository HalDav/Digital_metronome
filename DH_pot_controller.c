#include <stdint.h> 
#include "DH_pot_controller.h"
#include <avr/io.h>


void ADC_read_init(){
  ADMUX |= (1 << REFS0);
  ADCSRA |= (1 << ADEN) | (1 << ADPS0) | (1 << ADPS1) | (1 << ADPS2); //ADC enable + 16MHz/128 = 125kHz ADC prescaler//
}

uint16_t ADC_read(uint8_t channel){
  channel &= 0x0f; 
  ADMUX = (ADMUX & 0xf0) | channel;
  ADCSRA |= (1 << ADSC);
  while(ADCSRA & (1 << ADSC)){

  } 
  return ADC;
}