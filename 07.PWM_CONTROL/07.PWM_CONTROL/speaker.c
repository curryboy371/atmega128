/*
 * speaker.c
 *
 * Created: 2025-03-14 오전 2:18:16
 *  Author: microsoft
 */ 

#include "speaker.h"
#include "def.h"

#include "led.h"



int School_Bell_Tune[] = {SO_01, SO_01, LA_01, LA_01, SO_01, SO_01, MI_01, 
					 	  SO_01, SO_01, MI_01, MI_01, RE_01, 
					 	  SO_01, SO_01, LA_01, LA_01, SO_01, SO_01, MI_01, 
					 	  SO_01, MI_01, RE_01, MI_01, DO_01,'/0'};
						   
const int School_Bell_Beats[] = {BEAT_1_4, BEAT_1_4, BEAT_1_4, BEAT_1_4, BEAT_1_4, BEAT_1_4, BEAT_1_2,
						   BEAT_1_4, BEAT_1_4, BEAT_1_4, BEAT_1_4, BEAT_1,
						   BEAT_1_4, BEAT_1_4, BEAT_1_4, BEAT_1_4, BEAT_1_4, BEAT_1_4, BEAT_1_2, 
						   BEAT_1_4, BEAT_1_4, BEAT_1_4, BEAT_1_4, BEAT_1};
						   
