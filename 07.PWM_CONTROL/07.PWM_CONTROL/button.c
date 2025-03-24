/*
 * button.c
 *
 * Created: 2025-03-05 오후 2:30:27
 *  Author: microsoft
 */ 

#include "button.h"
#include "event_queue.h"
#include "def.h"

#include "led.h"

// initialize button  ( 방향 설정 : 입력 )
// PORTD 76543210
// 


extern volatile int msec_button_delay;

volatile uint8_t buttons_state;

// External Interrupt button 0
ISR(INT0_vect) {

	// 노이즈 or 중복 클릭 방지
	if(msec_button_delay > 0) {
		return;
	}

	// evnet는 지역변수이므로 isr이어도 volatile 사용하지 않을거임
	Event_t eventValue;
	eventValue.type = EEVENT_BUTTON_PRESS;
	eventValue.data = BUTTON0;
	
	push_event_queue(eventValue);
	
	
	msec_button_delay = BUTTON_DELAY;
}

// External Interrupt button 1
ISR(INT1_vect) {

	// 노이즈 or 중복 클릭 방지
	if(msec_button_delay > 0) {
		return;
	}

	// evnet는 지역변수이므로 isr이어도 volatile 사용하지 않을거임
	Event_t eventValue;
	eventValue.type = EEVENT_BUTTON_PRESS;
	eventValue.data = BUTTON1;
	
	push_event_queue(eventValue);

	
	msec_button_delay = BUTTON_DELAY;
}

// External Interrupt button 2
ISR(INT4_vect) {

	// 노이즈 or 중복 클릭 방지
	if(msec_button_delay > 0) {
		return;
	}

	// evnet는 지역변수이므로 isr이어도 volatile 사용하지 않을거임
	Event_t eventValue;
	eventValue.type = EEVENT_BUTTON_PRESS;
	eventValue.data = BUTTON2;
	
	push_event_queue(eventValue);
	
	msec_button_delay = BUTTON_DELAY;
}

// External Interrupt button 3
ISR(INT5_vect) {

	// 노이즈 or 중복 클릭 방지1
	if(msec_button_delay > 0) {
		return;
	}

	// evnet는 지역변수이므로 isr이어도 volatile 사용하지 않을거임
	Event_t eventValue;
	eventValue.type = EEVENT_BUTTON_PRESS;
	eventValue.data = BUTTON3;
	
	push_event_queue(eventValue);
	
	msec_button_delay = BUTTON_DELAY;
}


// button에 연결된 external interrupt 0~3을 초기화
void init_button_INT() {
	
	// int 활성화
	EIMSK |= 1 << INT0 | 1 << INT1 | 1 << INT4 | 1 << INT5;
	
	 // (버튼을 누를 때 인터럽트 발생)
	 
	 
	// INT0, INT1: 상승 에지
	EICRA |= (1 << ISC01) | (1 << ISC00) |  // INT0: 상승 에지 감지
	(1 << ISC11) | (1 << ISC10);   // INT1: 상승 에지 감지

	// INT4, INT5: 상승 에지
	EICRB |= (1 << ISC41) | (1 << ISC40) |  // INT4: 상승 에지 감지
	(1 << ISC51) | (1 << ISC50);   // INT5: 상승 에지 감지
	
}

void init_button(void) 
{	
	// DDR 입출력 설정 : 버튼과 연결된 핀 입력으로 
	BUTTONA_DDR &= ~(1 << BUTTON0PIN | 1 << BUTTON1PIN);
	BUTTONB_DDR &= ~(1 << BUTTON2PIN | 1 << BUTTON3PIN);
	

	init_button_INT();
	
}

