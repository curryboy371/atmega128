/*
 * ultrasonic.h
 *
 * Created: 2025-03-12 오후 2:49:07
 *  Author: microsoft
 */ 


#ifndef ULTRASONIC_H_
#define ULTRASONIC_H_

#include "timer.h"

#define  F_CPU 16000000UL  // 16MHZ
#include <avr/io.h>
#include <util/delay.h>  // _delay_ms _delay_us
#include <avr/interrupt.h> // interrupt
#include <stdio.h>
#include <string.h>


#define ULTRASONIC_TIMER_TCCR TIMER2_TCCR

// TRIG 세팅
#define TRIG_PORT	PORTA
#define TRIG_DDR	DDRA

// ECHO 세팅
#define ECHO_PIN	PINE	
#define ECHO_DDR	DDRE

// TRIG 세팅
#define TRIG_RIGHT			0
#define TRIG_FORWARD		1
#define TRIG_LEFT			2

#define ULTRASONIC_RIGHT		0
#define ULTRASONIC_FORWARD		1
#define ULTRASONIC_LEFT			2

// ECHO 세팅
#define ECHO_RIGHT		4
#define ECHO_FORWARD	5
#define ECHO_LEFT		6


#define TRIGGER_SEND_DELAY 30
#define SPEED_OF_SOUND	0.0343


void init_ultrasonic();
void trigger_ultrasonic(uint8_t inTriggerIdx);

void update_ultrasonic();
uint8_t get_ultrasonic_distance(int outDistances[]);


void set_trigger_low(const uint8_t inTriggerIdx);
void set_trigger_high(const uint8_t inTriggerIdx);


#endif /* ULTRASONIC_H_ */