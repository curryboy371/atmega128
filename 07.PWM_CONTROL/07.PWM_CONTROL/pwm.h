/*
 * pwm.h
 *
 * Created: 2025-03-13 오전 3:49:02
 *  Author: microsoft
 */ 


#ifndef PWM_H_
#define PWM_H_

#include "timer.h"

#define F_CPU 16000000UL	// 16MHZ 설정
#include <avr/io.h>
#include <util/delay.h> // for _delay_ms _delay_us ...
#include <avr/interrupt.h> // interrupt
#include <stdio.h>

#define MOTER_TIMER_PIN			5			// pe3 - pe5 선택
#define MOTER_TIMER_PIN2		6			// pe3 - pe5 선택

// moter timer define
#define MOTER_TIMER_DDR		TIMER1_DDR
#define MOTER_TIMER_PORT	TIMER1_PORT

#if MOTER_TIMER_PIN == 3

#define MOTER_TIMER_PIN		TIMER1_PIN3
#define MOTER_TIMER_OCR		TIMER1_OCRA

#elif MOTER_TIMER_PIN == 4

#define MOTER_TIMER_PIN		TIMER1_PIN4
#define MOTER_TIMER_OCR		TIMER1_OCRB

#else // 5, wrong value...

#define MOTER_TIMER_PIN		TIMER1_PIN5
#define MOTER_TIMER_OCR		TIMER1_OCRC

#endif



#define MOTER_TIMER_TCCRA	TIMER1_TCCRA
#define MOTER_TIMER_TCCRB	TIMER1_TCCRB
#define MOTER_TIMER_TCCRC	TIMER1_TCCRC

// 비교 출력 레지스터
#define MOTER_TIMER_OCRA	TIMER1_OCRA
#define MOTER_TIMER_OCRB	TIMER1_OCRB
#define MOTER_TIMER_OCRC	TIMER1_OCRC

// top 값 설정시 사용
#define MOTER_TIMER_ICR		TIMER1_ICR

// moter define
#define MOTOR_DDR			DDRF
#define MOTOR_PORT			PORTF

#define MOTER_ENA_PIN		TIMER1_PIN5
#define MOTER_ENB_PIN		TIMER1_PIN6

#define MOTER_IN1_PIN		0
#define MOTER_IN2_PIN		1
#define MOTER_IN3_PIN		2
#define MOTER_IN4_PIN		3

void init_Moter();
void init_L298N_timer();
void init_L298N();
void L298N_change_speed(uint8_t bUp);
void L298N_run(uint8_t inSpeed);
void L298N_switch_direction();


uint8_t get_moter_speed();

#endif /* PWM_H_ */