/*
 * ultrasonic.c
 *
 * Created: 2025-03-12 오후 2:49:18
 *  Author: microsoft
 */ 

#include "ultrasonic.h"
#include "led.h"

extern volatile int ultrasonic_check_timer;

extern volatile int ultrasonic_trigger_count;

uint8_t bOnTrigger = 0;

volatile int ultrasonic_dist = 0;
volatile char scm[50];

// ISR INT7_vec
// 278page 표 12-3
// PE4 외부 INT4 초음파 센서의 상승 하강 에지 둘다 INT4_vec로 들어옴
// 상승 하강 한번씩 2번 들어옴
ISR(INT7_vect) {
	
	// 인터럽트가 들어왔는데... echo의 값이 high이면 상승이고
	if(ECHO_PIN & 1 << ECHO ) {
	// 상승에지
	
		
	} 
	else {
	// low인 경우는 하강임
	// 하강 에지
	
	// ECHO 핀에 들어온 펄스 갯수를 us로 환산
	ultrasonic_dist = 1000000.0 * TCNT2 * 1024 / F_CPU; // 거리 구하기

	}
}


// ultrasonic 초기화
void init_ultrasonic() {
	
	// DDRG 4번 포트만 output mode 설정
	TRIG_DDR |= 1 << TRIG; 
	
	// DDRE 4번 포트만 input모드로 설정
	ECHO_DDR &= ~(1 << ECHO);
	
	
	// EICRB 레지스터 참고!
	// ISC 비트 설정에 따른 이터럽트 발생 시점 
	//apge 287 12-6
	// 0 1 : 상승 에지 ( rising edge) 하강 (falling edge) 둘 다 INT를 띄우도록
	EICRB |= 0 << ISC71 | 1 << ISC70;
	
	// EICRB(External Interrupt Control Register B)는 **외부 인터럽트 4~7 (INT4 ~ INT7)**의 동작 모드를 설정하는 레지스터입니다.
	
	
	
	// 16bit timer 1번을 설정해서 사용 65535 (max)
	// 16Mhz를 1024로 분주 = 16,000,000/ 1,024 = 15,625 ( 15.625KHz)
	// 1주기 T(주기) = 1/15626 = 0.000064sec  64us
	// 왜 이렇게 주기를 맞췄는가
	// 318page 14-1 표
	
	// TCCR0 이거랑은 무슨 차이인지 확인!!
	
	// TCCR0: 8비트 타이머/카운터0 (Timer/Counter0)를 제어하는 레지스터.
	// 이거는 8비트 타이머 카운터 레지스터 ( 0번)
	
	
	// TCCR1: 16비트 타이머/카운터1 (Timer/Counter1)를 제어하는 레지스터. (1번)
	// 이거는 16비트 타이머 카운터 레지스터
	// 16비트이므로 레지스터가 A, B 두개로 나눠진 것
	// 즉, 타이머 카운터 레지스터를 1번으로 사용하겠다고 한 것 
	ULTRASONIC_TIMER_TCCR |= 1 << CS22 | 1 << CS20; // 1024로 분주 설정
	
	//TCNT1 = 0; // 이래도 될 것 같은데
	
	// 문 열어놓기
	// 287 page 12-6
	// EIMSK(External Interrupt Mask Register)은 외부 인터럽트를 활성화 & 비활성화하는 레지스터
	EIMSK |= 1 << ULTRASONIC_INT                                          ; // external int 7 ( echo pin )
	
	
	// ps
	// 0번 타이머에서 사용한 TIMSK (Timer Interrupt Mask Register)
	// 타이머 관련 인터럽트(Timer Overflow, Compare Match, Input Capture 등)**를 활성화
	// 즉... 타이머에 대한 오버플로우를 인터럽트를 활성화 했던 것.
	
	// 펄스를 몇개 카운트 했는지를 시간을 거리로 계산할거임
	
}

void trigger_ultrasonic() {
	
	// 트리거 날리기 :  10us 
	
	// trigger port(4)만 low로 변경
	TRIG_PORT &= ~(1 << TRIG);
	_delay_ms(1);
	
	// trigger port(4) high로 변경 ( trigger 발생 시작) 
	TRIG_PORT |= 1 << TRIG;
	
	// 최소 10us 만큼 trig를 high로 둬야함
	
	
	ultrasonic_trigger_count = 15;
	bOnTrigger = 1;
	//_delay_ms(15); // 규격은 10us이나 오차방지를 위해 +5us 추가해주었음
	
	// trigger port(4) 다시 low 로 변경 ( trigger 종료 ) 
	TRIG_PORT &= ~(1 << TRIG);
}

void update_ultrasonic() {
	
	//트리거 동작중
	if(bOnTrigger) {
			
		// 대기상태
		if(ultrasonic_trigger_count > 0) {
			return;
		}
			
		// 대기 완료 후 다시 trigger low
		set_trigger_low();
		bOnTrigger = 0;
	}
		
}

uint8_t distance_ultrasonic(uint8_t* outDistance) {
	
	if(bOnTrigger) {
		return 0;
	}
	
	if(ultrasonic_check_timer >= 1000) {
		
		ultrasonic_check_timer= 0;
		
		*outDistance = ultrasonic_dist / 58;
		
		if(*outDistance < 0) {
			*outDistance = 0;
		}
		
		// 출력
		//printf("%s \r\n", scm);
		
		// 트리거 다시 콜
		set_trigger_low();
		_delay_ms(1);
		
		
		set_trigger_high();
		ultrasonic_trigger_count = 15;
		bOnTrigger = 1;
		
		return 1;
	}
	
	return 0;
}


void set_trigger_low() {
	
	TRIG_PORT &= ~(1 << TRIG);
}		

void set_trigger_high() {
	
	TRIG_PORT |= 1 << TRIG;
}
	






