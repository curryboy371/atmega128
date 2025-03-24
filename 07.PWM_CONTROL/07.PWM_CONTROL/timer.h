/*
 * timer.h
 *
 * Created: 2025-03-15 오후 5:49:27
 *  Author: pscsp
 
 이런식으로 redef은 좋은 방법은 아님
 타이머 활용에 익숙해지려고 직접 redef해보면서 사용해보기 위해서 만들었음
 
 */ 


#ifndef TIMER_H_
#define TIMER_H_

// timer0 ( 8bit ) 
// micro sec count를 위한 timer count 사용 ( OVF INT )
// pwm을 사용하지 않으므로 pin 사용 안함

#define TIMER0_TCCR		TCCR0

// timer1 ( 16bit ) 
// 스피커 재생을 위해서 사용

// ddr port
#define TIMER1_DDR			DDRB
#define TIMER1_PORT			PORTB

#define TIMER1_PIN5			5	
#define TIMER1_PIN6			6

#define TIMER1_OCRA			OCR1A
#define TIMER1_OCRB			OCR1B
#define TIMER1_OCRC			OCR1C

#define TIMER1_TCCRA		TCCR1A
#define TIMER1_TCCRB		TCCR1B
#define TIMER1_TCCRC		TCCR1C


	
// top 값 설정시 사용
#define TIMER1_ICR			ICR1


// timer2 ( 8bit )
// 초음파 거리 측정을 위한 timer count 값 사용
// pwm을 사용하지 않으므로 pin 사용 안함

#define TIMER2_TCCR			TCCR2


// timer3 ( 16bit )
// moter 사용 - pwm 제어

// timer port pin
#define TIMER3_DDR			DDRE
#define TIMER3_PORT			PORTE

#define TIMER3_PIN3			3
#define TIMER3_PIN4			4
#define TIMER3_PIN5			5

#define TIMER3_TCCRA		TCCR3A		// 출력 비교 모드, pwm 설정
#define TIMER3_TCCRB		TCCR3B		// 타이머 동작모드, 분주비 설정
#define TIMER3_TCCRC		TCCR3C	


#define TIMER3_OCRA			OCR3A
#define TIMER3_OCRB			OCR3B
#define TIMER3_OCRC			OCR3C

#define TIMER3_ICR			ICR3

#endif /* TIMER_H_ */