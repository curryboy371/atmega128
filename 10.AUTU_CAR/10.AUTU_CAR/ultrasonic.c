/*
 * ultrasonic.c
 *
 * Created: 2025-03-12 오후 2:49:18
 *  Author: microsoft
 */ 

#define F_CPU 16000000UL	// 16MHZ 설정

#include <util/atomic.h>

#include <avr/io.h>
#include <util/delay.h> // for _delay_ms _delay_us ...
#include <avr/interrupt.h>
#include <string.h>


#include "ultrasonic.h"
#include "def.h"

#include "led.h"

volatile int ultrasonic_check_timer;
volatile int ultrasonic_dist[ULTRASONIC_LENGTH];
volatile int ultrasonic_trigger_counts[ULTRASONIC_LENGTH];
volatile uint8_t timer2_start_ovf_cnt[ULTRASONIC_LENGTH];
volatile uint8_t timer2_ovf;

volatile uint16_t echoStartTimeArr[ULTRASONIC_LENGTH];

uint8_t bEchoActive;
uint8_t bDistancePending; // 거리 계산 예약 중인 상태

uint8_t bOnTriggers[ULTRASONIC_LENGTH];

static uint8_t triggerStep = 1;

int preScale;
ISR(TIMER2_OVF_vect) {
	
	++timer2_ovf; // 자동으로 ovf 일어나도록
}


ISR(INT4_vect) {
	
	uint8_t curUltrasonicIdx = ULTRASONIC_RIGHT;
	
	if (ECHO_PIN & 1 << ECHO_RIGHT) {
		echoStartTimeArr[curUltrasonicIdx] = TCNT2;
		timer2_start_ovf_cnt[curUltrasonicIdx] = timer2_ovf;
	}
	else {
		int16_t countDiff = TCNT2 - echoStartTimeArr[curUltrasonicIdx];
		int16_t ovfDiff = (int16_t)timer2_ovf - (int16_t)timer2_start_ovf_cnt[curUltrasonicIdx];
		if(ovfDiff < 0 ) {
			ovfDiff += MAX_UINT8_COUNT;
		}
		
		if (countDiff < 0) {
			countDiff +=MAX_UINT8_COUNT;
			ovfDiff--;
		}
		
		
		ultrasonic_dist[curUltrasonicIdx] = countDiff + ovfDiff * MAX_UINT8_COUNT;
		bEchoActive = FALSE;
	}
}


ISR(INT5_vect) {
	
	uint8_t curUltrasonicIdx = ULTRASONIC_FORWARD;
	
	if (ECHO_PIN & 1 << ECHO_FORWARD) {
		echoStartTimeArr[curUltrasonicIdx] = TCNT2;
		timer2_start_ovf_cnt[curUltrasonicIdx] = timer2_ovf;
	}
	else {
		int16_t countDiff = TCNT2 - echoStartTimeArr[curUltrasonicIdx];
		int16_t ovfDiff = (int16_t)timer2_ovf - (int16_t)timer2_start_ovf_cnt[curUltrasonicIdx];
		if(ovfDiff < 0 ) {
			ovfDiff += MAX_UINT8_COUNT;
		}
		
		if (countDiff < 0) {
			countDiff +=MAX_UINT8_COUNT;
			ovfDiff--;
		}
		
		ultrasonic_dist[curUltrasonicIdx] = countDiff + ovfDiff * MAX_UINT8_COUNT;
		bEchoActive = FALSE;
		
		
		//printf("front tStart=%d tEnd=%d diff=%d ovfc=%d ovfs=%d ovfdif=%d totalCnt=%d distance=%.2f cm\r\n",
		//echoStartTimeArr[curUltrasonicIdx], TCNT2,countDiff,
		//timer2_ovf, timer2_start_ovf_cnt[curUltrasonicIdx], ovfDiff,
		//ultrasonic_dist[curUltrasonicIdx], distance);
	}
			

}


ISR(INT6_vect) {
	
	uint8_t curUltrasonicIdx = ULTRASONIC_LEFT;
	
	if (ECHO_PIN & 1 << ECHO_LEFT) {
		echoStartTimeArr[curUltrasonicIdx] = TCNT2;
		timer2_start_ovf_cnt[curUltrasonicIdx] = timer2_ovf;
	}
	else {
		int16_t countDiff = TCNT2 - echoStartTimeArr[curUltrasonicIdx];
		int16_t ovfDiff = (int16_t)timer2_ovf - (int16_t)timer2_start_ovf_cnt[curUltrasonicIdx];
		if(ovfDiff < 0 ) {
			ovfDiff += MAX_UINT8_COUNT;
		}
		
		if (countDiff < 0) {
			countDiff +=MAX_UINT8_COUNT;
			ovfDiff--;
		}
		
		ultrasonic_dist[curUltrasonicIdx] = countDiff + ovfDiff * MAX_UINT8_COUNT;
		bEchoActive = FALSE;
	}
}



