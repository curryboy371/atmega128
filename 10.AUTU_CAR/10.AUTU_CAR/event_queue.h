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

typedef enum {
	EEVENT_BUTTON_PRESS,
} t_EEventType;

// 이벤트 구조체 정의
typedef struct {
	t_EEventType type;
	uint8_t data;  // 추가 정보 (예: 버튼 번호, 모드 값 등)
} t_Event;


// 큐 관련 함수
void init_event_queue(void);
uint8_t push_event_queue(t_Event inEvent);
uint8_t pop_event_queue(t_Event **ppEvent);

uint8_t is_empty_event_queue();


#endif /* EVENT_QUEUE_H_ */