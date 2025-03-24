/*
 * button.c
 *
 * Created: 2025-03-05 오후 2:30:27
 *  Author: microsoft
 */ 

#define F_CPU 16000000UL	// 16MHZ 설정
#include <avr/io.h>	// PORT io 관련 register 등이 있음
#include <util/delay.h> // for _delay_ms _delay_us ...
#include <avr/interrupt.h>


#include "button.h"

#include "def.h"
#include "extern.h"
#include "event_queue.h"

volatile int msec_button_delay;


ISR(INT7_vect) {

	// 노이즈 or 중복 클릭 방지
	if(msec_button_delay <= 0) {
		
		// evnet는 지역변수이므로 isr이어도 volatile 사용하지 않을거임
		t_Event eventValue;
		eventValue.type = EEVENT_BUTTON_PRESS;
		eventValue.data = BUTTON0;
			
		push_event_queue(eventValue);
			
		msec_button_delay = BUTTON_DELAY;
	}
}


void init_button(void) 
{	
	// DDR 입출력 설정 : 버튼과 연결된 핀 입력으로 
	BUTTON_DDR &= ~(1 << BUTTON0PIN);
	
	init_button_INT();
	
}


// button에 연결된 external interrupt 7 활성화
void init_button_INT() {
	
	EICRB |= (1 << ISC71) | (1 << ISC70);  // INT7: 상승 에지 감지
	EIMSK |= 1 << INT7;
	
}