int He_Pirate[] = {RE_04, RE_04, RE_04, RE_04,
	RE_04, RE_04, RE_04, LA_03, DO_04,
	RE_04, RE_04, RE_04, MI_04,
	FA_04, FA_04, FA_04, SO_04,
	MI_04, MI_04, RE_04, DO_04,
	DO_04, RE_04, 0, LA_03, DO_04,
	RE_04, RE_04, RE_04, MI_04,
	FA_04, FA_04, FA_04, SO_04,
	MI_04, MI_04, RE_04, DO_04,
	RE_04, 0, 0, LA_03, DO_04,
	RE_04, RE_04, RE_04, FA_04,
	SO_04, SO_04, SO_04, LA_04,
	LA_04s, LA_04s, LA_04, SO_04,
	
	LA_04, RE_04, 0, RE_04, MI_04,
	FA_04, FA_04, SO_04,
	LA_04, RE_04, 0, RE_04, FA_04,
	MI_04, MI_04, FA_04, RE_04,
	MI_04, 0, 0, LA_04, DO_05,
	RE_05, RE_05, RE_05, MI_05,
	FA_05, FA_05, FA_05, SO_05,
	MI_05, MI_05, RE_05, DO_05,
	DO_05, RE_05, 0, LA_04, DO_05,
	RE_05, RE_05, RE_05, MI_05,
	FA_05, FA_05, FA_05, SO_05,
	MI_05, MI_05, RE_05, DO_05,
	RE_05, 0, 0, LA_04, DO_05,
	RE_05, RE_05, RE_05, FA_05,
	SO_05, SO_05, SO_05, LA_05,
	LA_05s, LA_05s, LA_05, SO_05,
	LA_05, RE_05, 0, RE_05, MI_05,
	FA_05, FA_05, SO_05,
	LA_05, RE_05, 0, RE_05, FA_05,
	MI_05, MI_05, RE_05, DO_05,
	RE_05, RE_05, MI_05,
	
	FA_05, FA_05, FA_05, SO_05,
	LA_05, FA_05, 0, 0, FA_05, RE_05,
	LA_04, 0, 0, 0,
	LA_04s, 0, 0, SO_05, RE_05,
	LA_04s, 0, 0, 0,
	MI_05, MI_05, RE_05,
	FA_04, 0, FA_04, SO_04,
	LA_04, LA_04, LA_04,
	LA_04s, LA_04,0 , 0,
	SO_04, SO_04, SO_04,
	SO_04, LA_04, 0, 0,
	LA_04, LA_04, LA_04,
	LA_04s, LA_04, 0, 0,
	SO_04, FA_04, MI_04,
	RE_04, 0, 0, RE_04, MI_04,
	FA_04, SO_04, LA_04,
	SO_04, FA_04, MI_04,
	FA_04, SO_04, LA_04,
	SO_04, 0, 0, FA_04, SO_04,
	LA_04, 0, 0, SO_04, FA_04,
	MI_04, FA_04, MI_04,
	RE_04, 0, 0, MI_04, DO_04,
	RE_04, 0, 0, RE_05, MI_05,
	
	FA_05, 0, 0, MI_05, FA_05,
	SO_05, FA_05, SO_05,
	LA_05, SO_05, FA_05,
	RE_05, 0, 0, RE_05, MI_05,
	FA_05, SO_05, LA_05,
	LA_05s, RE_05, SO_05,
	FA_05, 0, 0, SO_05, MI_05,
	RE_05, 0, 0, MI_05, DO_05s,
	LA_05, 0, 0, LA_05s, 0, 0,
	LA_05, LA_05, LA_05,
	LA_05, SO_05, 0, 0,
	SO_05, 0, 0,
	FA_05, 0, 0,
	FA_05, SO_05, MI_05,
	RE_05, RE_05, MI_05, FA_05,
	LA_05, RE_05, MI_05, FA_05,
	LA_05s, RE_05, MI_05, FA_05,
	LA_05, LA_05, DO_06,
	LA_05, SO_05, 0, 0,
	SO_05, 0, 0,
	FA_05, 0, 0,
	FA_05, SO_05, MI_05,
	RE_05, 0, 0,
RE_04, '/0'};
int He_Pirate1_Beat[] = {BEAT_1_4, BEAT_1_8, BEAT_1_4, BEAT_1_8,
	BEAT_1_4, BEAT_1_8, BEAT_1_16, BEAT_1_16, BEAT_1_16,
	BEAT_1_4, BEAT_1_4, BEAT_1_8, BEAT_1_8,
	BEAT_1_4, BEAT_1_4, BEAT_1_8, BEAT_1_8,
	BEAT_1_4, BEAT_1_4, BEAT_1_8, BEAT_1_8,
	BEAT_1_8, BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_8,
	BEAT_1_4, BEAT_1_4, BEAT_1_8, BEAT_1_8,
	BEAT_1_4, BEAT_1_4, BEAT_1_8, BEAT_1_8,
	BEAT_1_4, BEAT_1_4, BEAT_1_8, BEAT_1_8,
	BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8,
	BEAT_1_4, BEAT_1_4, BEAT_1_8, BEAT_1_8,
	BEAT_1_4, BEAT_1_4, BEAT_1_8, BEAT_1_8,
	BEAT_1_4, BEAT_1_4, BEAT_1_8, BEAT_1_8,
	
	BEAT_1_8, BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_8,
	BEAT_1_4, BEAT_1_4, BEAT_1_4,
	BEAT_1_8, BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_8,
	BEAT_1_4, BEAT_1_4, BEAT_1_8, BEAT_1_8,
	BEAT_1_8, BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_8,
	BEAT_1_4, BEAT_1_4, BEAT_1_8, BEAT_1_8,
	BEAT_1_4, BEAT_1_4, BEAT_1_8, BEAT_1_8,
	BEAT_1_4, BEAT_1_4, BEAT_1_8, BEAT_1_8,
	BEAT_1_8, BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_8,
	BEAT_1_4, BEAT_1_4, BEAT_1_8, BEAT_1_8,
	BEAT_1_4, BEAT_1_4, BEAT_1_8, BEAT_1_8,
	BEAT_1_4, BEAT_1_4, BEAT_1_8, BEAT_1_8,
	BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8,
	BEAT_1_4, BEAT_1_4, BEAT_1_8, BEAT_1_8,
	BEAT_1_4, BEAT_1_4, BEAT_1_8, BEAT_1_8,
	BEAT_1_4, BEAT_1_4, BEAT_1_8, BEAT_1_8,
	BEAT_1_8, BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_8,
	BEAT_1_4, BEAT_1_4, BEAT_1_4,
	BEAT_1_8, BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_8,
	BEAT_1_4, BEAT_1_4, BEAT_1_8, BEAT_1_8,
	BEAT_1_4, BEAT_1_4, BEAT_1_4,
	
	BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_4,
	BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8,
	BEAT_1_8, BEAT_1_8, BEAT_1_4, BEAT_1_4,
	BEAT_1_8, BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_8,
	BEAT_1_8, BEAT_1_8, BEAT_1_4, BEAT_1_4,
	BEAT_1_8, BEAT_1_4, BEAT_1_2,
	BEAT_1_2, BEAT_1_8, BEAT_1_8, BEAT_1_8,
	BEAT_1_4, BEAT_1_4, BEAT_1_4,
	BEAT_1_8, BEAT_1_8, BEAT_1_4, BEAT_1_4,
	BEAT_1_4, BEAT_1_4, BEAT_1_4,
	BEAT_1_8, BEAT_1_8, BEAT_1_4, BEAT_1_4,
	BEAT_1_4, BEAT_1_4, BEAT_1_4,
	BEAT_1_8, BEAT_1_8, BEAT_1_4, BEAT_1_4,
	BEAT_1_4, BEAT_1_4, BEAT_1_4,
	BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8,
	BEAT_1, BEAT_1_8, BEAT_1_8,
	BEAT_1_4, BEAT_1_4, BEAT_1_4,
	BEAT_1_4, BEAT_1_4, BEAT_1_4,
	BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8,
	BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8,
	BEAT_1_4, BEAT_1_4, BEAT_1_4,
	BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8,
	BEAT_1_8, BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_8,
	
	BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8,
	BEAT_1_4, BEAT_1_4, BEAT_1_4,
	BEAT_1_4, BEAT_1_4, BEAT_1_4,
	BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8,
	BEAT_1_4, BEAT_1_4, BEAT_1_4,
	BEAT_1_4, BEAT_1_4, BEAT_1_4,
	BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8,
	BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8,
	BEAT_1_4, BEAT_1_4, BEAT_1_4,
	BEAT_1_4, BEAT_1_4, BEAT_1_4,
	BEAT_1_4, BEAT_1_4, BEAT_1_4,
	BEAT_1_8, BEAT_1_8, BEAT_1_4, BEAT_1_4,
	BEAT_1_4, BEAT_1_4, BEAT_1_4,
	BEAT_1_4, BEAT_1_4, BEAT_1_4,
	BEAT_1_4, BEAT_1_4, BEAT_1_4,
	BEAT_1_2, BEAT_1_8, BEAT_1_8, BEAT_1_8,
	BEAT_1_2, BEAT_1_8, BEAT_1_8, BEAT_1_8,
	BEAT_1_2, BEAT_1_8, BEAT_1_8, BEAT_1_8,
	BEAT_1_4, BEAT_1_4, BEAT_1_4,
	BEAT_1_8, BEAT_1_8, BEAT_1_4, BEAT_1_4,
	BEAT_1_4, BEAT_1_4, BEAT_1_4,
	BEAT_1_4, BEAT_1_4, BEAT_1_4,
	BEAT_1_2, BEAT_1_8, BEAT_1_4,
BEAT_1};

