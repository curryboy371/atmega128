/*
 * led.h
 *
 * Created: 2025-03-05 오전 10:21:16
 *  Author: microsoft
 */ 


#ifndef LED_H_
#define LED_H_


#define F_CPU 16000000UL	// 16MHZ 설정
#include <avr/io.h>
#include <util/delay.h> // for _delay_ms _delay_us ...


#define LED_DDR		DDRA		// LED PORT가 변경되면 LED_DDR만 바꿔주면 compiler가 변경
#define LED_PORT	PORTA

#define LED_PORT1  0
#define LED_PORT2  1
#define LED_PORT3  2
#define LED_PORT4  3

#define LED_PORT_ALL (1 << LED_PORT1 | 1 << LED_PORT2 | 1 << LED_PORT3 | 1 << LED_PORT4)


#define LED_LENTH	4


void init_led();

void led_on(uint8_t inLedPortNum);
void led_off(uint8_t inLedPortNum);


#endif /* LED_H_ */