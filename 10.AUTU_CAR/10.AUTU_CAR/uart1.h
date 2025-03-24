/*
 * uart1.h
 *
 * Created: 2025-03-10 오전 10:32:22
 *  Author: microsoft
 */ 


#ifndef UART1_H_
#define UART1_H_


//여긴 fcpu def 안해도 되나

#define F_CPU 16000000UL	// 16MHZ 설정
#include <avr/io.h>
#include <util/delay.h> // for _delay_ms _delay_us ...
#include <avr/interrupt.h> // interrupt



void init_uart1(void);
void UART1_transmit(uint8_t data);














#endif /* UART0_H_ */