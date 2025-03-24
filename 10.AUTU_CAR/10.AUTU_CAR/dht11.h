/*
 * dht11.h
 *
 * Created: 2025-03-18 오전 10:52:38
 *  Author: microsoft
 */ 


#ifndef DHT11_H_
#define DHT11_H_

#define  F_CPU 16000000UL  // 16MHZ
#include <avr/io.h>
#include <util/delay.h>  // _delay_ms _delay_us
#include <avr/interrupt.h>
#include <stdlib.h>
#include <stdio.h>		// printf

#include "def.h"

#define DHT_DDR			DDRG
#define DHT_PORT		PORTG

#define DHT_PIN			PING
#define DHT_PIN_NUM		0





typedef uint8_t t_bool;

enum state_define {
	
	OK,
	TIMEOUT,
	VALUE_ERROR,
	TRANS_ERROR
};


uint8_t us_count;
enum state_define state;
void init_dht11();
void get_dht11_data(uint8_t outData[]);

void start_signal();
enum state_define check_dht_handshake();

enum state_define check_response_timeout(uint8_t inTimeOutCnt, uint8_t inReadValue);

enum state_define  receive_dhc_data(uint8_t outData[]);


#endif /* DHT11_H_ */