const int Elise_Tune[] = {MI_02, RE_02_H, MI_02, RE_02_H, MI_02, TI_01, RE_02, DO_02, LA_01, 0,
	DO_01, MI_01, LA_01, TI_01, 0, MI_01, SO_01_H, TI_01, DO_02, 0,
	MI_01, MI_02, RE_02_H, MI_02, RE_02_H, MI_02, TI_01, RE_02, DO_02, LA_01, 0,
	DO_01, MI_01, LA_01, TI_01, 0, MI_01, DO_02, TI_01, LA_01,
	MI_02, RE_02_H, MI_02, RE_02_H, MI_02, TI_01, RE_02, DO_02, LA_01, 0,
	DO_01, MI_01, LA_01, TI_01, 0, MI_01, SO_01_H, TI_01, DO_02, 0,
	MI_01, MI_02, RE_02_H, MI_02, RE_02_H, MI_02, TI_01, RE_02, DO_02, LA_01, 0,
	DO_01, MI_01, LA_01, TI_01, 0, MI_01, DO_02, TI_01, LA_01, 0,
	TI_01, DO_02, RE_02, MI_02, 0, SO_01, FA_02, MI_02, RE_02, 0, FA_01, MI_02, RE_02, DO_02, 0,
	MI_01, RE_02, DO_02, TI_01, 0, MI_01, MI_02, MI_03,
	RE_02_H, MI_02, RE_02_H, MI_02, RE_02, MI_02, RE_01_H, MI_02, TI_01, RE_01, DO_02, LA_01, 0,
	DO_01, MI_01, LA_01, TI_01, 0, MI_01, SO_01_H, TI_01, DO_02, 0,
	MI_01, MI_02, RE_02_H, MI_02, RE_02_H, MI_02, TI_01, RE_02, DO_02, LA_01, 0, DO_01, MI_01, LA_01, TI_01, 0,
RE_01, DO_02, TI_01, LA_01,'/0'};

