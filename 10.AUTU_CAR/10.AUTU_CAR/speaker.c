/*
 * speaker.c
 *
 * Created: 2025-03-14 오전 2:18:16
 *  Author: microsoft
 */ 

#define F_CPU 16000000UL	// 16MHZ 설정

#include <util/atomic.h>

#include <avr/io.h>
#include <util/delay.h> // for _delay_ms _delay_us ...
#include <avr/interrupt.h>
#include <string.h>

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

volatile uint16_t play_count = 0;

uint8_t isIncreaseSiren;

uint8_t bplay_music;

uint8_t play_type = PLAY_REPEAT;

t_EMusic_Type eCurPlay;

uint32_t play_index = 0;

int* g_tone = 0;
int* g_beats = 0;

// Timer3 위상교정 PWM
void init_speaker(void)
{
	init_speaker_pwm();
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

void start_music(t_EMusic_Type inMusic, uint8_t inPlayType)
{
	switch(inMusic) {
		case EMUSIC_NONE:
		stop_sound();
		return;

		case EMUSIC_ELI:	
		g_tone = Elise_Tune;
		g_beats = Elise_Beats;
		break;
		
		case EMUSIC_SCHOOL_BELL:
		g_tone = School_Bell_Tune;
		g_beats = School_Bell_Beats;
		break;
		
		case EMUSIC_AMBULANCE:
		isIncreaseSiren = TRUE;
		SPEAKER_OCR = SIREN_MIN_VALUE;
		break;
		
		case EMUSIC_POLICE:
		isIncreaseSiren = FALSE;
		SPEAKER_OCR = SIREN_MIN_VALUE;
		break;

		default:
		break;
	}
	
	bplay_music = 1;
	play_type = inPlayType;
	eCurPlay = inMusic;
	play_count = play_index = 0;
}

// return 1 음악종료
// return 0 음악 재생중
uint8_t play_sound() {
	
	if(!bplay_music) {
		return 1;
	}
	
	if(play_count > 0) {
		return 0;
	}
	
	uint8_t res = 0;
	
	switch(eCurPlay) {
		case EMUSIC_NONE:
		return 1;
		
		case EMUSIC_ELI:		
		case EMUSIC_SCHOOL_BELL:
		res = play_music();
		break;
			
		case EMUSIC_AMBULANCE:
		embulance_siren(10);
		break;
		case EMUSIC_POLICE:
		police_siren();
		break;

		default:
		break;
	}
		
	return res;
}

void stop_sound() {
	
	bplay_music = 0;
	eCurPlay = EMUSIC_NONE;
	play_count = play_index = 0;
	
	SPEAKER_OCR = 0;
}

uint8_t play_music() { 
	
	if(!g_tone || !g_beats) {
		return 1;
	}

	if(play_index & 1) {
		play_count = 10;
		SPEAKER_OCR = 0;
	}
	else {
				
		// 음악이 끝났는지
		// 아놔.. 왜 /0이거로 해둔거야... 어이없네
		if (*g_tone == '/0' || *g_beats == '/0') {
					
			if(play_type == PLAY_REPEAT) {
						
				start_music(eCurPlay, play_type);
				return 0;
			}
			SPEAKER_OCR = 0;
			play_index = play_count = 0;
			bplay_music = 0;
			printf("Music Finished!\r\n");
			return 1;
		}
				
		// 새로운 음 재생
		SPEAKER_OCR = *g_tone;
		play_count = *g_beats;
				
		// 다음 음으로
		++g_tone;
		++g_beats;
	}
			
	++play_index;
}



void embulance_siren(const int inDelta)
{
	
	// OCR값이 낮아 주기가 짧아져야 음이 높음
	
	if(SPEAKER_OCR > SIREN_MAX_VALUE) {
		isIncreaseSiren = TRUE;
	}
	else if(SPEAKER_OCR < SIREN_MIN_VALUE) {
		isIncreaseSiren = FALSE;
	}
	
	if(isIncreaseSiren) {
		SPEAKER_OCR -= inDelta;
		play_count = 10;
	}
	else {
		SPEAKER_OCR += inDelta;
		play_count = 30;
	}
}



void police_siren(void)
{
	isIncreaseSiren = !isIncreaseSiren;
	
	SPEAKER_OCR = isIncreaseSiren ? POLICE_MIN_VALUE : POLICE_MAX_VALUE;
	
	play_count = 1000;
}

uint8_t get_cur_sound_type() {
	
	uint8_t resType = LOW;
	
	switch(eCurPlay) {
		case EMUSIC_NONE:
		break;
		
		case EMUSIC_ELI:		
		case EMUSIC_SCHOOL_BELL:
		resType = SPEAKER_OCR == 0 ? LOW : HIGH;
		break;
				
		case EMUSIC_AMBULANCE:
		case EMUSIC_POLICE:
		resType = isIncreaseSiren;
		break;

		default:
		break;
		
	}
	
	return resType;
}

