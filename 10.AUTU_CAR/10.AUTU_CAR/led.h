/*
 * led.h
 *
 * Created: 2025-03-05 오전 10:21:16
 *  Author: microsoft
 */ 


#ifndef LED_H_
#define LED_H_

#define AUTO_RUN_LED_PORT	PORTG
#define AUTO_RUN_LED_DDR	DDRG
#define AUTO_RUN_LED_PIN	3

#define LED_LENTH	1

void init_led();

void led_on();
void led_off();

#endif /* LED_H_ */