// ultrasonic 초기화
void init_ultrasonic() {
	
	// trigger는 출력
	// echo는 입력
	TRIG_DDR |= 1 << TRIG_LEFT | 1 << TRIG_RIGHT | 1 << TRIG_FORWARD;   
	ECHO_DDR &= ~(1 << ECHO_LEFT | 1 << ECHO_RIGHT | 1 << ECHO_FORWARD);
	
	
	// EICRB 레지스터 참고!
	// ISC 비트 설정에 따른 이터럽트 발생 시점 
	//apge 287 12-6
	// 0 1 : 상승 에지 ( rising edge) 하강 (falling edge) 둘 다 INT를 띄우도록
	EICRB |= 0 << ISC41 | 1 << ISC40;
	EICRB |= 0 << ISC51 | 1 << ISC50;
	EICRB |= 0 << ISC61 | 1 << ISC60;
	
	ULTRASONIC_TIMER_TCCR &= ~((1 << CS22) | (1 << CS21) | (1 << CS20)); // 모두 클리어
	ULTRASONIC_TIMER_TCCR |= (1 << CS21) | (1 << CS20);
	preScale = 64;
	
	// 문 열어놓기
	// 287 page 12-6
	// EIMSK(External Interrupt Mask Register)은 외부 인터럽트를 활성화 & 비활성화하는 레지스터
	EIMSK |= 1 << INT4 | 1 << INT5 | 1 << INT6;
	
	// timer2 overflow INT 허용 ( enable )
	TIMSK |= 1 << TOIE2; // TIMSK |= 0x01;
}

void trigger_ultrasonic(uint8_t inTriggerIdx) {
	
	uint8_t curTriggerPin = 0;
	
	if(inTriggerIdx == ULTRASONIC_LEFT) {
		curTriggerPin = TRIG_LEFT;
	}
	else if(inTriggerIdx == ULTRASONIC_FORWARD) {
		curTriggerPin = TRIG_FORWARD;
	}
	else if(inTriggerIdx == ULTRASONIC_RIGHT) {
		curTriggerPin = TRIG_RIGHT;
	}
	else {
		printf("invalid trigger pin %d\r\n", inTriggerIdx);
		return;
	}
	
	// data reset
	//timer2_ovf_cnt[inTriggerIdx] = 0;
	//timer2_start_ovf_cnt[inTriggerIdx] = 0;
	//echoStartTimeArr[inTriggerIdx] = 0;

	
	// 트리거 날리기 :  10us 
	//TRIG_PORT &= ~(1 << TRIG_FORWARD | 1 << TRIG_RIGHT | 1 << TRIG_LEFT);
	TRIG_PORT &= ~(1 << curTriggerPin);
	
	_delay_us(1);
	
	// trigger port(4) high로 변경 ( trigger 발생 시작) 
	//TRIG_PORT |= 1 << TRIG_FORWARD | 1 << TRIG_RIGHT | 1 << TRIG_LEFT;
	TRIG_PORT |= 1 << curTriggerPin;
	
	// 최소 10us 만큼 trig를 high로 둬야함
	_delay_us(15); 
	
	// trigger port 다시 low 로 변경 ( trigger 종료 )
	//TRIG_PORT &= ~(1 << TRIG_LEFT | 1 << TRIG_RIGHT | 1 << TRIG_FORWARD);
	TRIG_PORT &= ~(1 << curTriggerPin);
	
}

void update_ultrasonic() {
	
	// 트리거 텀이 끝나면 다음 트리거 발동
	if(ultrasonic_check_timer > 0) {
		return;
	}
	
	if(bDistancePending) {
		return;
	}
	
	bEchoActive = TRUE;
	bDistancePending = TRUE;
	
	// 다음 스텝 트리거 발동
	trigger_ultrasonic(triggerStep);
	
}

uint8_t get_ultrasonic_distance(int outDistances[]) {

	// 트리거는 한번에 하나씩만 날림
	// 진행중인 트리거가 있다면 트리거를 날리지 않음
	if(bEchoActive) {
		return FAILED;
	}
	
	if(!bDistancePending) {
		return FAILED;
	}
	
	int distance = ultrasonic_dist[triggerStep] * 686L / 10000;
	//float distance = (ultrasonic_dist[triggerStep] * 1000000.0 * 64 / F_CPU) * SPEED_OF_SOUND / 2;
				
	const uint8_t preStep = triggerStep;			
	triggerStep = (triggerStep + 1) % ULTRASONIC_LENGTH; // next trigger
	
	bDistancePending = FALSE;
	ultrasonic_check_timer = TRIGGER_SEND_DELAY;
	
	if(distance < 0) {
		return FAILED;
	}
	
	// max값 보정
	if(distance > MAX_DISTANCE) {
		distance  = MAX_DISTANCE;
	}
	outDistances[preStep] = distance;  
	
	return SUCCESS;
}


void set_trigger_low(const uint8_t inTriggerIdx) {
	
	uint8_t curTriggerPin = 0;
	
	if(inTriggerIdx == ULTRASONIC_LEFT) {
		curTriggerPin = TRIG_LEFT;
	}
	else if(inTriggerIdx == ULTRASONIC_FORWARD) {
		curTriggerPin = TRIG_FORWARD;
	}
	else if(inTriggerIdx == ULTRASONIC_RIGHT) {
		curTriggerPin = TRIG_RIGHT;
	}
	
	TRIG_PORT &= ~(1 << curTriggerPin);
}		

void set_trigger_high(const uint8_t inTriggerIdx) {
	
	uint8_t curTriggerPin = 0;
	
	if(inTriggerIdx == ULTRASONIC_LEFT) {
		curTriggerPin = TRIG_LEFT;
	}
	else if(inTriggerIdx == ULTRASONIC_FORWARD) {
		curTriggerPin = TRIG_FORWARD;
	}
	else if(inTriggerIdx == ULTRASONIC_RIGHT) {
		curTriggerPin = TRIG_RIGHT;
	}
	
	TRIG_PORT |= (1 << curTriggerPin);
}
	






