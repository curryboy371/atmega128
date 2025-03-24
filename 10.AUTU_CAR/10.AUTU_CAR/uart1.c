/*
 * uart0.c
 *
 * Created: 2025-03-10 오전 10:32:13
 *  Author: microsoft
 */ 

#define F_CPU 16000000UL	// 16MHZ 설정
#include <avr/io.h>
#include <util/delay.h> // for _delay_ms _delay_us ...
#include <avr/interrupt.h>
#include <string.h>



#include "uart1.h"



volatile uint8_t bt_data;

ISR(USART1_RX_vect/*_VECTOR(30)*/) {
	
	bt_data = UDR1;
	
	UART0_transmit(bt_data);	// BT로부터 들어온 문자가 어떤 것인지 확인하기 위해서 comport master로 출력
}

/*
	1. 전송 속도 : 9600bps : ( 총 글자수 : 9600/10bit = 960자)
		1 char를 송&수신하는데 소요되는 시간 : 약 1ms ( 1/960 sec)
	2. 비동기 방식 ( start/stop 부호 비트로 데이터를 구분)
	3. RX (수신) : INT 방식으로 구현 ( TX는 pollin으로 할거임)

*/
void init_uart1(void) {
	
	// UBRRn ( USART Baud Rate Register ) 
	
	// 9600 bps 설정 218page
	// UBRR register
	UBRR1H = 0x00; // 상위는 4비트를 나타냄
	UBRR1L = 207; //  하위 8비트를 나타냄 9600 bps  // 
	
	
	// 2배속 통신 (샘플링을 2배로 ) 
	// 215 page 표 9-1
	UCSR1A |= 1 << U2X1; // 2배속 통신 ( 2배모드 1, 1배모드 0 )
	
	
	// UCSRC register 216 page
	// UMSEL0 비동기, 동기 
	// USBS0 정지비트 수 설정
	// UPM00 UPM01패리티비트 설정
	// UCPOL0  UCSZ00 UCSZ01나머지는 데이터비트 수 설정?
	UCSR1C |= 0x06; // 비동기, data 8bit, non 패리티, stopbit 1
	
	// UCSR1B  page215 9-2
	// RXEN1 : UART1로부터 수신이 가능하도록 설정
	// TXEN1 : UART1로부터 송신이 가능하도록 설정
	// RXCIE1 : UART1로 부터 1 byte가 들어오면  (stop bit를 만나면) RX INT를 발생시켜라
	UCSR1B |= 1 << RXEN1 | 1 << TXEN1 | 1 << RXCIE1;
	
}

// UART1로 1byte를 전송하는 함수 ( polling 방식)
void UART1_transmit(uint8_t data) {
	
	// 데이터 전송 중이면 전송이 끝날 때까지 대기
	
	// page 215 표 9-1
	// UCSRnA 
	// UDREn 
	
	// 계속 기다림
	while(!(UCSR1A & 1 << UDRE1)) {
		;
	}
	
	
	UDR1 = data; // data를 H/w 전송 register에 send
	
}