const int Elise_Beats[] = 
   { BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_4, BEAT_1_4,
	BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_4, BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_4, BEAT_1_4,
	BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_4, BEAT_1_4,
	BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_4, BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_2,
	BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_4, BEAT_1_4,
	BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_4, BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_4, BEAT_1_4,
	BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_4, BEAT_1_4,
	BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_4, BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_2, BEAT_1_8,
	BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_4, BEAT_1_8,
	BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8,
	BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8,
	BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8,
	BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8,
BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8};

int GrandFa_Tune[] = {SO_01, DO_02, TI_01, DO_02, RE_02, DO_02, RE_02, MI_02, FA_02, MI_02, LA_01, RE_02, RE_02, DO_02, DO_02, DO_02, TI_01, LA_01, TI_01, DO_02, 0,
	SO_01, DO_02, TI_01, DO_02, RE_02, DO_02, RE_02, MI_02, FA_02, MI_02, LA_01, RE_02, RE_02, DO_02, DO_02, DO_02, TI_01, LA_01, TI_01, DO_02, 0,
	DO_02, MI_02, SO_02, MI_02, RE_02, DO_02, TI_01, DO_02, RE_02, DO_02, TI_01, LA_01, SO_01, DO_02, MI_02, SO_02, MI_02, RE_02, DO_02, TI_01, DO_02, RE_02,
SO_01, DO_02, RE_02, MI_02, FA_02, MI_02, LA_01, RE_02, RE_02, DO_02, DO_02, DO_02, TI_01, LA_01, TI_01, DO_02, '/0'};

const int GrandFa_Beats[] = {BEAT_1_4, BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_2, BEAT_1_4,
	BEAT_1_4, BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_2, BEAT_1_4,
	BEAT_1_8, BEAT_1_8, BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_4,
BEAT_1_4, BEAT_1_2, BEAT_1_2, BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_2};


#define BEEP_DURATION 200  // 200ms

volatile uint32_t button_play_count = 0;  // 반복 횟수
volatile uint8_t play_state = 0;  //ON/OFF 상태



volatile uint32_t music_count = 0;
uint8_t bplay_music = 0;
uint32_t music_index = 0;

int* g_tone = 0;
int* g_beats = 0;

// Timer3 위상교정 PWM
void init_speaker(void)
{
	init_speaker_pwm();
	//init_speaker_compare();
}

void init_speaker_pwm() {
	
	SPEAKER_TIMER_DDR |= 1 << SPEAKER_TIMER_PIN; 
	
	
	// 비교일치 발생 시 출력 반전 (Toggle on Compare Match)
	//  (P328 표14-6 참고)
	//SPEAKER_TIMER_TCCRA = (1 << COM1B0);
	//SPEAKER_TIMER_TCCRA |= (1 << COM1B0);
	SPEAKER_TIMER_TCCRA |= (1 << COM1A0);
	
	// CTC 모드 설정 (WGM12) (P327 표14-5)
	SPEAKER_TIMER_TCCRB |= (1 << WGM12);
	
	// 분주비 8 설정 (CS11)
	SPEAKER_TIMER_TCCRB |= (1 << CS11);
	
	SPEAKER_TIMER_TCCRC = 0;  // P328 그림 14-11 참고
		
		
	SPEAKER_OCR = 0;;   // 
}

