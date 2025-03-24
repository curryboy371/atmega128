/*
 * common.h
 *
 * Created: 2025-03-07 오후 4:33:26
 *  Author: microsoft
 */ 


#ifndef COMMON_H_
#define COMMON_H_

#include <stdio.h>



typedef enum {
	
	EMusic_Elise,
	EMusic_GrandFa,
	EMusic_He_Pirate,
	EMusic_School_Bell,
	EMusic_Max,
	
} __attribute__((packed)) t_EMusic_Type;


typedef enum {
	EWashMode_Standby,     // 대기 상태 (전원은 켜졌으나 동작 없음)
	EWashMode_Select_Auto, // 자동모드 선택
	EWashMode_Select_Man,  // 수동 선택
	EWashMode_Wash,        // 본 세탁 (세제 사용)
	EWashMode_Spin,        // 탈수
	EWashMode_Finish,       // 완료
	EWashMode_Error,       // 오류 발생
	EWashMode_Stop,        // 정지
	EWashMode_Max          // 모드 개수 (배열 크기 체크용)
} __attribute__((packed)) t_EWash_Mode;

typedef enum {
	ESelect_None,
	ESelect_WashTime,
	ESelect_WashPower,
	ESelect_SpinTime,
	ESelect_SpinPower,
	ESelect_Max          
} __attribute__((packed)) t_ESelect_Type;


typedef struct {
	
	// 이거 bit 쪼개서 써도 되긴할듯...
	uint8_t bIsRun;
	uint8_t bIsMode; // 0자동 or 1수동
	
	
	t_EWash_Mode ePreWashMode; // 직전모드 (일시정지용)
	t_EWash_Mode eWashMode;
	
	t_ESelect_Type eSelectType; // 수동 선택 타입
	
	uint32_t remainSecond; // 남은시간
	uint32_t progressSecond; // 진행시간
	
	
	uint8_t washTime; // 세탁 시간
	uint8_t spinTime; // 탈수 시간
	
	uint8_t washPower; // 세탁 파워
	uint8_t spinPower; // 탈수 파워
	
	uint8_t curInputValue; // 입력중 value
	
	uint32_t errorValue;	// 에러코드
	
}t_WashingManager;

#endif /* COMMON_H_ */