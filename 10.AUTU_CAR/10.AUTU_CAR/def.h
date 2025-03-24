/*
 * df.h
 *
 * Created: 2025-03-10 오후 3:05:45
 *  Author: microsoft
 */ 


#ifndef DEF_H_
#define DEF_H_



#define MAX_UINT8_COUNT  256


#define HIGH 1
#define LOW  0

#define TRUE 1
#define FALSE 0
#define SUCCESS 1
#define FAILED  0


#define ULTRASONIC_LENGTH		3


#define COMMAND_NUMBER	10
#define COMMAND_LENGTH	40


#define ULTRASONIC_DELAY 2000
#define BUTTON_DELAY 250

#define DEF_FPS	  1



#define MANUAL_MODE		0			  // bt command run
#define DISTANCE_CHECK	1			  // 초음파 거리 측정
#define MODE_CHECK		2			  // auto or manual btn check
#define AUTO_MODE		3			  // 자율 주행


#define PLAY_REPEAT		1
#define PLAY_ONCE		0


typedef enum {
	EMUSIC_NONE,
	EMUSIC_AMBULANCE,
	EMUSIC_POLICE,
	EMUSIC_SCHOOL_BELL,
	EMUSIC_ELI,
	EMUSIC_MAX,
	
} t_EMusic_Type;

typedef enum {
	EMANUAL_MODE, 			  // bt command run
	EAUTO_MODE,				  // 자율 주행
}t_ECAR_MODE;

typedef enum {
	EDIR_STOP,
	EDIR_FOWARD,
	EDIR_BACKWARD,
	EDIR_RIGHT,
	EDIR_LEFT,
	EDIR_MAX,
	
}t_EDIR_MODE;







#endif /* DEF_H_ */