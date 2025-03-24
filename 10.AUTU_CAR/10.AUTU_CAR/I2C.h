/*
 * I2C.h
 *
 * Created: 2020-01-07 오후 7:57:06
 *  Author: kccistc
 */ 


#ifndef I2C_H_
#define I2C_H_

//typedef enum {
//	EI2C_STATE_NONE,			// Init 이전
//	EI2C_STATE_IDLE,
//	EI2C_STATE_START_SENT,
//	EI2C_STATE_SLA_SENT,
//	EI2C_STATE_DATA_SENT,
//	EI2C_STATE_DONE,
//	EI2C_STATE_ERROR
//	
//} t_I2C_STATE;

typedef enum {
	I2C_NONE,
	I2C_IDLE,
	I2C_START_SENT,
	I2C_SLA_W_SENT,
	I2C_TRANSMITTING,
	I2C_WRITE_DONE
} t_I2C_STATE;



#define TWI_START_SENT            0x08  // start 전송 성공 - start를 전송하고 start 신호가 성공적인지 하드웨어적인 확인
#define TWI_SLA_W_ACK             0x18  // 주소를 전송 성공 - address + w을 전송하고 ack
#define TWI_SLA_W_NACK            0x20  // 주소를 전송 실패 - address + w을 전송하고 nack
#define TWI_DATA_TX_ACK           0x28  // data 전송 성공 - data를 전송하고 ack 받음
#define TWI_DATA_TX_NACK          0x30  // data 전송 실패 - data를 전송하고 nack 받음

#define I2C_WRITE_MAX_SIZE 10
#define I2C_TIME_OUT 1000


void I2C_init(unsigned int baud);
void I2C_start(void);

void I2C_write_data(uint8_t inAddress, uint8_t inData);
void I2C_write_datas(uint8_t inAddress, uint8_t* pData, uint8_t inLen);
void I2C_write_string(uint8_t inAddress, char* inStr);

void I2C_transmit(uint8_t data);
void I2C_write_byte(uint8_t address, uint8_t data);
void I2C_stop(void);
uint8_t I2C_receive_ACK(void);
uint8_t I2C_receive_NACK(void);


uint8_t get_write_done_flag();
void set_write_done_flag(uint8_t inFlag);

#endif /* I2C_H_ */