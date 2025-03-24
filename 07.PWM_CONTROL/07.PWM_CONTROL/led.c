/*
 * led.c
 *
 * Created: 2025-03-05 오전 10:21:45
 *  Author: microsoft
 */ 

#include "led.h"
#include "def.h"

void init_led()
{
	LED_DDR |= LED_PORT_ALL; // led port output mode로 설정
}

void led_on(uint8_t inLedPortNum) {

	PORTA |= 1 << inLedPortNum;
}

void led_off(uint8_t inLedPortNum) {
	
	PORTA &= ~(1 << inLedPortNum);
}

void led_all_on(void) 
{
	PORTA |= LED_PORT_ALL;

}

void led_all_off(void) 
{
	PORTA &= ~LED_PORT_ALL;
}


