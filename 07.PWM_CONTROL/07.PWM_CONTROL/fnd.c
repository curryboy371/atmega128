/*
 * fnd.c
 *
 * Created: 2025-03-06 오후 12:23:40
 *  Author: microsoft
 */ 

#include "fnd.h"
#include "button.h"
#include "common.h"
#include "pwm.h"

extern volatile int msec_count;
extern int sec_count;

extern volatile int ultrasonic_dist;

extern t_WashingManager g_washingMgr;

 
void init_fnd()
{
	FND_DATA_DDR = 0xff; // 모두 출력 모드
	
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

void set_fnd2_digit_port(uint8_t inDigitPortNum) {
	
	// 기존 데이터 비워줘야 잔상이 안남음
	FND_DATA_PORT = FND_NULL;

#ifdef FND_COMMON_ANODE

	FND1_DIGIT_PORT &= ~(1 << FND1_DIGIT_D1 | 1 << FND1_DIGIT_D2 | 1 << FND1_DIGIT_D3 | 1 << FND1_DIGIT_D4); // 다른 fnd digit 초기화
	//FND2_DIGIT_PORT &= ~(1 << FND2_DIGIT_D1 | 1 << FND2_DIGIT_D2 | 1 << FND2_DIGIT_D3 |1 <<  FND2_DIGIT_D4); // 기존 digit 초기화
	//FND2_DIGIT_PORT |= 1 << inDigitPortNum;												 // 출력할 digit만 설정

#else

	// 기존 digit 초기화
	FND1_DIGIT_PORT |= (1 << FND1_DIGIT_D1 | 1 << FND1_DIGIT_D2 | 1 << FND1_DIGIT_D3 | 1 << FND1_DIGIT_D4);
	//FND2_DIGIT_PORT |= (1 << FND2_DIGIT_D1 | 1 << FND2_DIGIT_D2 | 1 << FND2_DIGIT_D3 | 1 << FND2_DIGIT_D4); 
	//FND2_DIGIT_PORT &= ~(1 << inDigitPortNum);											// 출력할 digit만 설정
	
	// FND1의 digit 초기화 후 새로운 digit 설정 (한 줄로 처리)
	
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

void fnd_display(t_EWash_Mode inEMode) {

	static t_EFND_Digit_All eDigitSelect = EFND_Digit_All_MAX;
	
	static int tempSpeed = 0;
	
	switch(inEMode) {
		
		// 현재 모드를 확인하고
		case EWashMode_Standby:
		fnd_standy(eDigitSelect);
		break;
					
		case EWashMode_Select_Auto:
		fnd_select_mode(eDigitSelect);
		break;
		
		case EWashMode_Select_Man:
		fnd_input_value(eDigitSelect);
		break;
					
		case EWashMode_Wash:
		case EWashMode_Spin:
		fnd_run(eDigitSelect, tempSpeed++);
		break;
		
		case EWashMode_Finish:
		fnd_finish();
		break;
		
		case EWashMode_Error:
		fnd_error(eDigitSelect);
		break;
					
		case EWashMode_Stop:
		fnd_stop(eDigitSelect);
		break;
	}

	
	eDigitSelect = (eDigitSelect + 1) % EFND_Digit_All_MAX;
	
}

void fnd_standy(t_EFND_Digit_All inDigitSelect) {
	// 표시 없음
	
	static const uint8_t standyCycle[FND_LENGTH] = { FND_A | FND_F | FND_E | FND_D,	FND_A | FND_D,	FND_A| FND_D,	FND_A | FND_B | FND_C | FND_D};

	switch(inDigitSelect)
	{
		
		case EFND1_Digit_D4:
		set_fnd1_digit_port(FND1_DIGIT_D4);
		FND_DATA_PORT = FND_NULL;
		break;
		
		case EFND1_Digit_D3:
		set_fnd1_digit_port(FND1_DIGIT_D3);
		FND_DATA_PORT = FND_NULL;
		break;
		
		case EFND1_Digit_D2:
		set_fnd1_digit_port(FND1_DIGIT_D2);
		FND_DATA_PORT = FND_NULL;
		break;
		
		case EFND1_Digit_D1:
		set_fnd1_digit_port(FND1_DIGIT_D1);
		FND_DATA_PORT = FND_NULL;
		break;
		
		default:
		break;
	}
		
}

void fnd_select_mode(t_EFND_Digit_All inDigitSelect) {
	
	switch(inDigitSelect)
	{
		case EFND1_Digit_D4:
		set_fnd1_digit_port(FND1_DIGIT_D4);
		FND_DATA_PORT = FND_NULL;
		break;
		
		case EFND1_Digit_D3:
		set_fnd1_digit_port(FND1_DIGIT_D3);
		FND_DATA_PORT = FND_NULL;
		break;
		
		case EFND1_Digit_D2:
		set_fnd1_digit_port(FND1_DIGIT_D2);
		FND_DATA_PORT = FND_NULL;
		break;
		
		case EFND1_Digit_D1:
		set_fnd1_digit_port(FND1_DIGIT_D1);
		FND_DATA_PORT = FND_NULL;
		break;
		
		
		default:
		break;
	}
	
}

void fnd_input_value(t_EFND_Digit_All inDigitSelect) {
	
	uint32_t onesValue = g_washingMgr.curInputValue % 10;
	uint8_t tensValue = (g_washingMgr.curInputValue / 10) % 10;
	uint8_t hundsValue = g_washingMgr.curInputValue / 100;	
	switch(inDigitSelect)
	{
		case EFND1_Digit_D4:
		set_fnd1_digit_port(FND1_DIGIT_D4);
		FND_DATA_PORT = get_digit_num(onesValue);
		break;
		
		case EFND1_Digit_D3:
		set_fnd1_digit_port(FND1_DIGIT_D3);
		FND_DATA_PORT = get_digit_num(tensValue);
		break;
		
		case EFND1_Digit_D2:
		set_fnd1_digit_port(FND1_DIGIT_D2);
		FND_DATA_PORT = get_digit_num(hundsValue);
		break;
		
		case EFND1_Digit_D1:
		set_fnd1_digit_port(FND1_DIGIT_D1);
		FND_DATA_PORT = FND_NULL;
		break;
		
		default:
		break;
	}
}

void fnd_run(t_EFND_Digit_All inDigitSelect, uint32_t inSpeed) {
	
	fnd_remain_time(inDigitSelect);
}

void fnd_stop(t_EFND_Digit_All inDigitSelect) {

	uint32_t remainSec = g_washingMgr.remainSecond;
		
	uint8_t seconds = remainSec % 60;		 // 초 (0~59)
	uint8_t secOnes = remainSec % 10;         // 초의 일의 자리 (0-9)
	uint8_t secTens = remainSec / 10;         // 초의 십의 자리 (0~5)

	uint8_t minutes = remainSec / 60;        // 분 (0-59)
	uint8_t minOnes = minutes % 10;          // 분-일의 자리 (0-9)
	uint8_t minTens = (minutes / 10) % 6;    // 분-십의 자리 (0~5)

	switch(inDigitSelect)
	{
		case EFND1_Digit_D4:
		set_fnd1_digit_port(FND1_DIGIT_D4);
		FND_DATA_PORT = get_digit_num(secOnes);
		break;
		
		case EFND1_Digit_D3:
		set_fnd1_digit_port(FND1_DIGIT_D3);
		FND_DATA_PORT = get_digit_num(secTens);
		break;
		
		case EFND1_Digit_D2:
		set_fnd1_digit_port(FND1_DIGIT_D2);
		FND_DATA_PORT =	get_digit_num(minOnes);
		break;
		
		case EFND1_Digit_D1:
		set_fnd1_digit_port(FND1_DIGIT_D1);
		FND_DATA_PORT =	get_digit_num(minTens);
		break;
		
		
		default:
		break;
	}
}

void fnd_error(t_EFND_Digit_All inDigitSelect) {
	
	uint32_t onesValue = g_washingMgr.errorValue % 10;
	uint8_t tensValue = (g_washingMgr.errorValue / 10) % 10;
	uint8_t hundsValue = g_washingMgr.errorValue / 100;
	
	switch(inDigitSelect)
	{
		
		case EFND1_Digit_D4:
		set_fnd1_digit_port(FND1_DIGIT_D4);
		FND_DATA_PORT = get_digit_num(onesValue);
		break;
		
		case EFND1_Digit_D3:
		set_fnd1_digit_port(FND1_DIGIT_D3);
		FND_DATA_PORT = get_digit_num(tensValue);
		break;
		
		case EFND1_Digit_D2:
		set_fnd1_digit_port(FND1_DIGIT_D2);
		FND_DATA_PORT =	get_digit_num(hundsValue);
		break;
		
		case EFND1_Digit_D1:
		set_fnd1_digit_port(FND1_DIGIT_D1);
		FND_DATA_PORT =	FND_NULL;
		break;
		
		
		default:
		break;
	}
	
	
}

void fnd_finish() {
	
	FND_DATA_PORT = FND_NULL;
	
	static uint8_t digitIdx = 0;
	static uint32_t printIdx = 0;
	
	if(printIdx > 1000) {
		
		printIdx = 0;
		digitIdx = (digitIdx + 1) % EFND_Digit_All_MAX;
	}
	
	printIdx++;
	
	switch(digitIdx)
	{
		
		case EFND1_Digit_D4:
		set_fnd1_digit_port(FND1_DIGIT_D4);
		FND_DATA_PORT = get_digit_num(0);
		break;
		
		case EFND1_Digit_D3:
		set_fnd1_digit_port(FND1_DIGIT_D3);
		FND_DATA_PORT = get_digit_num(0);
		break;
		
		case EFND1_Digit_D2:
		set_fnd1_digit_port(FND1_DIGIT_D2);
		FND_DATA_PORT =	get_digit_num(0);
		break;
		
		case EFND1_Digit_D1:
		set_fnd1_digit_port(FND1_DIGIT_D1);
		FND_DATA_PORT =	get_digit_num(0);
		break;
		
		default:
		break;
	}
		
}

void fnd_remain_time(t_EFND_Digit_All inDigitSelect) {
	
	// 배열에 cycle을 모두 저장
	const static FND_MoterCycle FND_MoterCycleInfo[FND_MOTER_CYCLE_MAX] = {
			
		//d1								d2								 d3										 d4
		{ FND_NULL,							FND_NULL,						FND_NULL,								FND_A,							},
		{ FND_NULL,							FND_NULL,						FND_A,									FND_A,							},
		{ FND_NULL,							FND_A,							FND_A,									FND_A,							},
		{ FND_A ,							FND_A,							FND_A,									FND_A,							},
		{ FND_A | FND_F	,					FND_A,							FND_A,									FND_A,							},
		{ FND_A | FND_F | FND_E,			FND_A,							FND_A,									FND_A,							},
		{ FND_A | FND_F | FND_E | FND_D,	FND_A,							FND_A,									FND_A,							},
		{ FND_A | FND_F | FND_E | FND_D,	FND_A | FND_D,					FND_A,									FND_A,							},
		{ FND_A | FND_F | FND_E | FND_D,	FND_A | FND_D,					FND_A| FND_D,							FND_A,							},
		{ FND_A | FND_F | FND_E | FND_D,	FND_A | FND_D,					FND_A| FND_D,							FND_A | FND_D,					},
		{ FND_A | FND_F | FND_E | FND_D,	FND_A | FND_D,					FND_A| FND_D,							FND_A | FND_D | FND_C,			},
		{ FND_A | FND_F | FND_E | FND_D,	FND_A | FND_D,					FND_A| FND_D,							FND_A | FND_D | FND_C | FND_D,	},
	};
	
	
	uint32_t remainSec = g_washingMgr.remainSecond;
	
	uint8_t seconds = remainSec % 60;		 // 초 (0~59)
	uint8_t secOnes = remainSec % 10;         // 초의 일의 자리 (0-9)
	uint8_t secTens = remainSec / 10;         // 초의 십의 자리 (0~5)

	uint8_t minutes = remainSec / 60;        // 분 (0-59)
	uint8_t minOnes = minutes % 10;          // 분-일의 자리 (0-9)
	uint8_t minTens = (minutes / 10) % 6;    // 분-십의 자리 (0~5)
	
	int cycle_sec = g_washingMgr.remainSecond % FND_MOTER_CYCLE_MAX;
	
	switch(inDigitSelect)
	{
		
		case EFND1_Digit_D4:
		set_fnd1_digit_port(FND1_DIGIT_D4);
		FND_DATA_PORT = get_digit_num(secOnes);
		break;
		
		case EFND1_Digit_D3:
		set_fnd1_digit_port(FND1_DIGIT_D3);
		FND_DATA_PORT = get_digit_num(secTens);
		break;
		
		case EFND1_Digit_D2:
		set_fnd1_digit_port(FND1_DIGIT_D2);
		FND_DATA_PORT =	get_digit_num(minOnes);
		break;
		
		case EFND1_Digit_D1:
		set_fnd1_digit_port(FND1_DIGIT_D1);
		FND_DATA_PORT =	get_digit_num(minTens);
		break;
		
		default:
		break;
	}
}

void fnd_reset() {
	FND_DATA_PORT = FND_NULL;
}