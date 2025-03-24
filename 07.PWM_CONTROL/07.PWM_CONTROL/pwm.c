/*
 * pwm.c
 *
 * Created: 2025-03-13 오전 3:48:48
 *  Author: microsoft
 
 
 16bit timer 3번을 활용
 
 pwm 출력 신호 ( 3EA )
 
 PE3 : OC3A
 PE4 : OC3B / INT4 --> 사용중 ( 초음파, echo pulse external INT용으로 사용중임)
 PE5 : OC3C --> 모터 연결
 
 BTN 0 : start stop
 
 BTN 1 : speed up	( OC3C : 20씩 증가 ) 
 BTN 2 : speed down ( OC3C : 20씩 감소 ) 
 * OC3C min max : 60 ~ 250
 
 */ 

#include "pwm.h"

#include "button.h"
#include "common.h"
#include "def.h"


void init_Moter() {
	
	MOTOR_DDR |= 1 << MOTER_IN1_PIN | 1 << MOTER_IN2_PIN | 1 << MOTER_IN3_PIN | 1 << MOTER_IN4_PIN;  // 출력 모드로 설정
	MOTOR_PORT &= ~(1 << MOTER_IN1_PIN | 1 << MOTER_IN2_PIN | 1 << MOTER_IN3_PIN | 1 << MOTER_IN4_PIN); // 6,7 reset clear

	MOTOR_PORT |= 1 << MOTER_IN1_PIN | 1 << MOTER_IN2_PIN; // 정회전 설정
}


void init_L298N_timer() {

	// e의 3, 5 ( 4번은 초음파에서 사용중...)
	//DDRE |= 1 << 3 | 1 << 5;
	MOTER_TIMER_DDR |= 1 << MOTER_TIMER_PIN | 1  << MOTER_TIMER_PIN2;
	
	
	// 고속 pwm 모드로 사용함 page 348sd
	
	// 16bit timer3번의 mode 5: 8bit 고속 pwm 모드 
	
	// 8bit 고속모드 agm11 0  agm10 1
	// wgm13 0 wgm12 1
	
	//MOTER_TIMER_TCCRA |= 1 << WGM30;
	//MOTER_TIMER_TCCRA &= ~(1 << WGM31);
	//
	//MOTER_TIMER_TCCRB |= 1 << WGM32;
	//MOTER_TIMER_TCCRB &= ~(1 << WGM33);
	
	
	//COM3A1	COM3A0
	// 1		0		Non-Inverting (비반전 PWM)	OCR3A 값보다 작으면 HIGH, 크면 LOW
	// 비반전 모드
	// 사용자가 설정한 PWM값에 일치시 OC3C의 출력이 LOW로 바뀌고 BOTTOM에서 HIGH로 바뀜
	// page 350 표 15-7
	// 비반전모드 COMnx1 : 1, COMnx0 : 0
	
#if MOTER_TIMER_PIN == 3
	MOTER_TIMER_TCCRA |= 1 << COM3A1;
	MOTER_TIMER_TCCRA &= ~(1 << COM3A0);
#elif MOTER_TIMER_PIN == 4
	MOTER_TIMER_TCCRA |= 1 << COM3B1;
	MOTER_TIMER_TCCRA &= ~(1 << COM3B0);
#else
	MOTER_TIMER_TCCRA |= 1 << COM3C1;
	MOTER_TIMER_TCCRA &= ~(1 << COM3C0);
#endif

	//MOTER_TIMER_TCCRA |= 1 << COM3C1;
	
	// 분주비 : 64    16,000,000 / 64 --> 250,000Hz ( 250kHz )
	// T = 1/f  1 / 250,000Hz ==> 0.000004sec (4us)
	// 250,000Hz에서 256 펄스를 count 하면 소요시간은 1.02ms
	// 127 절반을 count하면 0.51ms
	// page 318 표 14-1
	
	MOTER_TIMER_TCCRB |= 1 << CS31 | 1 << CS30;
	
	MOTER_TIMER_OCR = 0;// OCR ( Output compare register ) : pwm 값

}

// PF6 : IN1 ( L298N)
// PF7 : IN2 ( L298N)
// PF6(IN1) PF7(IN2)
// 0		0       : 역회전
// 1		0		: 정회전
// 1		1		: stop
void init_L298N() {

	init_L298N_timer();

	MOTOR_DDR |= 1 << MOTER_IN1_PIN | 1 << MOTER_IN2_PIN | 1 << MOTER_IN3_PIN | 1 << MOTER_IN4_PIN;  // 출력 모드로 설정
	MOTOR_PORT &= ~(1 << MOTER_IN1_PIN | 1 << MOTER_IN2_PIN | 1 << MOTER_IN3_PIN | 1 << MOTER_IN4_PIN); 
	MOTOR_PORT |= 1 << MOTER_IN1_PIN | 1 << MOTER_IN2_PIN | 1 << MOTER_IN3_PIN | 1 << MOTER_IN4_PIN; 
}



void L298N_change_speed(uint8_t bUp) {

	if(bUp) {
		MOTER_TIMER_OCR = (MOTER_TIMER_OCR >= MAX_SPEED) ? MAX_SPEED : MOTER_TIMER_OCR+20;
	} 
	else{
		MOTER_TIMER_OCR = (MOTER_TIMER_OCR <= MIN_SPEED) ? MIN_SPEED : MOTER_TIMER_OCR-20;
	}
}

void L298N_run(uint8_t inSpeed) {
	
	//ControlMgr.bIsRun = 1;
	MOTER_TIMER_OCR = inSpeed;
}

void L298N_switch_direction() {
	
	//ControlMgr.bIsForward = !ControlMgr.bIsForward;
	//
	//// reset
	//MOTOR_PORT &= ~(1 << MOTER_IN1_PIN | 1 << MOTER_IN2_PIN);
	//MOTOR_PORT |= (ControlMgr.bIsForward) ? (1 << MOTER_IN1_PIN) : (1 << MOTER_IN2_PIN);
	
}

uint8_t get_moter_speed() {
	
	return (uint8_t)MOTER_TIMER_OCR;
}





