/*
 * led.c
 *
 * Created: 2025-03-05 오전 10:21:45
 *  Author: microsoft
 */ 



#define F_CPU 16000000UL	// 16MHZ 설정
#include <avr/io.h>
#include <util/delay.h> // for _delay_ms _delay_us ...


#include "led.h"
#include "def.h"
#include "extern.h"


void init_led() {
	// led 출력 모드로 설정
	AUTO_RUN_LED_DDR |= 1 << AUTO_RUN_LED_PIN;
	
}


void led_on(void)  {
	
	AUTO_RUN_LED_PORT |= 1 << AUTO_RUN_LED_PIN;
}

void led_off(void)  {
	
	AUTO_RUN_LED_PORT &= ~(1 << AUTO_RUN_LED_PIN);
}

