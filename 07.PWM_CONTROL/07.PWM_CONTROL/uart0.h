/*
 * uart0.h
 *
 * Created: 2025-03-10 오전 10:32:22
 *  Author: microsoft
 */ 


#ifndef UART0_H_
#define UART0_H_

#define F_CPU 16000000UL	// 16MHZ 설정
#include <avr/io.h>
#include <util/delay.h> // for _delay_ms _delay_us ...
#include <avr/interrupt.h> // interrupt
#include <stdio.h>
#include <string.h>

#include "def.h"

// led_all_on\n
//volatile uint8_t rx_buff[COMMAND_LENGTH];	// uart0로부터 들어온 문자를 저장하는 버퍼
volatile uint8_t rx_buff[COMMAND_NUMBER][COMMAND_LENGTH];	// 2차원 arr로 변경

// atomic은 처리 안해줘도 되는건가?

volatile uint32_t rear; // input index : ISR UART0 RX에서 집어 넣는 index
volatile uint32_t front; // output index : 


void init_uart0(void);

void UART0_transmit(uint8_t data);


void pc_command_processing();


#endif /* UART0_H_ */