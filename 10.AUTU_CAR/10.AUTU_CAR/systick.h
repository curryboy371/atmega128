/*
 * systick.h
 *
   Use Timer0
   
 * Created: 2025-03-23 오후 5:10:40
 *  Author: pscsp
 */ 



#ifndef SYSTICK_H_
#define SYSTICK_H_


void init_systick();
void update_tick();
void reset_time();

#endif /* SYSTICK_H_ */