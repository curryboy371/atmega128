/*
 * car.h
 *
 * Created: 2025-03-23 오후 4:14:49
 *  Author: pscsp
 */ 


#ifndef CAR_H_
#define CAR_H_

#define DISTANCE 25
#define TURN_DISTANCE 20

#define LR_DIFF_DISTANCE 25

#define CRITICAL_DISTANCE 3

void init_car();
void init_post_car();

void update_car();
void operate_car();



void update_event();

void update_manual_direction();
void update_auto_direction();

void operate_manual_car();
void operate_auto_car();

void auto_mode_check();


void input_button();
void change_mode();

void start_manual_mode();
void start_auto_mode();

void display_lcd(const char* inFirstLine, const char* inSecLine);

void reset_car();


#endif /* CAR_H_ */