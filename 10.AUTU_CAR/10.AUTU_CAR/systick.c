/*
 * systick.c
 *
 * Created: 2025-03-22 오후 5:10:51
 *  Author: pscsp
 */ 


#define F_CPU 16000000UL	// 16MHZ 설정
#include <avr/io.h>
#include <util/delay.h> // for _delay_ms _delay_us ...
#include <avr/interrupt.h>

#include "systick.h"
#include "extern.h"

#include "I2C.h"

volatile int msec_count;
volatile int g_fps;

int sec_count;

ISR(TIMER0_OVF_vect)
{
	TCNT0 = 6;
	
	++msec_count;
	
	if(msec_button_delay > 0) {
		--msec_button_delay;
	}
	
	if(turn_delay_count > 0) {
		--turn_delay_count;
	}
	
	if(dir_delay_count > 0) {
		--dir_delay_count;
	}
	
	if(back_delay_count >0) {
		--back_delay_count;
	}
	
	if(play_count > 0) {
		--play_count;
	}

	if(ultrasonic_check_timer > 0) {
		--ultrasonic_check_timer;
	}
	
	if(I2C_timeOut > 0) {
		--I2C_timeOut;
		
		if(I2C_timeOut == 0) {
			
			set_write_done_flag(TRUE);
		}
	}

	++g_fps;
}

void init_systick()
{
	TCNT0 = 6;
	
	// 분주비 설정  64분주  250,000Mz ( 250Khz )
	TCCR0 |= 1 << CS02 | 0 << CS01 | CS00; // TCCR0 |= 0xf4
	
	// timer0 overflow INT 허용 ( enable )
	TIMSK |= 1 << TOIE0; // TIMSK |= 0x01;
}


void update_tick()
{
	
	if(msec_count >= 1000)
	{
		msec_count = 0;
		++sec_count;
		
	}
}


void reset_time() {
	
	sec_count = msec_count = 0;
}