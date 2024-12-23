/*
 * timer.c
 *
 *  Created on: Sep 30, 2024
 *      Author: User
 */

#include "global.h"

 #include "timer.h"
#include "fsm_uart.h"


#define NUM_TIMERS 10  // Number of timers you want to handle
#define TICK 10
int timer_flag[NUM_TIMERS];  // Timer flags for each timer
int timer_counter[NUM_TIMERS];  // Timer counters for each timer

void setTimer(int timer_id, int duration)
{
	if (timer_id >= 0 && timer_id < NUM_TIMERS)
	    {
	        timer_counter[timer_id] = duration / TICK;
	        timer_flag[timer_id] = 0;
	    }
}
void timerRun(void)
{
    for (int i = 0; i < NUM_TIMERS; i++)
    {
        if (timer_counter[i] >= 0)
        {
            timer_counter[i]--;

            if (timer_counter[i] <= 0)
            {
                timer_flag[i] = 1;
            }
        }
    }
}
void update_handlemode(){
	if (timer_flag[1] == 1){
		run();
		setTimer(1,3000);
	 }
	if (timer_flag[2] == 1){
		HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
		setTimer(2,1000);
	 }
}

void HAL_TIM_PeriodElapsedCallback (TIM_HandleTypeDef *htim)
{
	if (htim->Instance == TIM2){
		timerRun();
	}


}

