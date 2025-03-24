/*
 * washing_machine.c
 *
 * Created: 2025-03-15 오후 9:54:55
 *  Author: pscsp
 */ 

#include "washing_machine.h"
#include "event_queue.h"

#include "redefine.h"

#include "common.h"

#include "button.h"
#include "led.h"
#include "speaker.h"
#include "fnd.h"
#include "pwm.h"



// function ptr

typedef bool (*inputHandler) (void);

typedef void (*operHandler)(void);


t_WashingManager g_washingMgr;

void init_washing_machine() {
	
	memset(&g_washingMgr, 0, sizeof(t_WashingManager));

}


void update_washing_machine() {
		
	if(g_washingMgr.bIsRun == 1) {
		if(g_washingMgr.remainSecond <= 0 ) {
			handle_timeout();
		}
	}
	
	update_evnet();
	
	update_ultrasonic();
}



void update_evnet() {

	//button input 
	static const inputHandler inputFuncs[BUTTON_NUM] = {
		input_button_select,
		input_button_next,
		input_button_previous,
		input_button_return,
	};
	
	Event_t* pEvent = NULL;

	if(is_empty_event_queue()) {
		
		return;
	}
	
	if(EQ_SUCCESS == pop_event_queue(&pEvent) || pEvent) {
		
		//printf("pop event type : %d data : %d\r\n", pEvent->type, pEvent->data);
		switch(pEvent->type) {
			case EEVENT_BUTTON_PRESS:

			if(get_play_state() == PLAY_OFF) {
				
				if(inputFuncs[pEvent->data]()) {
					
					play_button();
					led_on(LED_INPUT);
				}
			}
			
			break;
			
			
			default:
			break;
		}
	}
	
}

// 세탁기를 동작시키는 함수
void operate_washing_machine() {

	static const inputHandler operFuncs[EWashMode_Max] = {
		run_standby,
		run_select_mode,
		run_select_man,
		run_wash,
		run_spin,
		run_finish,
		run_error,
		run_stop,
	};
	
	operFuncs[g_washingMgr.eWashMode]();
	
}



void run_standby() {

	fnd_display(g_washingMgr.eWashMode);
}

void run_select_mode() {
	
	fnd_display(g_washingMgr.eWashMode);
}

void run_select_man() {
	
	fnd_display(g_washingMgr.eWashMode);
}


void run_wash() {
	
	if(!check_valid_distance()) {
		start_error(999);
		return;
	}
	
	// led
	if(g_washingMgr.progressSecond & 1) {
		led_on(LED_NORMAL);
	}
	else {
		led_off(LED_NORMAL);
	}
	
	fnd_display(g_washingMgr.eWashMode);

}

void run_spin() {

	if(!check_valid_distance()) {
		start_error(999);
		return;
	}

	// led
	if(g_washingMgr.progressSecond & 1) {
		led_on(LED_NORMAL);
	}
	else {
		led_off(LED_NORMAL);
	}
	
	fnd_display(g_washingMgr.eWashMode);

}

void run_finish() {
	led_on(LED_NORMAL);
	
	// 노래...
	if(play_music()) {
		reset_wash_data();
	}
	
	fnd_display(g_washingMgr.eWashMode);
}

void run_error() {
	
	if(play_music()) {
		reset_wash_data();
	}
	
	fnd_display(g_washingMgr.eWashMode);
}

void run_stop() {

	fnd_display(g_washingMgr.eWashMode);	
}

void start_wash() {
	
	printf("start wash  mode:%d pow:%d time:%d \r\n", g_washingMgr.bIsMode, g_washingMgr.washPower, g_washingMgr.washTime);
	
	g_washingMgr.bIsRun = 1;
	g_washingMgr.progressSecond = 0;
	g_washingMgr.remainSecond = g_washingMgr.washTime;
	set_wash_mode(EWashMode_Wash);
	
	L298N_run(g_washingMgr.washPower);

	
}

void start_spin() {
	
	printf("start spin  mode:%d pow:%d time:%d \r\n", g_washingMgr.bIsMode, g_washingMgr.spinPower, g_washingMgr.spinTime);
	
	g_washingMgr.bIsRun = 1;
	g_washingMgr.progressSecond = 0;
	g_washingMgr.remainSecond = g_washingMgr.spinTime;
	set_wash_mode(EWashMode_Spin);
	
	L298N_run(g_washingMgr.spinPower);
	

}


