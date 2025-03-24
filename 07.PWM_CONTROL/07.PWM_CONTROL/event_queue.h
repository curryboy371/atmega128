/*
 * event_queue.h
 *
 * Created: 2025-03-15 오후 10:04:58
 *  Author: pscsp
 */ 


#ifndef EVENT_QUEUE_H_
#define EVENT_QUEUE_H_

#include <stdio.h>

#define QUEUE_SIZE 50  // 큐 크기 설정

#define EQ_FAIL			0
#define EQ_SUCCESS		1

// 이벤트 타입
typedef enum {
	EEVENT_BUTTON_PRESS,
} EEventType_t;

// 이벤트 구조체 정의
typedef struct {
	EEventType_t type;
	uint8_t data;  // 추가 정보 ( 버튼 번호, 모드 값 등)
} Event_t;

// 큐 관련 함수
void init_event_queue(void);
uint8_t push_event_queue(Event_t inEvent);
uint8_t pop_event_queue(Event_t **ppEvent);

uint8_t is_empty_event_queue();


#endif /* EVENT_QUEUE_H_ */