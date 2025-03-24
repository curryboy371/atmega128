/*
 * car.c
 *
 * Created: 2025-03-22 오후 4:15:00
 *  Author: pscsp
 */ 

#include "car.h"
#include "def.h"
#include "event_queue.h"
#include "extern.h"

#include "speaker.h"
#include "led.h"
#include "button.h"
#include "ultrasonic.h"
#include "fnd.h"
#include "I2C_LCD.h"
#include "pwm.h"


volatile int turn_delay_count;
volatile int dir_delay_count;
volatile int back_delay_count;

int distances[ULTRASONIC_LENGTH];

uint8_t bAutoMode = 0;

void init_car() {
	
	init_L298N();
	init_button();
	init_speaker();
		
	init_ultrasonic();
		
	init_fnd();
	init_led();
		
}

void init_post_car() {
	I2C_LCD_init();
	
	start_manual_mode();
}

void update_car() {

	update_event();

	if(bAutoMode) {
		update_ultrasonic();
		update_auto_direction();
	}
	else {
		update_manual_direction();
	}
	
}

void operate_car() {
	
	if(bAutoMode) {
		operate_auto_car();
		
		uint8_t curSoundType = get_cur_sound_type();
		if(curSoundType) {
			led_on();
		}
		else {
			led_off();
		}
	}
	else {
		operate_manual_car();
	}

	fnd_display(bAutoMode);
	
	if(play_sound()) {
		;
	}
		
}


void update_event() {
	
	// event 처리
	if(is_empty_event_queue()) {
		return;
	}
	
	t_Event* pEvent = NULL;
		
	if(EQ_SUCCESS == pop_event_queue(&pEvent) || pEvent) {
			
		//printf("pop event type : %d data : %d\r\n", pEvent->type, pEvent->data);
		switch(pEvent->type) {
			case EEVENT_BUTTON_PRESS:
			input_button();
			break;
				
			default:
			break;
		}
	}
	
}

void update_manual_direction() {
	
	switch(bt_data) {
		case 'F':
		case 'f':
		eNewDirection = EDIR_FOWARD;
		break;
			
		case 'B':
		case 'b':
		eNewDirection = EDIR_BACKWARD;
		break;
			
		case 'R':
		case 'r':
		eNewDirection = EDIR_RIGHT;
		break;
			
		case 'L':
		case 'l':
		eNewDirection = EDIR_LEFT;
		break;
			
		case 'S':
		case 's':
		eNewDirection = EDIR_STOP;
		break;
			
		default:
		break;
	}
}

void update_auto_direction() {
	
	if(back_delay_count > 0) {
		return;
	}
	
	// 이거를 전역으로 빼서 이전값을 계속 유지해야함
	if(get_ultrasonic_distance(distances) == SUCCESS) {
		
		// 전방이 막히면
		if(distances[ULTRASONIC_FORWARD] < DISTANCE) {
			
			//if(turn_delay_count <= 0) {
			//	turn_delay_count = 0;
			//}
			
			if(distances[ULTRASONIC_LEFT] > distances[ULTRASONIC_RIGHT]) {

				eNewDirection = EDIR_LEFT;
				if(distances[ULTRASONIC_LEFT] < TURN_DISTANCE) {
					eNewDirection = EDIR_BACKWARD;
				}
			}
			else { // 우측이 더 크니 우회전
				
				eNewDirection = EDIR_RIGHT;
				if(distances[ULTRASONIC_RIGHT] < TURN_DISTANCE) {
					eNewDirection = EDIR_BACKWARD;
				}
			}
			
			//if(turn_delay_count > 0 || dir_delay_count > 0) {
			//	eNewDirection = EDIR_STOP;
			//}
		}
		else {
			
			eNewDirection = EDIR_FOWARD;
			
			if(CRITICAL_DISTANCE > distances[ULTRASONIC_RIGHT] && distances[ULTRASONIC_LEFT] > TURN_DISTANCE) {
				eNewDirection = EDIR_LEFT;
			}
			
			if(CRITICAL_DISTANCE > distances[ULTRASONIC_LEFT] && distances[ULTRASONIC_RIGHT] > TURN_DISTANCE) {
				eNewDirection = EDIR_RIGHT;
			}
		}
		
		//printf("r=%d f=%d l=%d dir=%d \r\n", distances[ULTRASONIC_RIGHT], distances[ULTRASONIC_FORWARD], distances[ULTRASONIC_LEFT], eNewDirection);
	}
}

void operate_manual_car() {
	
	if(eNewDirection == eCurDirection) {
		return;
	}
	
	switch(eNewDirection) {
		case EDIR_FOWARD:
		L298N_forward(500); // 펄스1 = 4us * 500 = 0.002sec ( 2ms)
		break;
		
		case EDIR_BACKWARD:
		L298N_backward(500); // 펄스1 = 4us * 500 = 0.002sec ( 2ms)
		break;
		
		case EDIR_RIGHT:
		L298N_turn_right(500); // 펄스1 = 4us * 500 = 0.002sec ( 2ms)
		break;
		
		case EDIR_LEFT:
		L298N_turn_left(500); // 펄스1 = 4us * 500 = 0.002sec ( 2ms)
		break;
		
		case EDIR_STOP:
		L298N_stop(); // 펄스1 = 4us * 500 = 0.002sec ( 2ms)
		break;
		
		default:
		break;
	}
}

void operate_auto_car() {
	
	if(eNewDirection == eCurDirection) {
		return;
	}
	
	switch(eNewDirection) {
		
		case EDIR_FOWARD:
		L298N_forward(400);
		break;
		
		case EDIR_BACKWARD:
		L298N_backward(400);
		break;
		
		case EDIR_RIGHT:
		L298N_turn_right(1200);
		break;
		
		case EDIR_LEFT:
		L298N_turn_left(1200);
		break;
		
		case EDIR_STOP:
		L298N_stop();
		break;
		
		default:
		break;
	}
}

void input_button() {
	
	if(bAutoMode == AUTO_MODE) {
		
		if(eCurPlay == EMUSIC_SCHOOL_BELL) {
			change_mode();
		}
		else {
			
			start_music(++eCurPlay, PLAY_REPEAT);
			
			char buffer[12];
			sprintf(buffer, "AUTO SONG %d",eCurPlay);
			display_lcd("HY", buffer);
		}
		
	}
	else {
		change_mode();
	}
}

void change_mode() {
	
	reset_car();
	
	if(bAutoMode) {
		start_manual_mode();
	}
	else {
		start_auto_mode();
	}
}

void start_manual_mode() {
	
	bAutoMode = MANUAL_MODE;
	display_lcd("HY", "MANUAL MODE");
	led_on();
}

void start_auto_mode() {
	
	bAutoMode = AUTO_MODE;
	char buffer[12];
	start_music(EMUSIC_AMBULANCE, PLAY_REPEAT);
	sprintf(buffer, "AUTO SONG %d", eCurPlay);
	display_lcd("HY", buffer);
}

void display_lcd(const char* inFirstLine, const char* inSecLine) {
	//I2C_LCD_clear();
	I2C_LCD_write_string_XY(0,0,inFirstLine);
	I2C_LCD_write_string_XY(1,0,inSecLine);
}

void reset_car() {
	bt_data = ' ';
	eNewDirection = EDIR_STOP;
	eCurDirection = EDIR_STOP;
	L298N_stop();
	stop_sound();
	led_off();
	
	//I2C_LCD_clear();
}