void init_speaker_compare() {
	
	SPEAKER_TIMER_DDR |= 1 << SPEAKER_TIMER_COUNT_PIN;  // pb4 출력 OC1B
	
	//16,000,000 Hz / 8
	// 카운트1 : 0.0000005초 
	
	
	// OCR1B 비교 매치 값을 설정 (1ms마다 실행되도록 조정)
	SPEAKER_OCR = 2000;  // 1ms 주기 설정
	// 0.0000005초*2000 = 1ms	 
	
	// OCR1B 비교 매치 인터럽트 활성화
	TIMSK |= (1 << OCIE1B);
}


void start_music(t_EMusic_Type inMusic)
{
	switch(inMusic) {
		case EMusic_Elise:
		g_tone = Elise_Tune;
		g_beats = Elise_Beats;
		break;
		
		case EMusic_GrandFa:
		g_tone = GrandFa_Tune;
		g_beats = GrandFa_Beats;
		break;
		
		case EMusic_He_Pirate:
		g_tone = He_Pirate;
		g_beats = He_Pirate1_Beat;
		break;
		
		case EMusic_School_Bell:
		g_tone = School_Bell_Tune;
		g_beats = School_Bell_Beats;
		break;

		default:
		break;
	}
	
	bplay_music = 1;
	music_count = music_index = 0;
}

// return 1 음악종료
// return 0 음악 재생중
uint8_t play_music() {
	
	if(!bplay_music) {
		return 1;
	}
	
	if(!g_tone || !g_beats) {
		return 1;
	}
	
	if(music_count > 0) {
		return 0;
	}
		
	if(music_index & 1) {
		music_count = 10;
		SPEAKER_OCR = 0;
	}
	else {
			
		// 음악이 끝났는지
		// 아놔.. 왜 /0이거로 해둔거야... 어이없네
		if (*g_tone == '/0' || *g_beats == '/0') {
			SPEAKER_OCR = 0;
			music_index = music_count = 0;
			bplay_music = 0;
			printf("Music Finished!\r\n");
			return 1;
		}
			
		// 새로운 음 재생
		SPEAKER_OCR = *g_tone;
		music_count = *g_beats;
		
		// 다음 음으로
		++g_tone;
		++g_beats;
	}
	
	++music_index;
	return 0;
}


uint8_t get_play_state() {
	
	return play_state;
}

void set_play_button(uint8_t bState) {
	
	play_state = bState;
	
	if(play_state == PLAY_OFF) {
		button_play_count = 0;
		SPEAKER_OCR = 0;
	}
	
	if(play_state == PLAY_ON) {
		SPEAKER_OCR = 600;  // 0.00025sec (250us) : 0.0000005 * 500
		button_play_count = BUTTON_DELAY;
	}
	
}

void play_button() {
		
	if(play_state == PLAY_ON) {
		return;
	}	
	
	set_play_button(PLAY_ON);
}

void Beep(int repeat)
{
	int  i;
	
	for(i=0; i < repeat; i++)
	{
		SPEAKER_OCR = 500;  // 0.00025sec (250us) : 0.0000005 * 500

		
		_delay_ms(200);
		SPEAKER_OCR = 0;
		_delay_ms(200);
	}
}

void Siren(int repeat)
{
	int i, j;
	
	SPEAKER_OCR = 900;
	
	for(i=0; i < repeat; i++)
	{
		for(j=0; j < 100; j++)
		{
			SPEAKER_OCR += 10;
			_delay_ms(20);
		}
		for(j=0; j < 100; j++)
		{
			SPEAKER_OCR -= 10;
			_delay_ms(20);
		}
	}
}

void RRR(void)
{
	int i;
	
	for(i=0; i<20; i++)
	{
		SPEAKER_OCR = 1136;
		_delay_ms(100);
		SPEAKER_OCR = 0;
		_delay_ms(20);
	}
}

void delay_ms(int ms)
{
	while(ms-- != 0) {
		
		_delay_ms(1);
		
	}
}