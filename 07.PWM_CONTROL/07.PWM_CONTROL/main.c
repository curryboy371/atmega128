/*
 * 01.LED_CONTROL.c
 *
 * Created: 2025-03-04 오후 4:25:34
 * Author : microsoft
 */ 

#define  F_CPU 16000000UL  // 16MHZ
#include <avr/io.h>
#include <util/delay.h>  // _delay_ms _delay_us

#include <avr/interrupt.h> // interrupt

#include <stdlib.h>
#include <stdio.h> // printf scanf fgets puts gets...

#include "common.h"
#include "washing_machine.h"

#include "event_queue.h"
#include "redefine.h"

#include "uart0.h"
#include "fnd.h"
#include "led.h"
#include "button.h"
#include "ultrasonic.h"
#include "pwm.h"
#include "speaker.h"


#define SEC_TIME 1000
#define DEF_FPS		1

void init_timer0();

void init_machine();
void update_machine();
void cycle_machine();

void update_time(); 

// UART0_transmit 함수 mapping해줘야함
// setup write 할 때 
FILE OUTPUT = FDEV_SETUP_STREAM(UART0_transmit, NULL, _FDEV_SETUP_WRITE);

// volatile 키워드
// 해당 변수에 대하여 컴파일러 최적화를 방지함
volatile int msec_count = 0;
volatile int ultrasonic_check_timer = 0;
volatile int ultrasonic_trigger_count = 0;

volatile int msec_button_delay = 0;

volatile uint8_t g_bIsStop;

volatile uint8_t g_fps;

int sec_count;


extern t_WashingManager g_washingMgr;

extern volatile uint32_t music_count;
extern volatile uint32_t button_play_count;
extern volatile uint8_t buttons_state;

extern volatile uint8_t rx_msg_received;




// 인터럽트 함수는 main 함수 이전에서 세팅하는게 규칙
// ISR의 name은 page 274 int table 참고!

// ISR : Interrupt Service Routine ( 함수라고 생각하면 됨 ) 
// --> H/W가 SW에게 event(상황 변화)가 일어났다고 알려주는 공간
// 250개의 pulse를 count (1ms)하면 이곳으로 자동적으로 들어옴
// ISR 루틴은 가능한 빠르게 동작하도록 짜야함!!



ISR(TIMER0_OVF_vect)
{
	// 256이 되면 overflow
	// 6~256 : 250 카운트하면 1ms 가 되므로 TCT0을 6으로 설정한 것
	TCNT0 = 6; // 초기화를 직접 해줘야하나봄
	
	++g_fps;
	
	if(!g_bIsStop)
	{
		++msec_count;
	}
	
	++ultrasonic_check_timer;
	if(ultrasonic_trigger_count > 0) {
		--ultrasonic_trigger_count;
	}
	
	if(button_play_count > 0) {
		
		--button_play_count;
		if(button_play_count == 0) {
			//flag
			//set_play_button(PLAY_OFF);
			//led_off(LED_INPUT);
		}
		
	}
	
	if(music_count > 0) {
		--music_count;
	}
	
	
	if(msec_button_delay > 0) {
		--msec_button_delay;
	}
}

int main(void)
{
	//init
	init_machine();
	
	//start_error(666);
	L298N_run(250);
	while(1) 
	{
		
		Beep(5);
		
		//if(g_fps >= DEF_FPS) {
		//	g_fps  = 0;
		//	update_machine();
		//	cycle_machine();
		//}
	}
	
}

// timer0을 초기화 시키는 함수
void init_timer0()
{
	TCNT0 = 6;
	
	// 분주비 설정  64분주  250,000Mz ( 250Khz )
	// 296 page cs0n에 따른 클록 선택 참고
	TCCR0 |= 1 << CS02 | 0 << CS01 | CS00; // TCCR0 |= 0xf4
	
	
	// timer0 overflow INT 허용 ( enable )
	TIMSK |= 1 << TOIE0; // TIMSK |= 0x01;

	
}

// device 초기화
void init_machine()
{
	init_uart0();
	init_timer0();
	init_ultrasonic();
	init_speaker();
	init_fnd();
	init_led();
	init_button();
	init_L298N();
	I2C_LCD_init();
	stdout = &OUTPUT; // printf가 동작 될 수 있도록 stdout에 output
	
	
	init_event_queue();
	
	// 머신 초기화
	init_washing_machine();
	
	
	// 전역 인터럽트 허용
	// 모든 INT가 초기화 되고 전역 인터럽트를 허용해야함
	// 그냥 초기화 모두 끝나면 허용하는거로...
	sei();
	
	printf("##### START UART #####\r\n");
	
}

void update_machine()
{
	update_time();	
	update_washing_machine();
	
}

void update_time()
{
	if(g_bIsStop)
	{
		return;
	}
	
	if(msec_count >= SEC_TIME)
	{
		msec_count = 0;
		++sec_count;
		
		if(g_washingMgr.bIsRun == 1) {
			--g_washingMgr.remainSecond;
			++g_washingMgr.progressSecond;
		}
	}
}


void cycle_machine()
{
	operate_washing_machine();
}

