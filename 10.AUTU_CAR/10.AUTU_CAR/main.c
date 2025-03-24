/*
 * 10.AUTO_CAR.c
 *
 * Created: 2025-03-04 오후 4:25:34
 * Author : microsoft
 */ 

#define F_CPU 16000000UL	// 16MHZ 설정
#include <avr/io.h>
#include <util/delay.h> // for _delay_ms _delay_us ...
#include <avr/interrupt.h>
#include <stdio.h>
#include <string.h>

#include "def.h"
#include "extern.h"

#include "event_queue.h"
#include "systick.h"

#include "uart0.h"
#include "uart1.h"

#include "car.h"


void init_timer0();

void init_device();
void init_post_device();

void update_device();
void operate_device();


#include "I2C_LCD.h"
// UART0_transmit 함수 mapping해줘야함
// setup write 할 때 
FILE OUTPUT = FDEV_SETUP_STREAM(UART0_transmit, NULL, _FDEV_SETUP_WRITE);

int main(void)
{
	init_device();
	init_post_device();
	
	while(TRUE) {
		if(g_fps >= DEF_FPS) {
			g_fps = 0;
			update_device();
			operate_device();
		}
	}
}

// device 초기화
void init_device()
{
	//system init
	init_systick();
	init_uart0();
	init_uart1();
	stdout = &OUTPUT; // printf가 동작 될 수 있도록 stdout에 output
	printf("Init uart \r\n");
	
	init_event_queue();
	init_car();
	
	// 모든 INT가 초기화 되고 전역 인터럽트를 허용해야함
	// 전역 인터럽트 허용
	sei();

	printf("Init complete\r\n");
}

void init_post_device() {
	
	init_post_car();
	printf("Init post complete\r\n");
}

void update_device()
{
	update_tick();
	update_car();
}

void operate_device() {
	
	operate_car();
}