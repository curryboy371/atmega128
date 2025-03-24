/*
 * washing_machine.h
 *
 * Created: 2025-03-15 오후 9:54:51
 *  Author: pscsp
 */ 


#ifndef WASHING_MACHINE_H_
#define WASHING_MACHINE_H_


#include <stdio.h>
#include <stdbool.h>
#include "common.h"



void init_washing_machine();

void run_empty();

void update_washing_machine();
void update_evnet();

void operate_washing_machine();

void run_standby();
void run_select_mode();
void run_select_man();
void run_wash();
void run_spin();
void run_finish();
void run_error();
void run_stop();

void start_wash();
void start_spin();
void finish_spin();
void restart_working();
void stop_working();
void start_error(uint32_t inErrorCode);

void handle_timeout();

void reset_wash_data();
void set_auto_mode_data();
void set_man_mode_data();
void set_wash_mode(t_EWash_Mode inMode);

uint8_t check_valid_distance();

bool input_button_select();
bool input_button_next();
bool input_button_previous();
bool input_button_return();




#endif /* WASHING_MACHINE_H_ */