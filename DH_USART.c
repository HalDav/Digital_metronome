#include "DH_USART.h"
#include <stdlib.h>
#include <stdio.h>

void USART_init(){
  UCSR0A &= ~(1 << U2X0);
  UCSR0B |= (1 << RXEN0) | (1 << TXEN0);
  UCSR0C |= (1 << UCSZ00) | (1 << UCSZ01);
  UBRR0H = (UBRR_value >> 8);
  UBRR0L = UBRR_value;

}

void USART_transmit_char(char data){
  while(!(UCSR0A & (1 << UDRE0))){
      //waiting for UDRE0 to be zero
  }
  UDR0 = data;
}

void USART_transmit_string(const char *str){
  while(*str){
   USART_transmit_char(*str++);
  }
}

void USART_transmit_float(float number){
  char buffer[20];
  dtostrf(number, 4, 4, buffer);
  USART_transmit_string(buffer);
}