void finish_spin() {
	
	g_washingMgr.remainSecond = 0;
	g_washingMgr.progressSecond = 0;
	g_washingMgr.remainSecond = 0;
	
	
	L298N_run(0);
	
	g_washingMgr.bIsRun = 0;
	
	fnd_reset();
	
	// 노래...
	start_music(EMusic_School_Bell);
	set_wash_mode(EWashMode_Finish);
	
}


void restart_working() {
	
	
	if(g_washingMgr.ePreWashMode == EWashMode_Wash) {
		
		L298N_run(g_washingMgr.washPower);
	}
	else if(g_washingMgr.ePreWashMode == EWashMode_Spin) {
		
		L298N_run(g_washingMgr.spinPower);
	}
	
	led_off(LED_STOP);
	
	set_wash_mode(g_washingMgr.ePreWashMode);
	
	g_washingMgr.bIsRun = 1; // 다시시작
}

void stop_working() {
	
	g_washingMgr.ePreWashMode = g_washingMgr.eWashMode;
	L298N_run(0);
	led_on(LED_STOP);
		
	set_wash_mode(EWashMode_Stop);
	
	g_washingMgr.bIsRun = 0; // 일시 정지
	
}

void start_error(uint32_t inErrorCode) {
	
	g_washingMgr.ePreWashMode = g_washingMgr.eWashMode;
	
	g_washingMgr.errorValue = inErrorCode;
	L298N_run(0);
	led_all_off();
	led_on(LED_ERROR);
	set_wash_mode(EWashMode_Error);
	
	start_music(EMusic_Elise);
	
	printf("ERROR!!!! %d \r\n", inErrorCode);
}

void handle_timeout() {

	if(g_washingMgr.eWashMode == EWashMode_Wash) {
		start_spin();
	}
	else if(g_washingMgr.eWashMode == EWashMode_Spin) {
		finish_spin();
	}
	
};


void reset_wash_data() {

	g_washingMgr.bIsRun = 0;
	g_washingMgr.remainSecond = 0;
	g_washingMgr.progressSecond = 0;
	g_washingMgr.remainSecond = 0;
	
	
	L298N_run(0);
	
	led_all_off();
	
	set_wash_mode(EWashMode_Standby);
};

void set_auto_mode_data() {
	
	g_washingMgr.washTime = 10;
	g_washingMgr.spinTime = 5;
	
	g_washingMgr.washPower = 250;
	g_washingMgr.spinPower = 250;
}

void set_man_mode_data() {
	
	switch(g_washingMgr.eSelectType) {
		
		case ESelect_WashTime:
		g_washingMgr.washTime = g_washingMgr.curInputValue;
		//memcpy(&g_washingMgr.washTime, &g_washingMgr.curInputValue, sizeof(g_washingMgr.washTime));
		//printf("st %d %d\r\n", g_washingMgr.curInputValue, g_washingMgr.washTime);
		break;
		
		case ESelect_WashPower:
		g_washingMgr.washPower = g_washingMgr.curInputValue;
		//printf("set man mode data ESelect_WashPower %d %d \r\n", g_washingMgr.curInputValue, g_washingMgr.washPower);
		break;
		
		case ESelect_SpinPower:
		g_washingMgr.spinPower = g_washingMgr.curInputValue;
		//printf("set man mode data ESelect_SpinPower %d %d \r\n", g_washingMgr.curInputValue, g_washingMgr.spinPower);
		break;
		
		case ESelect_SpinTime:
		g_washingMgr.spinTime = g_washingMgr.curInputValue;
		//printf("set man mode data ESelect_SpinTime %d %d \r\n", g_washingMgr.curInputValue, g_washingMgr.spinTime);
		break;
		
		default:
		break;
		
	}
	
	// 디폴트값으로
	g_washingMgr.curInputValue = 30;
}

void set_wash_mode(t_EWash_Mode inMode) {
	
	t_EWash_Mode preMode = g_washingMgr.eWashMode;
	g_washingMgr.eWashMode = inMode;
	printf("wash mode change %d to %d \r\n", preMode, g_washingMgr.eWashMode);
}

uint8_t check_valid_distance() {
	
	uint8_t distance = 0;
	if(distance_ultrasonic(&distance)) {
		
		if(distance > 10) {
			printf("check distance %d \r\n", distance);
			return 0;
		}
	}
	
	return 1;
}


