/*
 * I2C.c
 *
 * Created: 2020-01-07 오후 7:58:00
 *  Author: kccistc
 */ 
#include <avr/io.h>
#include "I2C.h"

void I2C_init(unsigned int baud){
	TWBR = baud;
	// I2C의 속도를 설정
}

void I2C_start(void)
{
	// TWCR : I2C (TWI) 통신을 제어하는 레지스터
	// TWINT : TWI 인터럽트 플래그
	// TWSTA : START 조건 전송
	// TWEN : TWEN 활성화
	
	TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);
	while (!(TWCR & (1<<TWINT)));  // 시작 완료 대기
}

void I2C_transmit(uint8_t data)
{
	TWDR = data;
	TWCR = (1<<TWINT) | (1<<TWEN);
	while (!(TWCR & (1<<TWINT)));
}

void I2C_write_byte(uint8_t address, uint8_t data)
{
	// 이 함수 호출로 address data 쓰기 한번에 끝내
	
	I2C_start();
	I2C_transmit(address);
	I2C_transmit(data);
	I2C_stop();
}

void I2C_stop(void)
{
	TWCR = (1<<TWINT)|(1<<TWEN)| (1<<TWSTO);
}

uint8_t I2C_receive_ACK(void)
{
	TWCR = (1<<TWINT) | (1<<TWEN) |(1<<TWEA);
	
	while( !(TWCR & (1<<TWINT)));             // 수신 완료 대기
	
	return TWDR;
}

uint8_t I2C_receive_NACK(void)
{
	TWCR = (1<<TWINT) | (1<<TWEN);
	
	while( !(TWCR & (1<<TWINT)));             // 수신 완료 대기
	
	return TWDR;
}

