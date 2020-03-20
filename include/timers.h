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

#include <stdio.h>
#include "esp_types.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/periph_ctrl.h"
#include "driver/timer.h"

#define TIMER_DIVIDER         16  //  Hardware timer clock divider
#define TIMER_SCALE           (TIMER_BASE_CLK / TIMER_DIVIDER)  // convert counter value to seconds
#define TIMER_INTERVAL1_SEC   (5.78)   // sample test interval for the second timer
#define WITH_RELOAD      1        // With auto reload


extern xQueueHandle timer_queue;
void tg0_timer_init(timer_idx_t timer_idx, bool auto_reload, double timer_interval_sec);

#ifdef __cplusplus
}
#endif

#endif /* MAIN_INCLUDE_TIMERS_H_ */
