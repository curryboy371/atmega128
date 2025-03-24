/*
 * I2C.c
 *
 * Created: 2020-01-07 오후 7:58:00
 *  Author: kccistc
 */ 
#include <avr/io.h>
#include <avr/interrupt.h>

#include "I2C.h"

// slave transmitter mode에서 twsr 상태 동작 흐름
//TWSR : TWI Status 레지스터
// TWI의 상태를 감지하는 레지스터가 존재함
// 하드웨어가 TWI 데이터 송수신 단계를 TWSR로 알려줌...

volatile t_I2C_STATE eI2C_state = I2C_NONE;
volatile uint8_t I2C_txBuffer[I2C_WRITE_MAX_SIZE];
volatile uint8_t I2C_txIndex = 0;
volatile uint8_t I2C_txLength = 0;

volatile uint8_t I2C_done = 0; // i2c 전송이 완료되었는지

volatile uint16_t I2C_timeOut = 0;


// 인터럽트로 I2C 통신 처리
ISR(TWI_vect)
{
	// 기존 풀링 방식 TWCR = (1<<TWINT) | (1<<TWEN);
	// 인터럽트 사용은 TWIE 추가
	
	switch (TWSR & 0xF8) // 상위 5bit만 사용 ( i2c state code)
	{
		case TWI_START_SENT: // start > success ( ack는 없음 )
		TWDR = I2C_txBuffer[I2C_txIndex++]; // address + w
		TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWIE);
		eI2C_state = I2C_SLA_W_SENT;
		break;

		case TWI_SLA_W_ACK: // address + w > ack
		TWDR = I2C_txBuffer[I2C_txIndex++]; // data
		TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWIE);
		eI2C_state = I2C_TRANSMITTING;
		break;

		case TWI_DATA_TX_ACK: // data > ack
		if (I2C_txIndex < I2C_txLength)
		{
			TWDR = I2C_txBuffer[I2C_txIndex++]; // data
			TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWIE);
		}
		else
		{
			TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);  // STOP
			eI2C_state = I2C_WRITE_DONE;
			I2C_timeOut = 0;
			I2C_done = 1;
		}
		break;

		default:  // 에러 처리 → STOP
		TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
		eI2C_state = I2C_IDLE;
		I2C_timeOut = 0;
		I2C_done = 1;
		break;
	}
}

void I2C_init(unsigned int baud){
	
	eI2C_state = I2C_IDLE;
	TWBR = baud;
	// I2C의 속도를 설정
}

void I2C_start(void)
{
	// TWCR : I2C (TWI) 통신을 제어하는 레지스터
	// TWINT : TWI 인터럽트 플래그
	// TWSTA : START 조건 전송
	// TWEN : TWEN 활성화
	
	eI2C_state = I2C_START_SENT;
	I2C_txIndex = 0;
	I2C_done = 0;
	
	TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);
	
	// start 조건이 완료될 때까지 기다림
	while (!(TWCR & (1<<TWINT)));  // 시작 완료 대기
}

void I2C_write_data(uint8_t inAddress, uint8_t inData) {
	
	eI2C_state = I2C_START_SENT;
	I2C_done = I2C_txIndex = I2C_txLength= 0;
	
	// 주소를 먼저 입력
	I2C_txBuffer[I2C_txLength++] = inAddress;
	
	// buffer에 data 입력
	I2C_txBuffer[I2C_txLength++] = inData;
	
	// 기존 start에서 interrupt로 처리하기 위해 TWIE를 추가
	TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN) | (1 << TWIE);
}

void I2C_write_datas(uint8_t inAddress, uint8_t* pData, uint8_t inLen) {
	
	eI2C_state = I2C_START_SENT;
	I2C_done = I2C_txIndex = I2C_txLength= 0;
		
	// 주소를 먼저 입력
	I2C_txBuffer[I2C_txLength++] = inAddress;
	
	// buffer에 data 입력	
	for (int i = 0; i < inLen; i++) {
		I2C_txBuffer[I2C_txLength++] = pData[i];
	}
		
	// 기존 start에서 interrupt로 처리하기 위해 TWIE를 추가
	TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN) | (1 << TWIE);
}

void I2C_write_string(uint8_t inAddress, char* inStr) {
	
	eI2C_state = I2C_START_SENT;
	I2C_done = I2C_txIndex = I2C_txLength= 0;
	
	// 주소를 먼저 입력
	I2C_txBuffer[I2C_txLength++] = inAddress; 
	
	// buffe에 data write
	while(*inStr != '\0') {
		I2C_txBuffer[I2C_txLength++] = *inStr++;
		
		if(I2C_txLength > I2C_WRITE_MAX_SIZE) {
			break;
		}
	}
	
	I2C_timeOut = I2C_TIME_OUT;
	
	// 기존 start에서 interrupt로 처리하기 위해 TWIE를 추가
	TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN) | (1 << TWIE);
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

uint8_t get_write_done_flag() {
	return I2C_done;
}

void set_write_done_flag(uint8_t inFlag) {
	I2C_done = inFlag;
	eI2C_state = I2C_IDLE;
}