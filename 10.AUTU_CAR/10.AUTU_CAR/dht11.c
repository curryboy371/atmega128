/*
 * dht11.c
 *
 * Created: 2025-03-18 오전 10:52:29
 *  Author: microsoft
 */ 

#include "dht11.h"

//uint8_t us_count;
//enum state_define state;

#define READ_DHCPIN			((DHT_PIN & 1 << DHT_PIN_NUM) >> DHT_PIN_NUM)

#define SET_DHCDDR_OUTPUT	(DHT_DDR |= 1 << DHT_PIN_NUM)
#define SET_DHCDDR_INPUT	(DHT_DDR &= ~(1 << DHT_PIN_NUM))

#define SET_DHCPORT_HIGH   	(DHT_PORT |= 1 << DHT_PIN_NUM)
#define SET_DHCPORT_LOW		(DHT_PORT &= ~(1 << DHT_PIN_NUM))

void init_dht11() {
	
	// ddr은 출력 모드로
	// 포트는 defualt hight 로 시작
	SET_DHCDDR_OUTPUT;
	SET_DHCPORT_HIGH;
	
	// 상태 초기화	
	state = OK;
}


void get_dht11_data(uint8_t outData[])
{
	//start signal
	start_signal();
	
	// hand shake
	if(check_dht_handshake() != OK) {
		printf("hand shake timeout \n");
		return;
	}
	
	// recieve data
	if(receive_dhc_data(outData) != OK) {
		printf("data recieve error %d", state);
		return;
	}
	
}


enum state_define receive_dhc_data(uint8_t outData[]) {
			
	for(int i = 0; i<5; i++)
	{
		uint8_t pulse[8] = {0,};
		for(int j = 7; j>=0; j--)
		{
			if(check_response_timeout(70, LOW) != OK ) {
				return TIMEOUT;
			}
			
			if(check_response_timeout(90, HIGH) != OK ) {
				return TIMEOUT;
			}
			
			if(us_count < 40) {
				pulse[j] = 0; // '0'
			}
			else if(us_count >= 40) {
				pulse[j] = 1; // '1'
			}
		}
		
		//memcpy(&outData[i], &pulse, sizeof(uint8_t) * 8);
		outData[i] = pulse[0] << 0 | pulse[1] << 1 | pulse[2] << 2 | pulse[3] << 3 | pulse[4] << 4 | pulse[5] << 5 | pulse[6] << 6 | pulse[7] << 7;
	}
			
	if(outData[4] != outData[0] + outData[1] + outData[2] + outData[3]) {
		return VALUE_ERROR;
	}
	
	_delay_us(60);
	
	return OK;
		
}

void start_signal() {
	
	init_dht11();
	_delay_ms(100);
			
	SET_DHCPORT_LOW;
	_delay_ms(20); // 18ms
			
	SET_DHCPORT_HIGH;
	SET_DHCDDR_INPUT;
	_delay_us(1);
}

enum state_define check_dht_handshake() {
	
	// response check...
	if(check_response_timeout(50, HIGH) != OK) {
		printf("response signal timeout\r\n");
		return TIMEOUT;
	}
		
	if(check_response_timeout(100, LOW) != OK) {
		printf("response signal check time out\r\n");
		return TIMEOUT;
	}
		
	if(check_response_timeout(100, HIGH) != OK) {
		printf("response high check timeout \r\n");
		return TIMEOUT;
	}
	
	return OK;
};

enum state_define check_response_timeout(uint8_t inTimeOutCnt, uint8_t inReadValue) {
	
	us_count = 0;
	while(READ_DHCPIN == inReadValue)
	{
		_delay_us(2);
		us_count += 2;
		if(us_count > inTimeOutCnt)
		{
			return TIMEOUT;
		}
	}
	return OK;
};









