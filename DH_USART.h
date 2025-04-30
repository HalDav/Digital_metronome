#ifndef DH_USART_H
#define DH_USART_H

#define F_CPU 16000000UL
#define baud_rate 9600UL
#define UBRR_value ((F_CPU / (16UL * baud_rate)) - 1)

#include <avr/io.h>

void USART_init();
void USART_transmit_float(float number);
void USART_transmit_char(char data);
void USART_transmit_string(const char* str);

#endif