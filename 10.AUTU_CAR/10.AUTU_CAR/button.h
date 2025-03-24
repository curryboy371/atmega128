/*
 * button.h
 *
 * Created: 2025-03-05 오후 2:30:36
 *  Author: microsoft
 */ 


#ifndef BUTTON_H_
#define BUTTON_H_








#define BUTTON_DDR			DDRE
#define BUTTON_PIN			PINE		

// button to port num
#define BUTTON0PIN			7	// PORTC.7

// button num define
#define BUTTON0				0	// PORTC.7의 가상의 index(S/w번호)
#define BUTTON_NUM			1	// 버튼의 개수

#define BUTTON_PRESS		1	// 버튼 누르면 high( active-high)
#define BUTTON_RELEASE		0	// 버튼 떼면 low


void init_button(void);
void init_button_INT();
	

#endif /* BUTTON_H_ */