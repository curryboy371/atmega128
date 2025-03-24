/*
 * fnd.h
 *
 * Created: 2025-03-06 오후 12:23:21
 *  Author: microsoft
 */ 


#ifndef FND_H_
#define FND_H_


#include <stdio.h>
#include "def.h"


// anode 모드인 경우 define
// casode 모드라면 ndefine
//#define FND_COMMON_ANODE 


/*

fnd 7segment 
b3 : a 
b2 : b
b1 : c
b0 : d
a7 : e
a6 : f 
a5 : g 
a4 : dot

fnd1 : digit :  c 0123
fnd2 : digit : c4567

*/

typedef enum EFND_Digit_All{
	EFND1_Digit_D4 = 0,		// [ ][ ][ ][1]
	EFND1_Digit_D3,			// [ ][ ][1][ ]
	EFND1_Digit_D2,			// [ ][1][ ][ ]
	EFND1_Digit_D1,			// [1][ ][ ][ ]
	EFND_Digit_All_MAX
} t_EFND_Digit_All;


#define FND_MOTER_CYCLE_MAX 12
#define FND_CYCLE_MAX 16


#define FND1_DIGIT_PORT		PORTB
#define FND1_DIGIT_DDR		DDRB


// data poart c 0-7
#define FND_DATA_PORT	PORTC
#define FND_DATA_DDR	DDRC

// fnd1 digit pb01234
#define FND1_DIGIT_D1	0
#define FND1_DIGIT_D2	1
#define FND1_DIGIT_D3	2
#define FND1_DIGIT_D4	3

#define FND_COMMON_DIGIT_D1 0
#define FND_COMMON_DIGIT_D2 1
#define FND_COMMON_DIGIT_D3 2
#define FND_COMMON_DIGIT_D4 3

#define FND_LENGTH		4

#ifdef FND_COMMON_ANODE
#define FND_NUM_DOT		(uint8_t)0x7f
#define FND_NUM_0		(uint8_t)0xc0
#define FND_NUM_1		(uint8_t)0xf9
#define FND_NUM_2		(uint8_t)0xa4
#define FND_NUM_3		(uint8_t)0xb0
#define FND_NUM_4		(uint8_t)0x99
#define FND_NUM_5		(uint8_t)0x92
#define FND_NUM_6		(uint8_t)0x82
#define FND_NUM_7		(uint8_t)0xd8
#define FND_NUM_8		(uint8_t)0x80
#define FND_NUM_9		(uint8_t)0x90

#define FND_NULL		(uint8_t)~0b00000000
#define FND_A			(uint8_t)~0b00000001
#define FND_B			(uint8_t)~0b00000010
#define FND_C			(uint8_t)~0b00000100
#define FND_D			(uint8_t)~0b00001000
#define FND_E			(uint8_t)~0b00010000
#define FND_F			(uint8_t)~0b00100000
#define FND_G			(uint8_t)~0b01000000
#else
#define FND_NUM_DOT		(uint8_t)~0x7f
#define FND_NUM_0		(uint8_t)~0xc0
#define FND_NUM_1		(uint8_t)~0xf9
#define FND_NUM_2		(uint8_t)~0xa4
#define FND_NUM_3		(uint8_t)~0xb0
#define FND_NUM_4		(uint8_t)~0x99
#define FND_NUM_5		(uint8_t)~0x92
#define FND_NUM_6		(uint8_t)~0x82
#define FND_NUM_7		(uint8_t)~0xd8
#define FND_NUM_8		(uint8_t)~0x80
#define FND_NUM_9		(uint8_t)~0x90

#define FND_NULL		(uint8_t)0b00000000
#define FND_A			(uint8_t)0b00000001
#define FND_B			(uint8_t)0b00000010
#define FND_C			(uint8_t)0b00000100
#define FND_D			(uint8_t)0b00001000
#define FND_E			(uint8_t)0b00010000
#define FND_F			(uint8_t)0b00100000
#define FND_G			(uint8_t)0b01000000
#endif

#define FND_S_VAR	FND_A | FND_F | FND_G | FND_C | FND_D
#define FND_T_VAR	FND_A | FND_B | FND_C 
#define FND_O_VAR	FND_NUM_0
#define FND_P_VAR	FND_A | FND_B | FND_G | FND_F | FND_E


#define FND_A_VAR  	FND_A | FND_F | FND_B | FND_E | FND_C | FND_G
#define FND_B_VAR  	FND_NUM_8

void init_fnd();

void set_fnd1_digit_port(uint8_t inDigitPortNum);
uint8_t get_digit_num(uint8_t inDigitNum);

void fnd_display(uint8_t inbAutoMode);
void fnd_manual_mode(t_EFND_Digit_All inDigitSelect);
void fnd_auto_mode(t_EFND_Digit_All inDigitSelect);

#endif /* FND_H_ */