/*
 * pwm.c
 *
 * Created: 2025-03-13 오전 3:48:48
 *  Author: microsoft
 
 

 
 
 
 
 
 */ 

#define F_CPU 16000000UL	// 16MHZ 설정
#include <avr/io.h>
#include <util/delay.h> // for _delay_ms _delay_us ...
#include <avr/interrupt.h>
#include <string.h>


#include "pwm.h"

#include "button.h"
#include  "def.h"



/*
*/

//extern t_ControlMamager ControlMgr;

t_EDIR_MODE eNewDirection = EDIR_STOP;
t_EDIR_MODE eCurDirection = EDIR_STOP;

#define RESET_MOTER_DRIVER_DIRECTION	MOTER_DRIVER_DIRECTION_PORT &= ~(1 << MOTER_IN1 | 1 << MOTER_IN2 | 1 << MOTER_IN3 | 1 << MOTER_IN4)

void init_timer1() {

	//분주비
	// 분주비 : 64    16,000,000 / 64 --> 250,000Hz ( 250kHz )
	// T = 1/f  1 / 250,000Hz ==> 0.000004sec (4us)
	// 250,000Hz에서 256 펄스를 count 하면 소요시간은 1.02ms
	// 127 절반을 count하면 0.51ms
	// page 318 표 14-1
	TCCR1B |= 1 << CS11 | 1 << CS10;

	// timer1 mode 14번 고속 pwm (p327 표 14-5)
	// top값을 ICR1으로 지정해서 고속 pwm을 사용할 수 있는 모드임
	// top값을 ICR1에 지정 가능
	TCCR1A |= 1 << WGM11 | 0 << WGM10; 
	TCCR1B |= 1 << WGM13 | 1 << WGM12;
	
	
	// 비반전모드로 설정 page 350 15-7
	// TOP값은 ICR1 비교일치값(PWM) 지정 : OCR1A, OCR1B
	
	// pb5니까 a 서정하고, pb6도 사용하니 OCR1A, OCR1B 둘 다 
	
	TCCR1A |= 1 << COM1A1 | 0 << COM1A0; // OCR1A
	TCCR1A |= 1 << COM1B1 | 0 << COM1B0; // OCR1B
	
	// top값을 지정하기 (pwm값)
	ICR1 = 0x3ff; // 1023 = 4ms 	
}

/*

// Timer 1번 사용
// PB5 : OC1A 왼쪽 바퀴
// PB6 : OC1B 오른쪽 바퀴

// BTN0 : auto <> manual

방향설정

1. LEFT MOTER 

PF0 : IN1 ( moter driver)
PF1 : IN2 ( 

2. RIGHT MOTER
PF2 : IN3 ( moter driver)
PF3 : IN4 ( 


	IN1/IN3		IN2/IN4
	=======		=======
	0			1		: 역회전
	1			0		: 정회전
	1			1		: STOP

*/


void init_L298N() {
	
	init_timer1();
	
	// moter pwm ddr 출력 모드로 설정
	MOTER_PWM_DDR |= 1 << MOTER_LEFT_PORT_DDR | 1 << MOTER_RIGHT_PORT_DDR;
	
	// moter direction port ddr 출력 모드로 설정 IN1, IN2, IN3, IN4
	MOTER_DRIVER_DIRECTION_DDR |= 1 << MOTER_IN1 | 1 << MOTER_IN2 | 1 << MOTER_IN3 | 1 << MOTER_IN4;
	
	// 방향설정 reset

	RESET_MOTER_DRIVER_DIRECTION;
	// 전진 모드로 설정
	MOTER_DRIVER_DIRECTION_PORT |= 1 << MOTER_IN3 | 1 << MOTER_IN1; // 전진 모드 설정
	
}

void reset_L298N_direction() {
	MOTER_DRIVER_DIRECTION_PORT &= ~(1 << MOTER_IN1 | 1 << MOTER_IN2 | 1 << MOTER_IN3 | 1 << MOTER_IN4);
}

void L298N_stop() {
	
	//reset_L298N_direction();
	
	RESET_MOTER_DRIVER_DIRECTION;
	MOTER_DRIVER_DIRECTION_PORT |= (1 << MOTER_IN1 | 1 << MOTER_IN2 | 1 << MOTER_IN3 | 1 << MOTER_IN4);
	
	eCurDirection = EDIR_STOP;
}

void L298N_forward(uint16_t inSpeed) {
	//reset_L298N_direction();
	RESET_MOTER_DRIVER_DIRECTION;
	// 전진 모드로 설정
	MOTER_DRIVER_DIRECTION_PORT |= 1 << MOTER_IN3 | 1 << MOTER_IN1; // 전진 모드 설정
	
	MOTER_LEFT_OCR = MOTER_RIGHT_OCR = inSpeed; // pb5 왼쪽 pb6 오른쪽
	
	eCurDirection = EDIR_FOWARD;
}

void L298N_backward(uint16_t inSpeed) {
	
	//reset_L298N_direction();
	RESET_MOTER_DRIVER_DIRECTION;
		
	// 후진 모드로 설정
	MOTER_DRIVER_DIRECTION_PORT |= 1 << MOTER_IN4 | 1 << MOTER_IN2;
		
	MOTER_LEFT_OCR = MOTER_RIGHT_OCR = inSpeed; 
	
	eCurDirection = EDIR_BACKWARD;
}


void L298N_turn_left(uint16_t inSpeed) {
	
	RESET_MOTER_DRIVER_DIRECTION;
	
	MOTER_DRIVER_DIRECTION_PORT |= 0 << MOTER_IN1 | 1 << MOTER_IN2;
	MOTER_DRIVER_DIRECTION_PORT |= 1 << MOTER_IN3 | 0 << MOTER_IN4;
	
	MOTER_RIGHT_OCR = MOTER_RIGHT_OCR = inSpeed;
	
	eCurDirection = EDIR_LEFT;
	
}

void L298N_turn_right(uint16_t inSpeed) {
	
	RESET_MOTER_DRIVER_DIRECTION;
	MOTER_DRIVER_DIRECTION_PORT |= 1 << MOTER_IN1 | 0 << MOTER_IN2;
	
	MOTER_DRIVER_DIRECTION_PORT |= 0 << MOTER_IN3 | 1 << MOTER_IN4;
	
	MOTER_LEFT_OCR = MOTER_RIGHT_OCR = inSpeed; // pb5 왼쪽 pb6 오른쪽
	
	eCurDirection = EDIR_RIGHT;
}