bool input_button_select() {
	
	bool bRes = false;
	
	// 현재 모드를 확인하고
	switch(g_washingMgr.eWashMode) {
			
		case EWashMode_Standby:
		set_wash_mode(EWashMode_Select_Auto);
		bRes = true;
		break;
			
		case EWashMode_Select_Auto:
		if(g_washingMgr.bIsMode == 0) {
			set_auto_mode_data();
			start_wash();
		}
		else {
			g_washingMgr.eSelectType = ESelect_WashTime;
			g_washingMgr.curInputValue = 30;
			set_wash_mode(EWashMode_Select_Man);
		}

		bRes = true;
		break;
		
		case EWashMode_Select_Man:
		set_man_mode_data();
		if(++g_washingMgr.eSelectType == ESelect_Max) {
			start_wash();
		}
		
		bRes = true;
		break;
			
		case EWashMode_Wash:
		//printf("mode change %d\r\n", g_washingMgr.eWashMode);
		//bRes = true;
		break;
			
		case EWashMode_Spin:
		break;
			
		case EWashMode_Error:
		break;
			
		case EWashMode_Stop:
		restart_working();
		bRes = true;
		break;
			
	}
	
	return bRes;
}


bool input_button_next() {
	
	bool bRes = false;
		
	// 현재 모드를 확인하고
	switch(g_washingMgr.eWashMode) {
			
		case EWashMode_Standby:
		break;
			
		case EWashMode_Select_Auto:
		g_washingMgr.bIsMode = !g_washingMgr.bIsMode;
		
		printf("select mode change %d\r\n", g_washingMgr.bIsMode);
		bRes = true;
		break;
		
		case EWashMode_Select_Man:
		g_washingMgr.curInputValue += 30;
		if(g_washingMgr.curInputValue >= 250) {
			g_washingMgr.curInputValue = 250;
		}
		printf("curInputValue : %d\r\n", g_washingMgr.curInputValue);
		
		bRes = true;
		break;
			
		case EWashMode_Wash:
		break;
			
		case EWashMode_Spin:
		break;
			
		case EWashMode_Error:
		break;
			
		case EWashMode_Stop:
		break;
			
	}
	
	return bRes;
}

bool input_button_previous() {
	
	//printf("input_button_previous\r\n");
	
	bool bRes = false;
	
	// 현재 모드를 확인하고
	switch(g_washingMgr.eWashMode) {
			
		case EWashMode_Standby:
		break;
			
		case EWashMode_Select_Auto:
		g_washingMgr.bIsMode = !g_washingMgr.bIsMode;
		printf("select mode change %d\r\n", g_washingMgr.bIsMode);
		bRes = true;
		break;
			
		case EWashMode_Select_Man:
		g_washingMgr.curInputValue -= 30;
		if(g_washingMgr.curInputValue <= 30) {
			g_washingMgr.curInputValue = 30;
		}
		printf("curInputValue : %d\r\n", g_washingMgr.curInputValue);
		
		bRes = true;
		break;
			
		case EWashMode_Wash:
		break;
			
		case EWashMode_Spin:
		break;
			
		case EWashMode_Error:
		break;
			
		case EWashMode_Stop:
		break;
			
	}
	
	return bRes;
}

bool input_button_return() {
		
	//printf("input_button_return\r\n");
	bool bRes = false;
	
	// 현재 모드를 확인하고
	switch(g_washingMgr.eWashMode) {
			
		case EWashMode_Standby:
		
		break;
			
		case EWashMode_Select_Auto:
		g_washingMgr.bIsMode = 0;
		set_wash_mode(EWashMode_Standby);
		bRes = true;
		break;
			
		case EWashMode_Select_Man:
		--g_washingMgr.eSelectType;
		if(g_washingMgr.eSelectType == ESelect_None) {
			set_wash_mode(EWashMode_Select_Auto);
		}
		bRes = true;
					
		break;
			
		case EWashMode_Wash:
		case EWashMode_Spin:
		stop_working();
		bRes = true;
		break;
			
		case EWashMode_Error:
		reset_wash_data();
		bRes = true;
		break;
			
		case EWashMode_Stop:
		reset_wash_data();
		bRes = true;
		break;
			
	}
	
	return bRes;
}

