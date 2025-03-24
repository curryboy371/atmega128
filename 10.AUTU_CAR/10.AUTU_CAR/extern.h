/*
 * extern.h
 *
 * Created: 2025-03-20 오전 2:29:17
 *  Author: microsoft
 */ 


#ifndef EXTERN_H_
#define EXTERN_H_

#include "def.h"

extern volatile uint8_t rx_msg_received;

volatile uint16_t play_count;

extern uint8_t bEchoActive;
extern uint8_t bDistancePending; // 거리 계산 예약 중인 상태

extern volatile int ultrasonic_check_timer;
extern volatile int ultrasonic_trigger_counts[ULTRASONIC_LENGTH];
extern volatile int ultrasonic_dist[ULTRASONIC_LENGTH];

extern volatile int msec_count;
extern volatile int msec_button_delay;
extern volatile int g_fps;


extern volatile int turn_delay_count;
extern volatile int dir_delay_count;
extern volatile int back_delay_count;

extern volatile uint16_t I2C_timeOut;

extern volatile uint8_t bt_data;

extern int func_index;



extern t_EMusic_Type eCurPlay;

extern t_EDIR_MODE eCurDirection;
extern t_EDIR_MODE eNewDirection;
extern t_ECAR_MODE eDriveMode;
extern uint8_t bAutoMode;

#endif /* EXTERN_H_ */