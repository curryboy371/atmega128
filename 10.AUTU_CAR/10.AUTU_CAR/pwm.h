/*
 * pwm.h
 *
 * Created: 2025-03-13 오전 3:49:02
 *  Author: microsoft
 */ 


#ifndef PWM_H_
#define PWM_H_

#define F_CPU 16000000UL	// 16MHZ 설정
#include <avr/io.h>
#include <util/delay.h> // for _delay_ms _delay_us ...
#include <avr/interrupt.h> // interrupt
#include <stdio.h>

#define MOTER_PWM_DDR	DDRB

// 네이밍이... ddr pin이 더 나을듯??
#define MOTER_LEFT_PORT_DDR		5 // OC1A
#define MOTER_RIGHT_PORT_DDR	6 // OC1B

#define MOTER_DRIVER_DIRECTION_PORT	PORTF   // 모터 방향설정 port
#define MOTER_DRIVER_DIRECTION_DDR	DDRF   // 모터 방향설정 ddr


#define MOTER_IN1	0
#define MOTER_IN2	1
#define MOTER_IN3	2
#define MOTER_IN4	3



#define MOTER_PWM_PORT	PORTB

// pb5 왼쪽 pb6 오른쪽
#define MOTER_LEFT_OCR		OCR1A
#define MOTER_RIGHT_OCR		OCR1B


//#define FORWARD_BIT 6
//#define BACKWARD_BIT 7

void init_timer3();
void init_L298N();

void reset_L298N_direction();

void L298N_stop();
void L298N_forward(uint16_t inSpeed);
void L298N_backward(uint16_t inSpeed);

void L298N_turn_left(uint16_t inSpeed);
void L298N_turn_right(uint16_t inSpeed);



#endif /* PWM_H_ */