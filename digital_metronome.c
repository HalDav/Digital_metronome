/**********************************************************************************************************************************
* Project:    Digital metronome
*
* Author:     HalDav
* Platform:   Atmel ATMEGA328P
*
*
* Used modules: 
*   - 4 x  LED for beat signalization
*   - 2 x  Push button for mode selection, on/off
*   - 1 x  Potentiometer for bpm selection
*
* Tasks:
*   - Changes bpm based on potentiometervoltage value (30-250bpm)
*   - Changes measures (4/4, 3/4, 2/4), beats are indicated via LEDs
*   - On/Off option via push button 
*   - Measure mode and bpm sends to computer via USART
*
*/



#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <util/delay.h>
#include <stdbool.h>
#include "DH_USART.h"
#include "DH_pot_controller.h"
#include "DH_bpm_generator.h"

volatile int j;
volatile int beat_mode;
volatile bool on_off_mode;


void interrupt_init(){
  EICRA &= ~ (1 << ISC01) & (1 << ISC00);
  EICRA &= ~ (1 << ISC11) & (1 << ISC10);
  EIMSK |= (1 << INT0) | (1 << INT1);
  sei();
}

ISR(INT0_vect){
  _delay_ms(500);
  if(on_off_mode == true){
    on_off_mode = false;
    PORTB = 0x00;
  }
  else{
    on_off_mode = true;
    j = 3;
  }
}


ISR(INT1_vect){
  _delay_ms(350);
  USART_transmit_string("SWITCHING MODE\n");
  beat_mode++;
  if(beat_mode == 4){
    beat_mode = 1;
  }
}

ISR(TIMER1_COMPA_vect){
  if(on_off_mode == true){
    int beat = 0;
    j++;
    if(j > beat_mode){
      j = 0;
    }
    beat = (1 << j);
    PORTB = (PORTB & 0xf0) | beat;
  }
}




int main(void){
  DDRB = 0xff;
  DDRD = 0x00;
  PORTD = 0xff; 
  j = 0;
  beat_mode = 3;
  on_off_mode = false;
  
  USART_init();
  ADC_read_init();
  interrupt_init();
  timer_init();


  while(1){
    uint16_t ADC_value = ADC_read(2);
    float cycle = 0.24 + (ADC_value * 0.00172); 
    float OCR1_value = ((cycle * F_CPU) / 1024.); 
    OCR1A = OCR1_value;
    float bps = 1/cycle;
    float bpm = bps*60.;

    if(on_off_mode == true){
      USART_transmit_string("*-*-*-*-*-*-*-*-*-*-*-*-*\n");
      USART_transmit_string("Your actual beat mode: ");
      if(beat_mode == 3){
        USART_transmit_string(" 4/4");
      }
      if(beat_mode == 2){
        USART_transmit_string(" 3/4");
      }
      if(beat_mode == 1){
        USART_transmit_string(" 2/4");
      }
    
      USART_transmit_string("\n");
      USART_transmit_string("Your actual bpm: ");
      USART_transmit_float(bpm);
      USART_transmit_string("\n");
      _delay_ms(200);
      }
  }
}