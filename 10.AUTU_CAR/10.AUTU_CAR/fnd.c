/*
 * fnd.c
 *
 * Created: 2025-03-06 오후 12:23:40
 *  Author: microsoft
 */ 


#define F_CPU 16000000UL	// 16MHZ 설정

#include <util/atomic.h>

#include <avr/io.h>
#include <util/delay.h> // for _delay_ms _delay_us ...
#include <avr/interrupt.h>
#include <string.h>


#include "fnd.h"
#include "extern.h"

#include "button.h"
#include "pwm.h"



void init_fnd()
{
	FND_DATA_DDR |= 0xff;
	FND1_DIGIT_DDR |= 1 << FND1_DIGIT_D1 | 1 << FND1_DIGIT_D2 | 1 << FND1_DIGIT_D3 | 1 << FND1_DIGIT_D4;
	
	// fnd off
	FND_DATA_PORT = FND_NULL;
}


void set_fnd1_digit_port(uint8_t inDigitPortNum) {
	
	// 포트가 led등 다른 것과 섞여있음
	
	// 기존 데이터 비워줘야 잔상이 안남음
	FND_DATA_PORT = FND_NULL;
	
#ifdef FND_COMMON_ANODE

	// FND1의 digit을 초기화 후 새로운 digit을 설정 (한 줄로 처리)
	FND1_DIGIT_PORT = (FND1_DIGIT_PORT & ~(1 << FND1_DIGIT_D1 | 1 << FND1_DIGIT_D2 | 1 << FND1_DIGIT_D3 | 1 << FND1_DIGIT_D4)) | (1 << inDigitPortNum);
#else
	// FND1의 digit 초기화 후 새로운 digit 설정 (한 줄로 처리)
	FND1_DIGIT_PORT = (FND1_DIGIT_PORT | (1 << FND1_DIGIT_D1 | 1 << FND1_DIGIT_D2 | 1 << FND1_DIGIT_D3 | 1 << FND1_DIGIT_D4)) & ~(1 << inDigitPortNum);
#endif
	
}

// param inDigitNum 0~9 숫자 입력
// return fnd 모드에 따른 fnd number value 출력
uint8_t get_digit_num(uint8_t inDigitNum)
{
	static const uint8_t fnd_font[10] = {
		FND_NUM_0, FND_NUM_1, FND_NUM_2, FND_NUM_3, FND_NUM_4, 
		FND_NUM_5, FND_NUM_6, FND_NUM_7, FND_NUM_8, FND_NUM_9};

    if (inDigitNum >= 10) 
	{
	    return FND_NUM_0;
    }

	return fnd_font[inDigitNum];
}

void fnd_display(uint8_t inbAutoMode) {

	static t_EFND_Digit_All eDigitSelect = EFND_Digit_All_MAX;
	
	if(inbAutoMode) {
		fnd_auto_mode(eDigitSelect);
	}
	else {
		fnd_manual_mode(eDigitSelect);
	}
	
	eDigitSelect = (eDigitSelect + 1) % EFND_Digit_All_MAX;
}

void fnd_manual_mode(t_EFND_Digit_All inDigitSelect) {
	
	
	switch(inDigitSelect)
	{
		case EFND1_Digit_D4:
		set_fnd1_digit_port(FND1_DIGIT_D4);
		FND_DATA_PORT = get_digit_num(eCurDirection);
		break;
		
		case EFND1_Digit_D3:
		set_fnd1_digit_port(FND1_DIGIT_D3);
		FND_DATA_PORT = get_digit_num(eCurDirection);
		break;
		
		case EFND1_Digit_D2:
		set_fnd1_digit_port(FND1_DIGIT_D2);
		FND_DATA_PORT = get_digit_num(eCurDirection);
		break;
		
		case EFND1_Digit_D1:
		set_fnd1_digit_port(FND1_DIGIT_D1);
		FND_DATA_PORT = get_digit_num(eCurDirection);
		break;
		
		default:
		break;
	}
	
}

void fnd_auto_mode(t_EFND_Digit_All inDigitSelect) {
	
	switch(inDigitSelect)
	{
		case EFND1_Digit_D4:
		set_fnd1_digit_port(FND1_DIGIT_D4);
		FND_DATA_PORT = get_digit_num(eCurDirection);
		break;
		
		case EFND1_Digit_D3:
		set_fnd1_digit_port(FND1_DIGIT_D3);
		FND_DATA_PORT = get_digit_num(eCurDirection);
		break;
		
		case EFND1_Digit_D2:
		set_fnd1_digit_port(FND1_DIGIT_D2);
		FND_DATA_PORT =	FND_NULL;
		break;
		
		case EFND1_Digit_D1:
		set_fnd1_digit_port(FND1_DIGIT_D1);
		FND_DATA_PORT =	get_digit_num(eCurPlay);
		break;
		
		default:
		break;
	}
}
