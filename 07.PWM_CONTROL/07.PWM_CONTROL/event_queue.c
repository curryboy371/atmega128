/*
 * event_queue.c
 *
 * Created: 2025-03-15 오후 10:04:50
 *  Author: pscsp
 */ 

#include "event_queue.h"
#include <util/atomic.h>  // ATOMIC_BLOCK

volatile Event_t eventQueue[QUEUE_SIZE];  // 이벤트 저장 공간
volatile uint8_t eventQFront = 0;
volatile uint8_t eventQRear = 0;
volatile uint8_t eventQCount = 0;


void init_event_queue(void) {
	
	eventQFront = eventQRear = eventQCount = 0;
}


uint8_t push_event_queue(Event_t inEvent) {
	
	// atomic하게 실행
	// 블록 내에서 새로운 인터럽트 실행 금지, 블록 후 인터럽트 실행
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		 
		if(eventQCount >= QUEUE_SIZE) {
			
			printf("error event queue is full!!!\r\n");
			return EQ_FAIL;
		}
		
		eventQueue[eventQRear++] = inEvent;
		eventQRear %= QUEUE_SIZE;
		++eventQCount;
		return EQ_SUCCESS;
	}
	
}


uint8_t pop_event_queue(Event_t **ppEvent) {
	
	// atomic하게 실행
	// 블록 내에서 새로운 인터럽트 실행 금지, 블록 후 인터럽트 실행
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
	
		if (eventQCount == 0) {
			printf("Queue is empty! count=%d\r\n", eventQCount);
			return 0;  // 큐가 비어 있음
		}
		
		*ppEvent = &eventQueue[eventQFront++];
		eventQFront %= QUEUE_SIZE;
		--eventQCount;
		return EQ_SUCCESS;
	}
}


uint8_t is_empty_event_queue() {
	
	if(eventQCount <= 0) {
		return 1;
	}
	
	return 0;
}

