/*
 * uart0.c
 *
 * Created: 2025-03-10 오전 10:32:13
 *  Author: microsoft
 */ 

#include "uart0.h"
#include "common.h"


/* 
	PC comportmaster로 부터 1byte가 들어올 때마다 RX INT 호출
	ex ) led_all_on\n 한글자씩 들어오므로 11번 들어옴
*/

volatile uint8_t rx_msg_received = 0;


extern volatile int msec_count;


/// led_on   
// l,e,d,_,o,n, '\n'
// 1byte stop statrt 10
// 8bit 
ISR(USART0_RX_vect/*_VECTOR(18)*/) {
	
	// INT안에서 사용하는 것은 volatile 사용해줘야함
	static volatile uint8_t idx = 0;
	
	volatile uint8_t rx_data;
	
	
	// UDRn  page 221 
	rx_data = UDR0; // uart0의 h/w register(UDR0)로 부터 1byte를 읽음
	// 이후 UDR0의 내용은 자동으로 비워짐 
	
	// new line 체크
	if(rx_data == '\n') {
		
		rx_buff[rear++][idx] = '\0';
		rear %= COMMAND_NUMBER;
		idx = 0;
		// check queue full 
		
		//rx_msg_received = 1; // 완전한 msg 수신이 완료
		//rx_buff[idx] = '\0'; // null char set
		//idx = 0;
		
	} else {
		// check command length 
		rx_buff[rear][idx++] = rx_data;
	}
}

/*
	1. 전송 속도 : 9600bps : ( 총 글자수 : 9600/10bit = 960자)
		1 char를 송&수신하는데 소요되는 시간 : 약 1ms ( 1/960 sec)
	2. 비동기 방식 ( start/stop 부호 비트로 데이터를 구분)
	3. RX (수신) : INT 방식으로 구현 ( TX는 pollin으로 할거임)

*/
void init_uart0(void) {
	
	// UBRRn ( USART Baud Rate Register ) 
	
	// 9600 bps 설정 218page
	// UBRR register
	UBRR0H = 0x00; // 상위는 4비트를 나타냄
	UBRR0L = 207; //  하위 8비트를 나타냄 9600 bps  // 
	
	
	// 2배속 통신 (샘플링을 2배로 ) 
	// 215 page 표 9-1
	UCSR0A |= 1 << U2X0; // 2배속 통신 ( 2배모드 1, 1배모드 0 )
	
	
	// UCSRC register 216 page
	// UMSEL0 비동기, 동기 
	// USBS0 정지비트 수 설정
	// UPM00 UPM01패리티비트 설정
	// UCPOL0  UCSZ00 UCSZ01나머지는 데이터비트 수 설정?
	UCSR0C |= 0x06; // 비동기, data 8bit, non 패리티, stopbit 1
	
	// UCSR0B  page215 9-2
	// RXEN0 : UART0로부터 수신이 가능하도록 설정
	// TXEN0 : UART0로부터 송신이 가능하도록 설정
	// RXCIE0 : UART0로 부터 1 byte가 들어오면  (stop bit를 만나면) RX INT를 발생시켜라
	UCSR0B |= 1 << RXEN0 | 1 << TXEN0 | 1 << RXCIE0;
	
}

// UART0로 1byte를 전송하는 함수 ( polling 방식)
void UART0_transmit(uint8_t data) {
	
	// 데이터 전송 중이면 전송이 끝날 때까지 대기
	
	// page 215 표 9-1
	// UCSRnA 
	// UDREn 
	
	// 계속 기다림
	while(!(UCSR0A & 1 << UDRE0)) {
		;
	}
	
	
	UDR0 = data; // data를 H/w 전송 register에 send
	
}

void pc_command_processing() {
	
	//static const char* ledModes[ELed_Mode_Max] = {
	//	"led_all_on",
	//	"led_all_off",
	//	"shift_left_ledon",
	//	"shift_right_ledon",
	//	"shift_left_keep_led_on",
	//	"shift_right_keep_led_on",
	//	"flower_ledon",
	//	"flower_ledoff",
	//};
	

	
	
	//if(front != rear) { // rx_buffer에 data 존재
	//	
	//	printf("%s\n", (char*)rx_buff[front]); // rx_buffer 
	//	
	//	int bFind = 0;
	//	for(int i = 0; i < ELed_Mode_Max; ++i) {
	//		
	//		if(strncmp(rx_buff[front], ledModes[i], strlen(ledModes[i])) == NULL) {
	//			
	//			printf("success find : %s\n", ledModes[i]);
	//			g_ELodeMode = i;
	//			bFind = 1;
	//			msec_count = LED_DELAY;
	//			break;
	//		}
	//	}
	//	
	//	if(!bFind) {
	//		printf("fail : %s!!!!\n", rx_buff[front]);
	//	}
	//	
	//	++front;
	//	front %= COMMAND_NUMBER;
	//	
	//	// check queue full
	//
	//}
	//else {
	//	// 새로운 데이터 x
	//}
	
}