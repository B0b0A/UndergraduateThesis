/*
 * timers.h
 *
 *  Created on: Mar 20, 2020
 *      Author: Bojan ALeksovski
 */

#ifndef MAIN_INCLUDE_TIMERS_H_
#define MAIN_INCLUDE_TIMERS_H_


#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"
#include "driver/periph_ctrl.h"
#include "driver/timer.h"
#include "timers.h"

#define TIMER_DIVIDER         16  //  Hardware timer clock divider
#define TIMER_SCALE           (TIMER_BASE_CLK / TIMER_DIVIDER)  // convert counter value to seconds
#define TIMER_INTERVAL0_SEC   (60) // sample test interval for the first timer
#define TEST_WITHOUT_RELOAD   0        // testing will be done without auto reload
#define TEST_WITH_RELOAD      1        // testing will be done with auto reload

extern xQueueHandle timer_queue;

void initTimerGroup0(timer_idx_t timer_idx, bool auto_reload, double timer_interval_sec);
void deinitTimer(timer_group_t timer_group, timer_idx_t timer_idx);

#ifdef __cplusplus
}
#endif

#endif /* MAIN_INCLUDE_TIMERS_H_ */
