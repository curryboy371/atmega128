/*
 * button.h
 *
 * Created: 2025-03-05 오후 2:30:36
 *  Author: microsoft
 */ 


#ifndef BUTTON_H_
#define BUTTON_H_

#define F_CPU 16000000UL	// 16MHZ 설정
#include <avr/io.h>	// PORT io 관련 register 등이 있음
#include <util/delay.h> // for _delay_ms _delay_us ...
#include <avr/interrupt.h> // interrupt

#define BUTTONA_DDR DDRD
#define BUTTONA_PIN PIND		// PIND는 PORTD를 읽는 register 5v:1 0v:0
#define BUTTONB_DDR DDRE
#define BUTTONB_PIN PINE		// 

// button to port num
#define BUTTON0PIN 0		// PORTD.0
#define BUTTON1PIN 1		// PORTD.1
#define BUTTON2PIN 4		// PORTE.4
#define BUTTON3PIN 5		// PORTE.5


// button num define
#define BUTTON0			0		
#define BUTTON1			1		
#define BUTTON2			2		
#define BUTTON3			3		
#define BUTTON_NUM		4	// 버튼의 개수``

#define BUTTON_PRESS	1	// 버튼 누르면 high( active-high)
#define BUTTON_RELEASE	0	// 버튼 떼면 low

void init_button_INT();
void init_button(void);


#endif /* BUTTON_H_ */